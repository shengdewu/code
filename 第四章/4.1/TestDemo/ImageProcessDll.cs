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
        private static extern void f_AffinetransformMetrixCompute(float x1, float y1, float x2, float y2, float x3, float y3, float tx1, float ty1, float tx2, float ty2, float tx3, float ty3, float[]hMatrix);
        [DllImport("TestDemo_C.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, ExactSpelling = true)]
        private static extern void getWHFromHMatrix(int width, int height, float[] H, int[] wh);
        [DllImport("TestDemo_C.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, ExactSpelling = true)]
        private static extern int f_AffineTransform(byte* srcData, int width, int height, int stride, float []H, byte* dstData, int dWidth, int dHeight, int dStride);
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        public static Bitmap AffineTransform(Bitmap src, float[] H)
        {
            Bitmap a = new Bitmap(src);
            int []wh = new int[2];
            getWHFromHMatrix(a.Width, a.Height, H, wh);
            Bitmap dst = new Bitmap(wh[0], wh[1], a.PixelFormat);
            BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            BitmapData dstData = dst.LockBits(new Rectangle(0, 0, dst.Width, dst.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            f_AffineTransform((byte*)srcData.Scan0, a.Width, a.Height, srcData.Stride, H, (byte*)dstData.Scan0, dst.Width, dst.Height, dstData.Stride);
            a.UnlockBits(srcData);
            dst.UnlockBits(dstData);
            return dst;
        }
        public static void GetAffineTransformHMatrix(float x1, float y1, float x2, float y2, float x3, float y3, float tx1, float ty1, float tx2, float ty2, float tx3, float ty3, float[] hMatrix)
        {
            f_AffinetransformMetrixCompute(x1, y1, x2, y2, x3, y3, tx1, ty1, tx2, ty2, tx3, ty3, hMatrix);
        }
        
    }
}
