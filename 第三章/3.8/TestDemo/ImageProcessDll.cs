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
        private static extern int f_BeepsFilter(byte* srcData, int nWidth, int nHeight, int nStride, float delta, float delta_s);


        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        public static Bitmap BeepsFilter(Bitmap src, float delta, float delta_s)
        {
            Bitmap dst = new Bitmap(src);
            BitmapData srcData = dst.LockBits(new Rectangle(0, 0, dst.Width, dst.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            f_BeepsFilter((byte*)srcData.Scan0, dst.Width, dst.Height, srcData.Stride, delta, delta_s);
            dst.UnlockBits(srcData);
            return dst;
        }
       
    }
}
