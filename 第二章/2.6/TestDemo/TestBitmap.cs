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
        private static extern int f_LUTFilter(byte *srcData, int width, int height, int stride, byte* Map);
        public Bitmap ColorFilter(Bitmap src, Bitmap lut)
         {
             Bitmap a = new Bitmap(src);
             int w = a.Width;
             int h = a.Height;
             BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
             BitmapData lutData = lut.LockBits(new Rectangle(0, 0, lut.Width, lut.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
             f_LUTFilter((byte*)srcData.Scan0, w, h, srcData.Stride, (byte*)lutData.Scan0);
             a.UnlockBits(srcData);
             lut.UnlockBits(lutData);
             return a;
         }

    }
}
