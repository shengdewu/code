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
        private static extern int SF_MNN_Initialize(string fdmodelpath, string famodelPath, int tensorSize, int stdMode, int numThread);
        [DllImport("TestDemo_C.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.None, ExactSpelling = true)]
        private static extern void SF_MNN_Unitialize();
        [DllImport("TestDemo_C.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.None, ExactSpelling = true)]
        private static extern int SF_FaceDetect(byte* srcData, int width, int height, int stride, int []faceRects);
        [DllImport("TestDemo_C.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.None, ExactSpelling = true)]
        private static extern int SF_FaceAlignment(byte* srcData, int width, int height, int stride, int []faceRect, float []facePoints);
        [DllImport("TestDemo_C.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.None, ExactSpelling = true)]
        private static extern int SF_Gender_MNN_Initialize(string genderModelPath, int stdMode, int numThread);
        [DllImport("TestDemo_C.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.None, ExactSpelling = true)]
        private static extern void SF_Gender_MNN_Unitialize();
        [DllImport("TestDemo_C.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.None, ExactSpelling = true)]
        private static extern int SF_FaceGender(byte* srcData, int width, int height, int stride, float[] outGender);

        public static int MNN_Initialize(string fdmodelPath, string famodelPath, int tensorSize, int stdMode, int numThread)
        {
            return SF_MNN_Initialize(fdmodelPath, famodelPath, tensorSize, stdMode, numThread);
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
        public static Bitmap FaceDetection(Bitmap src, int[] faceRects)
        {
            Bitmap a = new Bitmap(src);
            int w = a.Width;
            int h = a.Height;
            BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            SF_FaceDetect((byte*)srcData.Scan0, w, h, srcData.Stride, faceRects);
            a.UnlockBits(srcData);
            GC.Collect();
            return a;
        }
        public static Bitmap FaceAlignment(Bitmap src, int[] faceRect, float[] facePoints)
        {
            Bitmap a = new Bitmap(src);
            int w = a.Width;
            int h = a.Height;
            BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            SF_FaceAlignment((byte*)srcData.Scan0, w, h, srcData.Stride, faceRect, facePoints);
            a.UnlockBits(srcData);
            GC.Collect();
            return a;
        }

        public static int Gender_MNN_Initialize(string fdmodelPath, int stdMode, int numThread)
        {
            return SF_Gender_MNN_Initialize(fdmodelPath, stdMode, numThread);
        }
        public static void Gender_MNN_Unitialize()
        {
            SF_Gender_MNN_Unitialize();
        }
        public static Bitmap FaceGender(Bitmap src, float[]outGender)
        {
            Bitmap a = new Bitmap(src);
            int w = a.Width;
            int h = a.Height;
            BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            SF_FaceGender((byte*)srcData.Scan0, w, h, srcData.Stride,outGender);
            a.UnlockBits(srcData);
            GC.Collect();
            return a;
        }
    }
}
