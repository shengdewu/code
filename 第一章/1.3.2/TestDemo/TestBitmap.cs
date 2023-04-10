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
        private static extern int f_HSVAdjust(byte* srcData, int width, int height, int stride, float hIntensity, float sIntensity, float vIntensity);
        public Bitmap HSVAdjust(Bitmap src, float hIntensity, float sIntensity, float vIntensity)
         {
             Bitmap a = new Bitmap(src);
             int w = a.Width;
             int h = a.Height;

             BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
             f_HSVAdjust((byte*)srcData.Scan0, w, h, srcData.Stride, hIntensity, sIntensity, vIntensity);
             a.UnlockBits(srcData);
             return a;
         }

    }
}
