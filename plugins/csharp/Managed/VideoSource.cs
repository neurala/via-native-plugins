using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Runtime.InteropServices;

namespace Neurala {
    public static class VideoSource {
        private static FrameGrabber Source = new FrameGrabber();
        private static Bitmap CurrentBitmap;

        // Return (width, height) of current image.
        public static void GetMetadata(out int width, out int height) {
            width = CurrentBitmap.Width;
            height = CurrentBitmap.Height;

            Console.WriteLine($"Got image metadata {width}x{height} {CurrentBitmap.PixelFormat}.");
        }

        // Iterate to next image and return status.
        public static void MoveNextFrame(out int status) {
            Console.Write("Moving next frame...");

            CurrentBitmap = Source.GrabFrame();
            status = 0;
        }

        // Get current image frame data and copy to buffer.
        public static void GetFrame(IntPtr buffer) {
            Console.WriteLine("Locking new bitmap data.");

            var bits = CurrentBitmap.LockBits(new Rectangle(0, 0, CurrentBitmap.Width, CurrentBitmap.Height),
                                              ImageLockMode.ReadOnly,
                                              CurrentBitmap.PixelFormat);

            CopyMemory(buffer, bits.Scan0, CurrentBitmap.Width * CurrentBitmap.Height * 3);

            CurrentBitmap.UnlockBits(bits);
        }

        // Execute output action.
        public static void Execute(string action) {
            Console.WriteLine($"Execute(\"{action}\")");
        }

        [DllImport("kernel32.dll")]
        private static extern void CopyMemory(IntPtr destination, IntPtr source, int length);
    }
}
