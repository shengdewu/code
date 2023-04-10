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
        private static extern int f_ConcaveMirrorFilter(byte* srcData, int width, int height, int stride, int x, int y, int k);
        [DllImport("TestDemo_C.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.None, ExactSpelling = true)]
        private static extern int f_ConvexMirrorFilter(byte* srcData, int width, int height, int stride, int x, int y, int k);
        public Bitmap HahaFilter(Bitmap src, int x, int y, int k, int mode)
         {
             Bitmap a = new Bitmap(src);
             int w = a.Width;
             int h = a.Height;
             BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
             if (mode == 0)
                f_ConvexMirrorFilter((byte*)srcData.Scan0, w, h, srcData.Stride, x, y, k);
             else
                f_ConcaveMirrorFilter((byte*)srcData.Scan0, w, h, srcData.Stride, x, y, k);
             a.UnlockBits(srcData);
             return a;
         }

    }
}
