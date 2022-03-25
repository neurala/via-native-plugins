using System;
using System.Runtime.InteropServices;

namespace Neurala {
    public class VideoSource {
        [StructLayout(LayoutKind.Sequential)]
        private struct StructureForGetMetadata {
            public UInt64 Width;
            public UInt64 Height;
        }

        [StructLayout(LayoutKind.Sequential)]
        private struct StructureForMoveNextFrame {
            public Int32 Status;
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
            var structure = new StructureForGetMetadata { Width = 1, Height = 1 };

            Marshal.StructureToPtr<StructureForGetMetadata>(structure, result, false);
        }

        public static void MoveNextFrame(IntPtr result, int size) {
            var structure = new StructureForMoveNextFrame { Status = 0 };

            Marshal.StructureToPtr<StructureForMoveNextFrame>(structure, result, false);
        }

        public static void GetFrame(IntPtr result, int size) {
            var structure = new StructureForGetFrame { Data = new IntPtr(0) };

            Marshal.StructureToPtr<StructureForGetFrame>(structure, result, false);
        }

        public static void Execute(IntPtr argument, int size) {
            var structure = Marshal.PtrToStructure<StructureForExecute>(argument);
            var action = Marshal.PtrToStringUTF8(structure.Action);

            Console.WriteLine($"Execute(\"{action}\")");
        }
    }
}
