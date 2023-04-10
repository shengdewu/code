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
        
    }
}
