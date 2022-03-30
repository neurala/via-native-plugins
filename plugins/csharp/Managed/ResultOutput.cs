using System;
using System.Runtime.InteropServices;

namespace Neurala {
    public static class ResultOutput {
        public static void Invoke(string metadata, IntPtr imageData, int width, int height) {
            Console.WriteLine($"Output with {width}x{height} image.");
        }
    }
}
