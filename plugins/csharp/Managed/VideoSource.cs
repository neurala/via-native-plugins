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
        private static Bitmap PendingBitmap;
        private static Bitmap CurrentBitmap;
        private static String Result;

        private static uint IngestedImageCount;
        private static uint AwaitedImageCount;
        private static uint ResultCount;
        private static uint StateFlag;
        private static bool WaitingForResult;

        static VideoSource() {
            Lock = new object();
        }

        public static string SendImage(Bitmap bitmap) {
            lock (Lock) {
                PendingBitmap = bitmap;
                IngestedImageCount++;
                WaitingForResult = true;
                Monitor.Pulse(Lock);

                while (Result == null)
                    Monitor.Wait(Lock);
                var result = Result;
                ResultCount++;
                Result = null;
                WaitingForResult = false;
                return result;
            }
        }

        internal static void PushResult(string result) {
            lock (Lock) {
                Result = result;
                Monitor.Pulse(Lock);
            }
        }

        // Return (width, height) of current image.
        public static void GetMetadata(out int width, out int height) {
            var image = CurrentBitmap;

            width = image.Width;
            height = image.Height;
        }

        public static void MoveNextFrame(out int status) {
            lock (Lock) {
                if (WaitingForResult && PendingBitmap == null)
                    PushResult("NULL");
                while (PendingBitmap == null)
                    Monitor.Wait(Lock);
                CurrentBitmap = PendingBitmap;
                PendingBitmap = null;
                AwaitedImageCount++;
                Console.WriteLine($"{AwaitedImageCount}, {IngestedImageCount}");
            }

            status = 0;
        }

        // Get current image frame data and copy to buffer.
        public static void GetFrame(IntPtr buffer) {
            var image = CurrentBitmap;
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
