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
        private static extern int f_IDW(byte* srcData, int width, int height, int stride, int[] inputPoints, int []outputPoints, int pointNum);

        public static Bitmap IDW(Bitmap src, int[] inputPoints, int[] outputPoints)
        {
            Bitmap a = new Bitmap(src);
            
            BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            f_IDW((byte*)srcData.Scan0, a.Width, a.Height, srcData.Stride, inputPoints, outputPoints, inputPoints.Length / 2);
            a.UnlockBits(srcData);
            return a;
        }

        
    }
}
