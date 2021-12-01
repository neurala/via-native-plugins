using System;
using System.Collections;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Net;
using System.Runtime.InteropServices;
using System.Threading;
using System.Threading.Tasks;

using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

using WebSocketSharp;
using WebSocketSharp.Server;

namespace Neurala.VIA {
    public interface IRequestHandler {
        void HandleResults(string results);
        void ExecuteAction(string action);
    }

    public class WebSocket {
        private readonly HttpServer Server;
        private readonly IEnumerator<Stream> ImageDataProducer;
        private readonly IRequestHandler RequestHandler;

        private sealed class ConnectionHandler : WebSocketBehavior {
            private const int exifOrientationID = 0x112; // 274

            private readonly WebSocket Server;

            private Image currentImage;

            private Image CurrentImage {
                get {
                    if (currentImage == null)
                        currentImage = Image.FromStream(CurrentStream);
                    return currentImage;
                }
            }

            private Stream CurrentStream {
                get {
                    var imageStream = Server.ImageDataProducer.Current;
                    imageStream.Seek(0L, SeekOrigin.Begin);
                    return imageStream;
                }
            }

            public ConnectionHandler(WebSocket server) {
                Server = server;
            }

            protected sealed override void OnMessage(MessageEventArgs @event) {
                JToken token;

                var message = JObject.Parse(@event.Data);

                lock (Server) {
                    if (message.TryGetValue("request", out token)) {
                        var request = token.ToObject<string>();

                        if (request == "metadata") {
                            Console.WriteLine("Got metadata request.");

                            var currentFrame = CurrentImage;

                            message = new JObject();
                            message["format"] = "jpg";
                            message["width"] = currentFrame.Width;
                            message["height"] = currentFrame.Height;
                            message["layout"] = "interleaved";
                            message["dataType"] = "uint8";
                            message["colorSpace"] = currentFrame.PixelFormat switch
                              { PixelFormat.Format24bppRgb => "BGR",
                                PixelFormat.Format32bppArgb => "BGRA",
                                _ => "unknown" };

                            var json = message.ToString();

                            Console.WriteLine(json);
                            Send(json);
                        } else if (request == "frame") {
                            Console.WriteLine("Got frame request.");

                            var currentFrame = CurrentImage;
#if _
                            var data = currentFrame.LockBits(new Rectangle(0, 0, currentFrame.Width, currentFrame.Height),
                                                             ImageLockMode.ReadOnly,
                                                             currentFrame.PixelFormat);

                            try {
                                var count = Math.Abs(data.Stride) * data.Height;
                                var bytes = new byte[count];

                                Marshal.Copy(data.Scan0, bytes, 0, count);
                                Console.WriteLine($"Sending {count} bytes of image data.");
                                Send(bytes);
                            } finally {
                                currentFrame.UnlockBits(data);
                                MoveNext();
                            }
#endif
                            var outputStream = ExifRotate(currentFrame);
                            var length = (int) outputStream.Length;
                            var bytes = new byte[length];

                            outputStream.Read(bytes, 0, length);
                            Send(bytes);
                            MoveNext();
                        } else if (request == "execute") {
                            var action = message["body"]["action"].ToString();
                            Console.WriteLine("Got execute request.");
                            Server.RequestHandler.ExecuteAction(action);
                            Send("{}");
                        } else if (request == "result") {
                            var body = message["body"].ToString();
                            Console.WriteLine("Got result request.");
                            Server.RequestHandler.HandleResults(body);
                            Send("{}");
                        }
                    }
                }
            }

            protected sealed override void OnOpen() {
                Console.WriteLine("Connection opened.");
            }

            protected sealed override void OnClose(CloseEventArgs @event) {
                Console.WriteLine("Connection closed.");
            }

            private void MoveNext() {
                currentImage = null;
                Server.ImageDataProducer.MoveNext();
            }

            // Adapted from https://stackoverflow.com/questions/27835064/get-image-orientation-and-rotate-as-per-orientation
            private Stream ExifRotate(Image img) {
                if (!Array.Exists<int>(img.PropertyIdList, value => value == exifOrientationID))
                    return CurrentStream;

                var prop = img.GetPropertyItem(exifOrientationID);
                int val = BitConverter.ToUInt16(prop.Value, 0);
                var rot = RotateFlipType.RotateNoneFlipNone;

                if (val == 3 || val == 4)
                    rot = RotateFlipType.Rotate180FlipNone;
                else if (val == 5 || val == 6)
                    rot = RotateFlipType.Rotate90FlipNone;
                else if (val == 7 || val == 8)
                    rot = RotateFlipType.Rotate270FlipNone;

                if (val == 2 || val == 4 || val == 5 || val == 7)
                    rot |= RotateFlipType.RotateNoneFlipX;

                if (rot != RotateFlipType.RotateNoneFlipNone) {
                    var stream = new MemoryStream(64 * 1024);
                    img.RotateFlip(rot);
                    img.Save(stream, ImageFormat.Jpeg);
                    return stream;
                }

                return CurrentStream;
            }
        }

        public WebSocket(int port, IRequestHandler requestHandler) : this(port, new SynchronousImageDataProducer(), requestHandler) { }

        public WebSocket(int port, IEnumerable<Stream> imageDataProducer, IRequestHandler requestHandler) {
            Server = new HttpServer(port);
            Server.AddWebSocketService("/", MakeConnectionHandler);
            Server.AddWebSocketService("/via", MakeConnectionHandler);
            Server.Log.Level = LogLevel.Debug;

            ImageDataProducer = imageDataProducer.GetEnumerator();
            ImageDataProducer.MoveNext();

            RequestHandler = requestHandler;
        }

        public void Start() => Server.Start();
        public void Stop() => Server.Stop();

        public void SendImage(string path) {
            var imageData = File.OpenRead(path);

            SendImage(imageData);
        }

        public void SendImage(Stream imageData) {
            if (ImageDataProducer is SynchronousImageDataProducer producer)
                producer.AddImage(imageData);
            else throw new InvalidOperationException("This operation is available only when a custom imageData producer is not provided.");
        }

        private ConnectionHandler MakeConnectionHandler() => new ConnectionHandler(this);
    }

    /// <summary>Synchronous adaptor for producing images.</summary>
    internal class SynchronousImageDataProducer : IEnumerable<Stream> {
        private readonly Queue<Stream> ImageDataQueue;

        public SynchronousImageDataProducer() {
            ImageDataQueue = new Queue<Stream>();
        }

        public void AddImage(Stream imageData) {
            lock (ImageDataQueue) {
                ImageDataQueue.Enqueue(imageData);

                if (ImageDataQueue.Count == 1)
                    Monitor.Pulse(ImageDataQueue);
            }
        }

        private IEnumerable<Stream> Enumerate() {
            lock (ImageDataQueue) {
                while (true) {
                    if (ImageDataQueue.Count == 0)
                        Monitor.Wait(ImageDataQueue);

                    var next = ImageDataQueue.Dequeue();

                    if (next == null)
                        break;

                    Monitor.Exit(ImageDataQueue);

                    yield return next;

                    Monitor.Enter(ImageDataQueue);
                }
            }
        }

        IEnumerator IEnumerable.GetEnumerator() => Enumerate().GetEnumerator();
        IEnumerator<Stream> IEnumerable<Stream>.GetEnumerator() => Enumerate().GetEnumerator();
    }
}
