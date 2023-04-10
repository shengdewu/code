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
        private static extern int f_USM(byte* srcData,int width, int height,int stride,int radius, int amount, int threshold);
        public Bitmap USMFilter(Bitmap src, int radius, int amount, int threshold)
         {
             Bitmap a = new Bitmap(src);
             int w = a.Width;
             int h = a.Height;
             BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
             f_USM((byte*)srcData.Scan0, w, h, srcData.Stride, radius, amount, threshold);
             a.UnlockBits(srcData);
             return a;
         }


    }
}
