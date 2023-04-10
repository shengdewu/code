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
        private static extern int SF_PortraitSegment(byte* srcData, int width, int height, int stride, int isThreshold);
        [DllImport("TestDemo_C.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.None, ExactSpelling = true)]
        private static extern int SF_MNN_Initialize(string modelpath, int tensorSize, int stdMode, int numThread);
        [DllImport("TestDemo_C.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.None, ExactSpelling = true)]
        private static extern void SF_MNN_Unitialize();

        [DllImport("TestDemo_C.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.None, ExactSpelling = true)]
        private static extern int SF_FocusVirtualFilter(byte* srcData, int width, int height, int stride, int intensity);
        [DllImport("TestDemo_C.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.None, ExactSpelling = true)]
        private static extern int SF_PortraitEffect(byte* srcData, int width, int height, int stride, int intensity, int effectId);
        public static int MNN_Initialize(string modelPath, int tensorSize, int stdMode, int numThread)
        {
            return SF_MNN_Initialize(modelPath, tensorSize, stdMode, numThread);
        }
        public static void MNN_Unitialize()
        {
            SF_MNN_Unitialize();
        }
        public static Bitmap PortraitSegment(Bitmap src, int isThreshold)
        {
            Bitmap a = new Bitmap(src);
            int w = a.Width;
            int h = a.Height;
            BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            SF_PortraitSegment((byte*)srcData.Scan0, w, h, srcData.Stride, isThreshold);
            a.UnlockBits(srcData);
            GC.Collect();
            return a;
        }
        public static Bitmap FocusVirtualFilter(Bitmap src, int intensity)
        {
            Bitmap a = new Bitmap(src);
            int w = a.Width;
            int h = a.Height;
            BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            SF_FocusVirtualFilter((byte*)srcData.Scan0, w, h, srcData.Stride, intensity);
            a.UnlockBits(srcData);
            GC.Collect();
            return a;
        }
        public static Bitmap PortraitEffect(Bitmap src, int intensity, int effectId)
        {
            Bitmap a = new Bitmap(src);
            int w = a.Width;
            int h = a.Height;
            BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            SF_PortraitEffect((byte*)srcData.Scan0, w, h, srcData.Stride, intensity, effectId);
            a.UnlockBits(srcData);
            GC.Collect();
            return a;
        }
    }
}
