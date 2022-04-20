using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;

namespace Neurala {
    public static class Program {
        public static void Main() {
            var imageFiles = Directory.EnumerateFiles(@"C:\Images");

            while (true) {
                foreach (var imageFile in imageFiles) {
                    Console.WriteLine(imageFile);

                    var bitmap = new Bitmap(imageFile);
                    var result = VideoSource.SendImage(bitmap);
                }
            }
        }
    }
}
