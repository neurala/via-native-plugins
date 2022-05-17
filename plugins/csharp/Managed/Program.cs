using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;

namespace Neurala {
    public static class Program {
        public static void Main() {
            // Get the images in the "C:\Images" directory.
            var imageFiles = Directory.EnumerateFiles(@"C:\Images");

            // Run forever.
            while (true) {
                // For each file we found in the directory...
                foreach (var imageFile in imageFiles) {
                    // Say what file is being loaded.
                    Console.WriteLine($"Loading {imageFile} for processing.");

                    // Load the bitmap.
                    var bitmap = new Bitmap(imageFile);

                    // Send the image to SDKService and get back a result.
                    // If a result doesn't come back (e.g. if running in
                    // Live View mode), this will return an empty string.
                    var result = VideoSource.SendImage(bitmap);

                    // Print the result (if there is any).
                    Console.WriteLine($"Got result for image {imageFile}: {result}");
                }
            }
        }
    }
}
