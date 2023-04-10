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
        private static extern int f_PerspectiveTransform(byte* srcData, int width, int height, int stride, float[]H );
        
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        public static Bitmap PerspectiveTransform(Bitmap src, float[] H)
        {
            Bitmap a = new Bitmap(src);
            BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            f_PerspectiveTransform((byte*)srcData.Scan0, a.Width, a.Height, srcData.Stride, H);
            a.UnlockBits(srcData);
            return a;
        }
        
        
    }
}
