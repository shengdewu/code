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
        private static extern int f_SkindetectionRGB(byte* srcData, int width, int height, int stride);
        [DllImport("TestDemo_C.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, ExactSpelling = true)]
        private static extern int f_SkindetectionHSV(byte* srcData, int width, int height, int stride);
        [DllImport("TestDemo_C.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, ExactSpelling = true)]
        private static extern int f_SkindetectionYCgCr(byte* srcData, int width, int height, int stride);
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        public static Bitmap SkinDetectionRGB(Bitmap src)
        {
            Bitmap dst = new Bitmap(src);
            BitmapData srcData = dst.LockBits(new Rectangle(0, 0, dst.Width, dst.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            f_SkindetectionRGB((byte*)srcData.Scan0, dst.Width, dst.Height, srcData.Stride);
            dst.UnlockBits(srcData);
            return dst;
        }
        public static Bitmap SkinDetectionHSV(Bitmap src)
        {
            Bitmap dst = new Bitmap(src);
            BitmapData srcData = dst.LockBits(new Rectangle(0, 0, dst.Width, dst.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            f_SkindetectionHSV((byte*)srcData.Scan0, dst.Width, dst.Height, srcData.Stride);
            dst.UnlockBits(srcData);
            return dst;
        }
        public static Bitmap SkinDetectionYCgCr(Bitmap src)
        {
            Bitmap dst = new Bitmap(src);
            BitmapData srcData = dst.LockBits(new Rectangle(0, 0, dst.Width, dst.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            f_SkindetectionYCgCr((byte*)srcData.Scan0, dst.Width, dst.Height, srcData.Stride);
            dst.UnlockBits(srcData);
            return dst;
        }
    }
}
