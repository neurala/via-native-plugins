using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Threading;

using Neurala.VIA;

public static class Program {
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
        // Get port and image directory.
        var portString = arguments[0];
        var port = Int32.Parse(portString);
        var imageDirectory = arguments[1];

        // This is where you control how to send images and what to do with results.
        // All you need is a way to provide images, and a way to handle results.
        var requestHandler = new DummyHandler();

        // Create and start the WebSocket.
        Server = new WebSocket(port, requestHandler);
        Server.Start();

        // Have a way to stop this test program.
        var running = true;
        Console.CancelKeyPress += (_, _) => running = false;

        // This example program repeatedly iterates over the files in a given directory
        // and sends them.
        while (running) {
            var images = Directory.EnumerateFiles(imageDirectory);

            foreach (var image in images) {
                Console.WriteLine($"Trying to load image file \"{image}\".");
                Server.SendImage(image);
                Thread.Sleep(1000);
            }
        }

        Console.WriteLine("Stopping server.");
        Server.Stop();
    }
}
