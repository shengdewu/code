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
        private static extern int f_MeanFilter(byte *srcData, int width, int height, int stride, int radius);
        [DllImport("TestDemo_C.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.None, ExactSpelling = true)]
        private static extern int f_FastMeanFilter(byte* srcData, int width, int height, int stride, int radius);
        public Bitmap MeanFilter(Bitmap src, int saturation)
         {
             Bitmap a = new Bitmap(src);
             int w = a.Width;
             int h = a.Height;
             BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
             f_MeanFilter((byte*)srcData.Scan0, w, h, srcData.Stride, saturation);
             a.UnlockBits(srcData);
             return a;
         }
        public Bitmap FastMeanFilter(Bitmap src, int saturation)
        {
            Bitmap a = new Bitmap(src);
            int w = a.Width;
            int h = a.Height;
            BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            f_FastMeanFilter((byte*)srcData.Scan0, w, h, srcData.Stride, saturation);
            a.UnlockBits(srcData);
            return a;
        }

    }
}
