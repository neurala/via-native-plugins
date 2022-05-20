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

        private static bool WaitingForResult;

        static VideoSource() {
            Lock = new object();
        }

        public static string SendImage(Bitmap bitmap) {
            lock (Lock) {
                PendingBitmap = bitmap;
                WaitingForResult = true;
                Console.WriteLine($"THREAD {Thread.CurrentThread.ManagedThreadId} SENT IMAGE");
                Monitor.PulseAll(Lock);

                Console.WriteLine($"THREAD {Thread.CurrentThread.ManagedThreadId} AWAITING RESULT");
                while (Result == null)
                    Monitor.Wait(Lock);
                Console.WriteLine($"THREAD {Thread.CurrentThread.ManagedThreadId} GOT RESULT");
                var result = Result;
                Result = null;
                WaitingForResult = false;
                return result;
            }
        }

        internal static void PushResult(string result) {
            lock (Lock) {
                Result = result;
                Console.WriteLine($"THREAD {Thread.CurrentThread.ManagedThreadId} PUSHING RESULT");
                Monitor.PulseAll(Lock);
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
                Console.WriteLine($"THREAD {Thread.CurrentThread.ManagedThreadId} ENTERING MoveNextFrame");
                while (PendingBitmap == null)
                    Monitor.Wait(Lock);
                CurrentBitmap = PendingBitmap;
                PendingBitmap = null;
                Console.WriteLine($"THREAD {Thread.CurrentThread.ManagedThreadId} EXITING MoveNextFrame");
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
