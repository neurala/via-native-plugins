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
        private readonly IEnumerator<Bitmap> BitmapProducer;
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

                            var currentFrame = Server.BitmapProducer.Current;

                            message = new JObject();
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

                            var currentFrame = Server.BitmapProducer.Current;
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
                                Server.BitmapProducer.MoveNext();
                            }
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

        public WebSocket(int port, IRequestHandler requestHandler) : this(port, new SynchronousBitmapProducer(), requestHandler) { }

        public WebSocket(int port, IEnumerable<Bitmap> bitmapProducer, IRequestHandler requestHandler) {
            Server = new HttpServer(port);
            Server.AddWebSocketService("/", MakeConnectionHandler);
            Server.AddWebSocketService("/via", MakeConnectionHandler);
            Server.Log.Level = LogLevel.Debug;

            BitmapProducer = bitmapProducer.GetEnumerator();
            BitmapProducer.MoveNext();

            RequestHandler = requestHandler;
        }

        public void Start() => Server.Start();
        public void Stop() => Server.Stop();

        public void SendImage(string path) {
            var bitmap = new Bitmap(path);

            SendImage(bitmap);
        }

        public void SendImage(Bitmap bitmap) {
            if (BitmapProducer is SynchronousBitmapProducer producer)
                producer.AddImage(bitmap);
            else throw new InvalidOperationException("This operation is available only when a custom bitmap producer is not provided.");
        }

        private ConnectionHandler MakeConnectionHandler() => new ConnectionHandler(this);
    }

    /// <summary>Synchronous adaptor for producing images.</summary>
    internal class SynchronousBitmapProducer : IEnumerable<Bitmap> {
        private readonly Queue<Bitmap> BitmapQueue;

        public SynchronousBitmapProducer() {
            BitmapQueue = new Queue<Bitmap>();
        }

        public void AddImage(string path) {
            var bitmap = new Bitmap(path);

            AddImage(bitmap);
        }

        public void AddImage(Bitmap bitmap) {
            lock (BitmapQueue) {
                BitmapQueue.Enqueue(bitmap);

                if (BitmapQueue.Count == 1)
                    Monitor.Pulse(BitmapQueue);
            }
        }

        private IEnumerable<Bitmap> Enumerate() {
            lock (BitmapQueue) {
                while (true) {
                    if (BitmapQueue.Count == 0)
                        Monitor.Wait(BitmapQueue);

                    var next = BitmapQueue.Dequeue();

                    if (next == null)
                        break;

                    Monitor.Exit(BitmapQueue);

                    yield return next;

                    Monitor.Enter(BitmapQueue);
                }
            }
        }

        IEnumerator IEnumerable.GetEnumerator() => Enumerate().GetEnumerator();
        IEnumerator<Bitmap> IEnumerable<Bitmap>.GetEnumerator() => Enumerate().GetEnumerator();
    }
}
