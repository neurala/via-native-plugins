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
                    yield return new Bitmap(imageFile);
                }
            }
        }
    }

    public static class VideoSource {
        private static IEnumerator<Bitmap> Source = new DummyVideoSource(@"C:\Images").Images.GetEnumerator();

        private static BitmapData CurrentBitmapData;

        static VideoSource() {
            Source.MoveNext();
        }

        public static void GetMetadata(out int width, out int height) {
            // Return (width, height) of current image.
            var image = Source.Current;

            width = image.Width;
            height = image.Height;

            Console.WriteLine(image.PixelFormat);
        }

        public static void MoveNextFrame(out int status) {
            // Iterate to next image and return status.
            if (CurrentBitmapData != null) {
                Source.Current.UnlockBits(CurrentBitmapData);
                CurrentBitmapData = null;
            }

            status = Source.MoveNext() ? 0 : 1;
        }

        public static IntPtr GetFrame() {
            // Get current image frame data and copy to buffer.
            var image = Source.Current;

            CurrentBitmapData = image.LockBits(new Rectangle(0, 0, image.Width, image.Height),
                                               ImageLockMode.ReadOnly,
                                               image.PixelFormat);
            return CurrentBitmapData.Scan0;
        }

        public static void Execute(string action) {
            // Execute output action.
            Console.WriteLine($"Execute(\"{action}\")");
        }
    }
}
