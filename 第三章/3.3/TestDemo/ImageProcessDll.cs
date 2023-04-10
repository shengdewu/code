﻿using System;
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
        private static extern  int f_GuidedFilter(byte* srcData, int width, int height, int stride, int radius, float delta);

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        public static Bitmap GuidedFilter(Bitmap src, int radius, float delta)
        {
            Bitmap dst = new Bitmap(src);
            BitmapData srcData = dst.LockBits(new Rectangle(0, 0, dst.Width, dst.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            f_GuidedFilter((byte*)srcData.Scan0, dst.Width, dst.Height, srcData.Stride, radius, delta);
            dst.UnlockBits(srcData);
            return dst;
        }
       
    }
}
