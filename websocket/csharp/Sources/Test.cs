using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Threading;

using Neurala.VIA;

public static class Program {
    private static CancellationTokenSource Canceler;
    private static WebSocket Server;

    private sealed class DummyHandler : IRequestHandler {
        void IRequestHandler.HandleResults(string results) {
            Console.WriteLine(results);
        }

        void IRequestHandler.ExecuteAction(string action) {
            Console.WriteLine(action);
        }
    }

    public static void Main(string[] arguments) {
        var portString = arguments[0];
        var port = Int32.Parse(portString);
        var imageDirectory = arguments[1];
        var imageProvider = IterateImageDirectoryForever(imageDirectory);
        var requestHandler = new DummyHandler();

        Canceler = new CancellationTokenSource();
        Server = new WebSocket(port, imageProvider, requestHandler);
        Server.Start();

        Console.CancelKeyPress += (_, _) => Canceler.Cancel();

        Canceler.Token.WaitHandle.WaitOne();
        Console.WriteLine("Stopping server.");
        Server.Stop();
    }

    private static IEnumerable<Bitmap> IterateImageDirectoryForever(string directory) {
        while (true) {
            var files = Directory.EnumerateFiles(directory);

            foreach (var file in files) {
                Console.WriteLine($"Trying to load image file \"{file}\".");
                yield return new Bitmap(file);
            }
        }
    }
}
