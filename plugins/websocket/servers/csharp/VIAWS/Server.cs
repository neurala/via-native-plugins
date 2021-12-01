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
        private readonly IEnumerator<byte[]> ImageDataProducer;
        private readonly IRequestHandler RequestHandler;

        private sealed class ConnectionHandler : WebSocketBehavior {
            private readonly WebSocket Server;

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

                            var currentFrame = Server.ImageDataProducer.Current;

                            message = new JObject();
#if _
                            message["width"] = currentFrame.Width;
                            message["height"] = currentFrame.Height;
                            message["layout"] = "interleaved";
                            message["dataType"] = "uint8";
                            message["colorSpace"] = currentFrame.PixelFormat switch
                              { PixelFormat.Format24bppRgb => "BGR",
                                PixelFormat.Format32bppArgb => "BGRA",
                                _ => "unknown" };
#endif
                            message["format"] = "jpg";

                            var json = message.ToString();

                            Console.WriteLine(json);
                            Send(json);
                        } else if (request == "frame") {
                            Console.WriteLine("Got frame request.");

                            var currentFrame = Server.ImageDataProducer.Current;
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
                                Server.ImageDataProducer.MoveNext();
                            }
#endif
                            Send(currentFrame);
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
        }

        public WebSocket(int port, IRequestHandler requestHandler) : this(port, new SynchronousImageDataProducer(), requestHandler) { }

        public WebSocket(int port, IEnumerable<byte[]> imageDataProducer, IRequestHandler requestHandler) {
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
#if _
            var imageData = new ImageData(path);
#endif
            var imageData = File.ReadAllBytes(path);

            SendImage(imageData);
        }

        public void SendImage(byte[] imageData) {
            if (ImageDataProducer is SynchronousImageDataProducer producer)
                producer.AddImage(imageData);
            else throw new InvalidOperationException("This operation is available only when a custom imageData producer is not provided.");
        }

        private ConnectionHandler MakeConnectionHandler() => new ConnectionHandler(this);
    }

    /// <summary>Synchronous adaptor for producing images.</summary>
    internal class SynchronousImageDataProducer : IEnumerable<byte[]> {
        private readonly Queue<byte[]> ImageDataQueue;

        public SynchronousImageDataProducer() {
            ImageDataQueue = new Queue<byte[]>();
        }

        public void AddImage(byte[] imageData) {
            lock (ImageDataQueue) {
                ImageDataQueue.Enqueue(imageData);

                if (ImageDataQueue.Count == 1)
                    Monitor.Pulse(ImageDataQueue);
            }
        }

        private IEnumerable<byte[]> Enumerate() {
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
        IEnumerator<byte[]> IEnumerable<byte[]>.GetEnumerator() => Enumerate().GetEnumerator();
    }
}
