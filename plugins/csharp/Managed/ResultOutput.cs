using System;
using System.Runtime.InteropServices;

namespace Neurala {
    public static class ResultOutput {
        public static void Invoke(string metadata, IntPtr imageData, int width, int height) {
            VideoSource.PushResult(metadata);
        }
    }
}
