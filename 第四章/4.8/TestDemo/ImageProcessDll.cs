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
        private static extern int f_AutoBigeye(byte* srcData, int width, int height, int stride, int[] facePoints);
        [DllImport("TestDemo_C.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, ExactSpelling = true)]
        private static extern int f_BigEye(byte* srcData, int width, int height, int stride, int cenX, int cenY, int radius, int intensity);
        public static Bitmap AutoBigEye(Bitmap src, int[] facePoints)
        {
            Bitmap a = new Bitmap(src);
            BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            f_AutoBigeye((byte*)srcData.Scan0, a.Width, a.Height, srcData.Stride, facePoints);
            a.UnlockBits(srcData);
            return a;
        }
        public static Bitmap BigEye(Bitmap src, int cenX, int cenY, int radius, int intensity)
        {
            Bitmap a = new Bitmap(src);
            BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            f_BigEye((byte*)srcData.Scan0, a.Width, a.Height, srcData.Stride, cenX, cenY, radius, intensity);
            a.UnlockBits(srcData);
            return a;
        }
        
    }
}
