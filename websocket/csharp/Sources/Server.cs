using System;
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
    public class WebSocket {
        private readonly HttpServer Server;
        private readonly IEnumerator<Bitmap> BitmapProducer;

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
                            Server.BitmapProducer.MoveNext();

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

                            Send(json);
                        } else if (request == "frame") {
                            var currentFrame = Server.BitmapProducer.Current;
                            var data = currentFrame.LockBits(new Rectangle(0, 0, currentFrame.Width, currentFrame.Height),
                                                             ImageLockMode.ReadOnly,
                                                             currentFrame.PixelFormat);

                            try {
                                var count = Math.Abs(data.Stride) * data.Height;
                                var bytes = new byte[count];

                                Marshal.Copy(data.Scan0, bytes, 0, count);
                                Send(bytes);
                            } finally {
                                currentFrame.UnlockBits(data);
                            }
                        } else if (request == "cameraInfo") {
                            Send("{\"id\":\"whatever\",\"name\":\"whatever\"}");
                        } else if (request == "execute") {
                            Send("{}");
                        } else if (request == "result") {
                            Send("{}");
                        }
                    }
                }
            }
        }

        public WebSocket(int port, IEnumerable<Bitmap> bitmapProducer) {
            Server = new HttpServer(port);
            Server.AddWebSocketService("/", MakeConnectionHandler);
            Server.AddWebSocketService("/via", MakeConnectionHandler);
            Server.Log.Level = LogLevel.Debug;

            BitmapProducer = bitmapProducer.GetEnumerator();
        }

        public void Start() => Server.Start();
        public void Stop() => Server.Stop();

        private ConnectionHandler MakeConnectionHandler() => new ConnectionHandler(this);
    }
}
