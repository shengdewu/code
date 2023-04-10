using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;

namespace TestDemo
{
    unsafe class TestBitmap
    {

        [DllImport("TestDemo_C.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.None, ExactSpelling = true)]
        private static extern int f_getPixel(byte *srcData, int width, int height, int stride, int x, int y, int[] rgb);
         public Bitmap GetImgPixel(Bitmap src, int x, int y, ref int[] rgba)
         {
             Bitmap a = new Bitmap(src);
             int w = a.Width;
             int h = a.Height;
             BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
             f_getPixel((byte*)srcData.Scan0, w, h, srcData.Stride, x, y, rgba);
             a.UnlockBits(srcData);
             GC.Collect();
             return a;
         }

    }
}
