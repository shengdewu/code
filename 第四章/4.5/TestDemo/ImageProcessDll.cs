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
        private static extern int f_MLSImagewarpping(byte* srcData, int width ,int height, int stride, int []srcPoint, int []dragPoint, int pointNum, int gridSize, int method);


        public static Bitmap MLS(Bitmap src, int[] srcPoint, int[] dragPoint, int gridSize, int method)
        {
            Bitmap a = new Bitmap(src);
            
            BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            f_MLSImagewarpping((byte*)srcData.Scan0, a.Width, a.Height, srcData.Stride, srcPoint, dragPoint, srcPoint.Length / 2, gridSize, method);
            a.UnlockBits(srcData);
            return a;
        }

        
    }
}
