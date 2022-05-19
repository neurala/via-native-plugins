using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Runtime.InteropServices;
using System.Threading;

namespace Neurala {
    public static class VideoSource {
        private static Object BitmapLock;
        private static Object ResultLock;
        private static Bitmap CurrentBitmap;
        private static String Result;

        private static uint IngestedImageCount;
        private static uint AwaitedImageCount;
        private static uint ResultCount;
        private static uint StateFlag;

        static VideoSource() {
            BitmapLock = new object();
            ResultLock = new object();
        }

        public static string SendImage(Bitmap bitmap) {
            lock (BitmapLock) {
                CurrentBitmap = bitmap;
                IngestedImageCount++;
                Monitor.Pulse(BitmapLock);
            }

            lock (ResultLock) {
                var pulsed = Monitor.Wait(ResultLock, 10000);
                Console.WriteLine($"pulsed = {pulsed}");
                var result = Result ?? "null";
                ResultCount++;
                Result = null;
                return result;
            }
        }

        internal static void PushResult(string result) {
            lock (ResultLock) {
                Result = result;
                Monitor.Pulse(ResultLock);
            }
        }

        private static Bitmap WaitForImage() {
            lock (BitmapLock) {
                while (CurrentBitmap == null)
                    Monitor.Wait(BitmapLock);
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
            lock (BitmapLock) {
                CurrentBitmap = null;

                while (CurrentBitmap == null)
                    Monitor.Wait(BitmapLock);
            }

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
