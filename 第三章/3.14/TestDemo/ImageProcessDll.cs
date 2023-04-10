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
        private static extern int f_Softskin(byte* srcData, int width, int height, int stride, int skinMode, int ratio);
        [DllImport("TestDemo_C.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, ExactSpelling = true)]
        private static extern int f_SkinColor(byte* srcData, int width, int height, int stride, byte* lutData, int ratio);
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        public static Bitmap SoftSkin(Bitmap src, int skinMode, int ratio)
        {
            Bitmap dst = new Bitmap(src);
            BitmapData srcData = dst.LockBits(new Rectangle(0, 0, dst.Width, dst.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            f_Softskin((byte*)srcData.Scan0, dst.Width, dst.Height, srcData.Stride, skinMode, ratio);
            dst.UnlockBits(srcData);
            return dst;
        }
        public static Bitmap SkinColor(Bitmap src, Bitmap lut, int ratio)
        {
            Bitmap dst = new Bitmap(src);
            BitmapData srcData = dst.LockBits(new Rectangle(0, 0, dst.Width, dst.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            BitmapData lutData = lut.LockBits(new Rectangle(0, 0, lut.Width, lut.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            f_SkinColor((byte*)srcData.Scan0, dst.Width, dst.Height, srcData.Stride, (byte*)lutData.Scan0, ratio);
            dst.UnlockBits(srcData);
            lut.UnlockBits(lutData);
            return dst;
        }

        [DllImport("TestDemo_C.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, ExactSpelling = true)]
        private static extern int f_Softskin_ChannelMethod(byte* srcData, int width, int height, int stride, byte* lightMap, int ratio);
        public static Bitmap SoftSkin_ChannelMethod(Bitmap src, Bitmap curve, int ratio)
        {
            Bitmap dst = new Bitmap(src);
            BitmapData srcData = dst.LockBits(new Rectangle(0, 0, dst.Width, dst.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            BitmapData curveData = curve.LockBits(new Rectangle(0, 0, curve.Width, curve.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            f_Softskin_ChannelMethod((byte*)srcData.Scan0, dst.Width, dst.Height, srcData.Stride,(byte*)curveData.Scan0, ratio);
            dst.UnlockBits(srcData);
            curve.UnlockBits(curveData);
            return dst;
        }

        [DllImport("TestDemo_C.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, ExactSpelling = true)]
        private static extern int f_Softskin_HP(byte* srcData, int width, int height, int stride, int textureRatio, int ratio);
        public static Bitmap SoftSkin_HPMethod(Bitmap src, int textureRatio, int ratio)
        {
            Bitmap dst = new Bitmap(src);
            BitmapData srcData = dst.LockBits(new Rectangle(0, 0, dst.Width, dst.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            f_Softskin_HP((byte*)srcData.Scan0, dst.Width, dst.Height, srcData.Stride, textureRatio, ratio);
            dst.UnlockBits(srcData);
            return dst;
        }

        [DllImport("TestDemo_C.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, ExactSpelling = true)]
        private static extern int f_Softskin_MixMethod(byte* srcData, int width, int height, int stride, byte* lightMap, int textureRatio, int ratio);
        public static Bitmap SoftSkin_MixMethod(Bitmap src, Bitmap curve, int textureRatio, int ratio)
        {
            Bitmap dst = new Bitmap(src);
            BitmapData srcData = dst.LockBits(new Rectangle(0, 0, dst.Width, dst.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            BitmapData curveData = curve.LockBits(new Rectangle(0, 0, curve.Width, curve.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            f_Softskin_MixMethod((byte*)srcData.Scan0, dst.Width, dst.Height, srcData.Stride, (byte*)curveData.Scan0, textureRatio, ratio);
            dst.UnlockBits(srcData);
            curve.UnlockBits(curveData);
            return dst;
        }

        [DllImport("TestDemo_C.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, ExactSpelling = true)]
        private static extern int f_Softskin_DetailsAddingMethod(byte* srcData, int width, int height, int stride, int ratio, float K);
        public static Bitmap SoftSkin_DetailsAddingMethod(Bitmap src, int ratio, float K)
        {
            Bitmap dst = new Bitmap(src);
            BitmapData srcData = dst.LockBits(new Rectangle(0, 0, dst.Width, dst.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            f_Softskin_DetailsAddingMethod((byte*)srcData.Scan0, dst.Width, dst.Height, srcData.Stride, ratio, K);
            dst.UnlockBits(srcData);
            return dst;
        }
    }
}
