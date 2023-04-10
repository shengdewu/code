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
        private static extern int f_Instagram1977LutFilter(byte* srcData, int width, int height, int stride, byte* Map);

        [DllImport("TestDemo_C.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.None, ExactSpelling = true)]
        private static extern int f_Instagram1977Filter(byte* srcData, int width, int height, int stride);
        public Bitmap Instagram1977LutFilter(Bitmap src, Bitmap lut)
         {
             Bitmap a = new Bitmap(src);
             int w = a.Width;
             int h = a.Height;
             BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
             BitmapData lutData = lut.LockBits(new Rectangle(0, 0, lut.Width, lut.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
             f_Instagram1977LutFilter((byte*)srcData.Scan0, w, h, srcData.Stride, (byte*)lutData.Scan0);
             a.UnlockBits(srcData);
             lut.UnlockBits(lutData);
             return a;
         }
        public Bitmap Instagram1977Filter(Bitmap src)
        {
            Bitmap a = new Bitmap(src);
            int w = a.Width;
            int h = a.Height;
            BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            f_Instagram1977Filter((byte*)srcData.Scan0, w, h, srcData.Stride);
            a.UnlockBits(srcData);
            return a;
        }
    }
}
