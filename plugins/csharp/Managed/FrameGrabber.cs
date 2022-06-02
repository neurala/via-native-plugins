using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;

namespace Neurala {
    // Please replace this sample implementation with your own.
    // A template is provided in the README.
    public class FrameGrabber {
        private IEnumerator<Bitmap> Frames;

        public FrameGrabber() {
            Frames = EnumerateImages().GetEnumerator();
        }

        public Bitmap GrabFrame() {
            Frames.MoveNext();

            return Frames.Current;
        }

        public IEnumerable<Bitmap> EnumerateImages() {
            var imageFiles = Directory.EnumerateFiles(@"C:\Images");

            while (true)
                foreach (var imageFile in imageFiles)
                    yield return new Bitmap(imageFile);
        }
    }
}
