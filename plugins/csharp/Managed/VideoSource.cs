using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Runtime.InteropServices;
using System.Threading;

namespace Neurala {
    public static class VideoSource {
        private static Object Lock;
        private static Bitmap CurrentBitmap;
        private static String Result;

        static VideoSource() {
            Lock = typeof(VideoSource);
        }

        public static string SendImage(Bitmap bitmap) {
            lock (Lock) {
                while (CurrentBitmap != null)
                    Monitor.Wait(Lock);

                CurrentBitmap = bitmap;
                Monitor.Pulse(Lock);

                while (Result == null)
                    Monitor.Wait(Lock);

                var result = Result;
                CurrentBitmap = null;
                Result = null;
                return result;
            }
        }

        internal static void PushResult(string result) {
            lock (Lock) {
                Result = result;

                Monitor.Pulse(Lock);
            }
        }

        private static Bitmap WaitForImage() {
            lock (Lock) {
                while (CurrentBitmap == null)
                    Monitor.Wait(Lock);
                return CurrentBitmap;
            }
        }

        // Return (width, height) of current image.
        public static void GetMetadata(out int width, out int height) {
            var image = WaitForImage();

            width = image.Width;
            height = image.Height;
        }

        public static void MoveNextFrame(out int status) {
            // Do nothing.
            status = 0;
        }

        // Get current image frame data and copy to buffer.
        public static void GetFrame(IntPtr buffer) {
            var image = WaitForImage();
            var bits = image.LockBits(new Rectangle(0, 0, image.Width, image.Height),
                                      ImageLockMode.ReadOnly,
                                      image.PixelFormat);

            CopyMemory(buffer, bits.Scan0, image.Width * image.Height * 3);

            image.UnlockBits(bits);
        }

        // Execute output action.
        public static void Execute(string action) {
            Console.WriteLine($"Execute(\"{action}\")");
        }

        [DllImport("kernel32.dll")]
        private static extern void CopyMemory(IntPtr destination, IntPtr source, int length);
    }
}
