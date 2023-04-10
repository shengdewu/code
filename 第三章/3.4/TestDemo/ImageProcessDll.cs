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
        private static extern int f_LSNFilter(byte* srcData, int nWidth, int nHeight, int nStride, int radius, int delta2);

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        public static Bitmap LSNFilter(Bitmap src, int radius, int delta2)
        {
            Bitmap dst = new Bitmap(src);
            BitmapData srcData = dst.LockBits(new Rectangle(0, 0, dst.Width, dst.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            f_LSNFilter((byte*)srcData.Scan0, dst.Width, dst.Height, srcData.Stride, radius, delta2);
            dst.UnlockBits(srcData);
            return dst;
        }
       
    }
}
