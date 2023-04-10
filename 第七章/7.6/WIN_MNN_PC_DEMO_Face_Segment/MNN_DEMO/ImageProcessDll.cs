using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;


namespace MNN_DEMO
{
    unsafe class ImageProcessDll
    {
        [DllImport("TestDemo_C.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.None, ExactSpelling = true)]
        private static extern int SF_FaceskinSegment(byte* srcData, int width, int height, int stride, int isThreshold);
        [DllImport("TestDemo_C.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.None, ExactSpelling = true)]
        private static extern int SF_MNN_Initialize(string modelpath, int tensorSize, int stdMode, int numThread);
        [DllImport("TestDemo_C.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.None, ExactSpelling = true)]
        private static extern void SF_MNN_Unitialize();
        [DllImport("TestDemo_C.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.None, ExactSpelling = true)]
        private static extern int SF_AI_Softskin(byte* srcData, int width, int height, int stride, int ratio);
        [DllImport("TestDemo_C.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, ExactSpelling = true)]
        private static extern int f_SkinWhiteCurve(byte* srcData, int width, int height, int stride, int belta, int ratio);
        public static int MNN_Initialize(string modelPath, int tensorSize, int stdMode, int numThread)
        {
            return SF_MNN_Initialize(modelPath, tensorSize, stdMode, numThread);
        }
        public static void MNN_Unitialize()
        {
            SF_MNN_Unitialize();
        }
        public static Bitmap FaceSkinSegment(Bitmap src, int isThreshold)
        {
            Bitmap a = new Bitmap(src);
            int w = a.Width;
            int h = a.Height;
            BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            SF_FaceskinSegment((byte*)srcData.Scan0, w, h, srcData.Stride, isThreshold);
            a.UnlockBits(srcData);
            GC.Collect();
            return a;
        }
        public static Bitmap AI_Softskin(Bitmap src, int ratio)
        {
            Bitmap a = new Bitmap(src);
            int w = a.Width;
            int h = a.Height;
            BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            SF_AI_Softskin((byte*)srcData.Scan0, w, h, srcData.Stride, ratio);
            a.UnlockBits(srcData);
            GC.Collect();
            return a;
        }
        public static Bitmap SkinWhiteCurve(Bitmap src, int ratio)
        {
            Bitmap dst = new Bitmap(src);
            BitmapData srcData = dst.LockBits(new Rectangle(0, 0, dst.Width, dst.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            f_SkinWhiteCurve((byte*)srcData.Scan0, dst.Width, dst.Height, srcData.Stride, 2, ratio);
            dst.UnlockBits(srcData);
            return dst;
        }
    }
}
