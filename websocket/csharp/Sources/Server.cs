using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Net;
using System.Net.WebSockets;
using System.Runtime.InteropServices;
using System.Threading;
using System.Threading.Tasks;

using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

using WebSocketSharp;
using WebSocketSharp.Server;

namespace Neurala.VIA {
    public delegate Bitmap BitmapProducer();

    public class WebSocket {
        private readonly WebSocketServer Server;
        private readonly BitmapProducer BitmapProducer;

        private Bitmap CurrentFrame;

        private sealed class ConnectionHandler : WebSocketBehavior {
            private readonly WebSocket Server;

            public ConnectionHandler(WebSocket server) {
                Server = server;
            }

            protected sealed override void OnMessage(MessageEventArgs @event) {
                JToken token;

                var message = JObject.Parse(@event.Data);

                lock (Server) {
                    if (message.TryGetValue("metadata", out token)) {
                        var currentFrame = Server.CurrentFrame = Server.BitmapProducer();

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
                    } else if (message.TryGetValue("frame", out token)) {
                        var currentFrame = Server.CurrentFrame;
                        var roi = new Rectangle(0, 0, currentFrame.Width, currentFrame.Height);
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
                    }
                }
            }
        }

        public WebSocket(string prefix, int port, BitmapProducer bitmapProducer) {
            Server = new WebSocketServer(port);
            BitmapProducer = bitmapProducer;
            Server.AddWebSocketService("/", MakeConnectionHandler);
        }

        public void Start() => Server.Start();
        public void Stop() => Server.Stop();

        private ConnectionHandler MakeConnectionHandler() => new ConnectionHandler(this);
    }
}
