using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;

namespace TestDemo
{
    unsafe class ImageProcessDll
    {
        [DllImport("TestDemo_C.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, ExactSpelling = true)]
        private static extern void f_SurfaceBlur(byte* srcData, int nWidth, int nHeight, int nStride, int radius, int threshold);

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        public static Bitmap SurfaceBlur(Bitmap src, int radius, int threshold)
        {
            Bitmap dst = new Bitmap(src);
            BitmapData srcData = dst.LockBits(new Rectangle(0, 0, dst.Width, dst.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            f_SurfaceBlur((byte*)srcData.Scan0, dst.Width, dst.Height, srcData.Stride, radius, threshold);
            dst.UnlockBits(srcData);
            return dst;
        }
       
    }
}
