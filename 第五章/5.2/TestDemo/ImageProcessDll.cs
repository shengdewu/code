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
        private static extern int f_FaceBlush(byte* srcData, int width, int height, int stride, int[] srcFacePoints, byte* mskData, int mWidth, int mHeight, int mStride, int []maskKeyPoints, bool isLeft,  int ratio);

        public static Bitmap FaceBlush(Bitmap src, int[] srcPoints, Bitmap mask, int[] maskPoints, bool isLeft, int ratio)
        {
            Bitmap a = new Bitmap(src);
            BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            BitmapData maskData = mask.LockBits(new Rectangle(0, 0, mask.Width, mask.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            f_FaceBlush((byte*)srcData.Scan0, a.Width, a.Height, srcData.Stride, srcPoints, (byte*)maskData.Scan0, mask.Width, mask.Height, maskData.Stride, maskPoints, isLeft, ratio);
            a.UnlockBits(srcData);
            mask.UnlockBits(maskData);
            return a;
        }

        
    }
}
