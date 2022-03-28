using System;
using System.Runtime.InteropServices;

namespace Neurala {
    public class ResultOutput {
        [StructLayout(LayoutKind.Sequential)]
        private struct StructureForInvoke {
            public IntPtr Metadata;
            public IntPtr ImageData;
            public UInt64 Width;
            public UInt64 Height;
        }

        public static void Invoke(IntPtr argument, int size) {
            var structure = Marshal.PtrToStructure<StructureForInvoke>(argument);
            var metadata = Marshal.PtrToStringUTF8(structure.Metadata);

            /*
            var stream = new MemoryStream(structure.ImageData);
            var image = new Bitmap(stream);

            Console.WriteLine($"\n{metadata}\n\n{image.Width}x{image.Height} {image.PixelFormat}");
            */
        }
    }
}
