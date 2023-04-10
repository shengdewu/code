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
        private static extern int f_FaceLift(byte* srcData, int width, int height, int stride, int centerX, int centerY, int rmax, int mx, int my, int strength);

        public static Bitmap FaceLift(Bitmap src, int centerX, int centerY, int rmax, int mx, int my, int strength)
        {
            Bitmap a = new Bitmap(src);          
            BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            f_FaceLift((byte*)srcData.Scan0, a.Width, a.Height, srcData.Stride, centerX, centerY, rmax, mx, my, strength);
            a.UnlockBits(srcData);
            return a;
        }

        
    }
}
