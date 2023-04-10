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
        private static extern int f_FindEdges(byte *srcData, int width, int height,int stride);
        [DllImport("TestDemo_C.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.None, ExactSpelling = true)]
        private static extern int f_Sobel(byte *srcData, int width, int height,int stride, int threshold);
        public Bitmap SobelEdgedetection(Bitmap src, int threshold)
         {
             Bitmap a = new Bitmap(src);
             int w = a.Width;
             int h = a.Height;
             BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
             f_Sobel((byte*)srcData.Scan0, w, h, srcData.Stride, threshold);
             a.UnlockBits(srcData);
             return a;
         }
        public Bitmap FindEdgedes(Bitmap src)
        {
            Bitmap a = new Bitmap(src);
            int w = a.Width;
            int h = a.Height;
            BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            f_FindEdges((byte*)srcData.Scan0, w, h, srcData.Stride);
            a.UnlockBits(srcData);
            return a;
        }

    }
}
