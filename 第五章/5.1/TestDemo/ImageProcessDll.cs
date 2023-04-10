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
        private static extern int f_BeautyIris(byte* srcData, int width, int height, int stride, int []srcPoints, byte* maskData, int mWidth, int mHeight, int mStride, int []maskPoints, int ratio);

        public static Bitmap BeautyIris(Bitmap src, int[] srcPoints, Bitmap mask, int[] maskPoints, int ratio)
        {
            Bitmap a = new Bitmap(src);
            BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            BitmapData maskData = mask.LockBits(new Rectangle(0, 0, mask.Width, mask.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            f_BeautyIris((byte*)srcData.Scan0, a.Width, a.Height, srcData.Stride, srcPoints, (byte*)maskData.Scan0, mask.Width, mask.Height, maskData.Stride, maskPoints, ratio);
            a.UnlockBits(srcData);
            mask.UnlockBits(maskData);
            return a;
        }

        
    }
}
