using System;
using System.Runtime.InteropServices;

namespace Neurala {
    public static class VideoSource {
        public static void GetMetadata(out int width, out int height) {
            // Return (width, height) of current image.
            width = 1;
            height = 1;
        }

        public static void MoveNextFrame(out int status) {
            // Iterate to next image and return status.
            status = 0;
        }

        public static void GetFrame(IntPtr buffer) {
            // Get current image frame data and copy to buffer.
            var bytes = new byte[3] {0, 0, 0};
            Marshal.Copy(bytes, 0, buffer, bytes.Length);
        }

        public static void Execute(string action) {
            // Execute output action.
            Console.WriteLine($"Execute(\"{action}\")");
        }
    }
}
