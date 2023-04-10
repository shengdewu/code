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
        private static extern int SF_HairColored(byte* srcData, int width, int height, int stride, byte* filtermap, byte* colorMask, int ratio);
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
        public static Bitmap HairRecolor(Bitmap src, Bitmap filtermap, Bitmap colorMask, int ratio)
        {
            Bitmap a = new Bitmap(src);
            int w = a.Width;
            int h = a.Height;
            BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            BitmapData mapData = filtermap.LockBits(new Rectangle(0, 0, filtermap.Width, filtermap.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            BitmapData colorData = colorMask.LockBits(new Rectangle(0, 0, colorMask.Width, colorMask.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            SF_HairColored((byte*)srcData.Scan0, w, h, srcData.Stride, (byte*)mapData.Scan0, (byte*)colorData.Scan0, ratio);
            a.UnlockBits(srcData);
            filtermap.UnlockBits(mapData);
            colorMask.UnlockBits(colorData);
            GC.Collect();
            return a;
        }
    }
}
