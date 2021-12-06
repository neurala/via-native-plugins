using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Threading;

using Neurala.VIA;

public static class Program {
    public static void Main(string[] arguments) {
        // Get port, source image directory, and destination image directory (for processed images).
        var portString = arguments[0];
        var port = Int32.Parse(portString);
        var imageDirectory = arguments[1];
        var processedDirectory = arguments[2];

        // Create and start the WebSocket.
        var server = new WebSocket(port, HandleAction);
        server.Start();

        // Have a way to stop this test program.
        var running = true;
        Console.CancelKeyPress += (_, _) => running = false;

        // This example program repeatedly iterates over the files in a given directory,
        // sends them, and then moves them to a different directory to mark as processed.
        while (running) {
            var images = Directory.EnumerateFiles(imageDirectory);

            foreach (var image in images) {
                Console.WriteLine($"Trying to load image file \"{image}\".");

                var result = server.SendImage(image);

                Console.WriteLine(result);

                var fileName = Path.GetFileName(image);
                var destination = Path.Join(processedDirectory, fileName);

                File.Move(image, destination);
            }
        }

        Console.WriteLine("Stopping server.");
        server.Stop();
    }

    // What to do upon receiving an action request.
    private static void HandleAction(string action) {
        Console.WriteLine(action);
    }
}
