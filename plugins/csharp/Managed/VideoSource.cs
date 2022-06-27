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
        private static DateTime LastRequestTime;

        private static bool WaitingForResult;

        public static int Timeout { get; set; }

        static VideoSource() {
            Lock = new object();
            LastRequestTime = DateTime.Now;
            Timeout = 1000;
        }

        public static string SendImage(Bitmap bitmap) {
            lock (Lock) {
                PendingBitmap = bitmap;
                Console.WriteLine($"THREAD {Thread.CurrentThread.ManagedThreadId} SENT IMAGE");
                Monitor.PulseAll(Lock);

                Console.WriteLine($"THREAD {Thread.CurrentThread.ManagedThreadId} AWAITING RESULT");
                LastRequestTime = DateTime.Now;
                WaitingForResult = true;
                while (Result == null)
                    Monitor.Wait(Lock);
                WaitingForResult = false;
                Console.WriteLine($"THREAD {Thread.CurrentThread.ManagedThreadId} GOT RESULT");
                var result = Result;
                Result = null;
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

            if (image != null) {
                Console.WriteLine($"THREAD {Thread.CurrentThread.ManagedThreadId} GETTING METADATA");

                width = image.Width;
                height = image.Height;
            } else {
                Console.WriteLine($"THREAD {Thread.CurrentThread.ManagedThreadId} CAN'T GET METADATA");

                width = 0;
                height = 0;
            }
        }

        public static void MoveNextFrame(out int status) {
            lock (Lock) {
                Console.WriteLine($"THREAD {Thread.CurrentThread.ManagedThreadId} ENTERING MoveNextFrame");

                // Wait for a frame to come in.
                if (PendingBitmap == null)
                    Monitor.Wait(Lock, Timeout);

                // If nothing came in after the timeout...
                if (PendingBitmap == null) {
                    // ...and if the user has been waiting for longer than normal...
                    if (WaitingForResult && (DateTime.Now - LastRequestTime).TotalMilliseconds > Timeout * 3) {
                        // ...then just use the current frame, because the pipeline must have been stopped at some
                        // point and restarted.
                        Console.WriteLine($"  Using last frame, as the pipeline may have been stopped and restarted.");
                        PendingBitmap = CurrentBitmap;
                    } else {
                        // Otherwise, just get out of here.
                        Console.WriteLine($"THREAD {Thread.CurrentThread.ManagedThreadId} TIMED OUT in MoveNextFrame");
                        status = 1;
                        return;
                    }
                }

                CurrentBitmap = PendingBitmap;
                PendingBitmap = null;
                Console.WriteLine($"THREAD {Thread.CurrentThread.ManagedThreadId} EXITING MoveNextFrame");
                status = 0;
            }
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
