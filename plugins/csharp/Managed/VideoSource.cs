using System;
using System.Runtime.InteropServices;

namespace Neurala {
    public static class NativeVideoSource {
        [StructLayout(LayoutKind.Sequential)]
        private struct StructureForGetMetadata {
            public int Width;
            public int Height;
        }

        [StructLayout(LayoutKind.Sequential)]
        private struct StructureForMoveNextFrame {
            public int Status;
        }

        [StructLayout(LayoutKind.Sequential)]
        private struct StructureForGetFrame {
            public IntPtr Data;
        }

        [StructLayout(LayoutKind.Sequential)]
        private struct StructureForExecute {
            public IntPtr Action;
        }

        public static void GetMetadata(IntPtr result, int size) {
            var (width, height) = GetMetadata();

            var structure = new StructureForGetMetadata { Width = width, Height = height };

            Marshal.StructureToPtr<StructureForGetMetadata>(structure, result, false);
        }

        public static void MoveNextFrame(IntPtr result, int size) {
            var structure = new StructureForMoveNextFrame { Status = MoveNextFrame() };

            Marshal.StructureToPtr<StructureForMoveNextFrame>(structure, result, false);
        }

        public static void GetFrame(IntPtr argument, int size) {
            var (width, height) = GetMetadata();

            var structure = Marshal.PtrToStructure<StructureForGetFrame>(argument);

            Marshal.Copy(GetFrame(), 0, structure.Data, width * height);
        }

        public static void Execute(IntPtr argument, int size) {
            var structure = Marshal.PtrToStructure<StructureForExecute>(argument);
            var action = Marshal.PtrToStringUTF8(structure.Action);

            Execute(action);
        }

        public static (int, int) GetMetadata() {
            // Return (width, height) of current image.
            return (1, 1);
        }

        public static int MoveNextFrame() {
            // Iterate to next image and return status.
            return 0;
        }

        public static byte[] GetFrame() {
            // Get current frame data.
            return new byte[3] {0, 0, 0};
        }

        public static void Execute(string action) {
            // Execute output action.
            Console.WriteLine($"Execute(\"{action}\")");
        }
    }
}
