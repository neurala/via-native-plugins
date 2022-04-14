using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Runtime.InteropServices;

namespace Neurala {
    internal class DummyVideoSource {
        private IEnumerable<string> ImageFiles;

        public IEnumerable<Bitmap> Images {
            get => EnumerateImages();
        }

        public DummyVideoSource(string directory) {
            ImageFiles = Directory.EnumerateFiles(directory);
        }

        public IEnumerable<Bitmap> EnumerateImages() {
            while (true) {
                foreach (var imageFile in ImageFiles) {
                    Console.WriteLine($"  Yielding file {imageFile}.");
                    yield return new Bitmap(imageFile);
                }
            }
        }
    }

    public static class VideoSource {
        private static IEnumerator<Bitmap> Source = new DummyVideoSource(@"C:\Images").Images.GetEnumerator();

        private static BitmapData CurrentBitmapData;

        // Return (width, height) of current image.
        public static void GetMetadata(out int width, out int height) {
            var image = Source.Current;

            width = image.Width;
            height = image.Height;

            Console.WriteLine($"Got image metadata {width}x{height} {image.PixelFormat}.");
        }

        // Iterate to next image and return status.
        public static void MoveNextFrame(out int status) {
            Console.Write("Moving next frame...");

            status = Source.MoveNext() ? 0 : 1;

            Console.WriteLine($"with status {status}.");
        }

        // Get current image frame data and copy to buffer.
        public static void GetFrame(IntPtr buffer) {
            var image = Source.Current;

            Console.WriteLine("Locking new bitmap data.");

            CurrentBitmapData = image.LockBits(new Rectangle(0, 0, image.Width, image.Height),
                                               ImageLockMode.ReadOnly,
                                               image.PixelFormat);

            Console.WriteLine("BEGIN SANITY CHECK");
            Console.WriteLine("TOUCHING BEGINNING OF SOURCE");
            Marshal.ReadByte(CurrentBitmapData.Scan0);
            Console.WriteLine("TOUCHING END OF SOURCE");
            Marshal.ReadByte(CurrentBitmapData.Scan0, image.Width * image.Height * 3 - 1);
            Console.WriteLine("TOUCHING BEGINNING OF DESTINATION");
            Marshal.ReadByte(buffer);
            Console.WriteLine("TOUCHING END OF DESTINATION");
            Marshal.ReadByte(buffer, image.Width * image.Height * 3 - 1);
            Console.WriteLine("COPYING MEMORY");

            CopyMemory(buffer, CurrentBitmapData.Scan0, image.Width * image.Height * 3);

            image.UnlockBits(CurrentBitmapData);

            Console.WriteLine("END SANITY CHECK");
        }

        // Execute output action.
        public static void Execute(string action) {
            Console.WriteLine($"Execute(\"{action}\")");
        }

        [DllImport("kernel32.dll")]
        private static extern void CopyMemory(IntPtr destination, IntPtr source, int length);
    }
}
