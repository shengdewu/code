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
        private static extern int SF_NailsColored(byte* srcData, int width, int height, int stride, int R, int G, int B, int ratio);
        [DllImport("TestDemo_C.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.None, ExactSpelling = true)]
        private static extern int SF_MNN_Initialize(string modelpath, int tensorSize, int stdMode, int numThread);
        [DllImport("TestDemo_C.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.None, ExactSpelling = true)]
        private static extern void SF_MNN_Unitialize();

        public static int MNN_Initialize(string modelPath, int tensorSize, int stdMode, int numThread)
        {
            return SF_MNN_Initialize(modelPath, tensorSize, stdMode, numThread);
        }
        public static void MNN_Unitialize()
        {
            SF_MNN_Unitialize();
        }
        public static Bitmap ColorNails(Bitmap src, int r, int g, int b, int ratio)
        {
            Bitmap a = new Bitmap(src);
            int w = a.Width;
            int h = a.Height;
            BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            SF_NailsColored((byte*)srcData.Scan0, w, h, srcData.Stride, r, g, b, ratio);
            a.UnlockBits(srcData);
            return a;
        }
    }
}
