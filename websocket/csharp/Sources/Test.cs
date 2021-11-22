using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;

using Neurala.VIA;

public static class Program {
    private static WebSocket Server;

    public static void Main(string[] arguments) {
        var portString = arguments[0];
        var port = Int32.Parse(portString);
        var imageDirectory = arguments[1];
        var imageProvider = IterateImageDirectoryForever(imageDirectory);

        Server = new WebSocket(port, imageProvider);
        Server.Start();

        Console.CancelKeyPress += (_, _) => Server.Stop();
    }

    private static IEnumerable<Bitmap> IterateImageDirectoryForever(string directory) {
        var files = Directory.EnumerateFiles(directory);

        while (true)
            foreach (var file in files)
                yield return new Bitmap(file);
    }
}
