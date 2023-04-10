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
        //眼影
        [DllImport("Makeup.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, ExactSpelling = true)]
        private static extern int f_EyeShadow(byte* srcData, int width, int height, int stride, int[] srcFacePoints, byte* mskData, int mWidth, int mHeight, int mStride, int[] maskKeyPoints, bool isLeft, int mode, int ratio);
        //双眼皮
        [DllImport("Makeup.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, ExactSpelling = true)]
        private static extern int f_EyeLid(byte* srcData, int width, int height, int stride, int []srcFacePoints, byte* mskData, int mWidth, int mHeight, int mStride, int []maskKeyPoints, bool isLeft, int ratio);
        //眼线
        [DllImport("Makeup.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, ExactSpelling = true)]
        private static extern int f_EyeLine(byte* srcData, int width, int height, int stride, int[] srcFacePoints, byte* mskData, int mWidth, int mHeight, int mStride, int[] maskKeyPoints, bool isLeft, int ratio);
        //眼睫毛
        [DllImport("Makeup.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, ExactSpelling = true)]
        private static extern int f_EyeLash(byte* srcData, int width, int height, int stride, int[] srcFacePoints, byte* mskData, int mWidth, int mHeight, int mStride, int[] maskKeyPoints, bool isLeft, int ratio);
        //眉毛
        [DllImport("Makeup.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, ExactSpelling = true)]
        private static extern int f_EyeBrow(byte* srcData, int width, int height, int stride, int[] srcFacePoints, byte* mskData, int mWidth, int mHeight, int mStride, int[] maskKeyPoints, bool isLeft, int ratio);
        //修鼻
        [DllImport("Makeup.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, ExactSpelling = true)]
        private static extern int f_SteroNose(byte* srcData, int width, int height, int stride, int[] srcFacePoints, byte* mskData, int mWidth, int mHeight, int mStride, int[] maskKeyPoints, int ratio);
        //唇妆
        [DllImport("Makeup.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, ExactSpelling = true)]
        private static extern int f_LipsRecolor(byte* srcData, int width, int height, int stride, int[] srcFacePoints, byte* mskData, int mWidth, int mHeight, int mStride, int[] maskKeyPoints, byte* lipsColorMap, int ratio);
        //美瞳
        [DllImport("Makeup.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, ExactSpelling = true)]
        private static extern int f_BeautyIris(byte* srcData, int width, int height, int stride, int[] srcPoints, byte* maskData, int mWidth, int mHeight, int mStride, int[] maskPoints, int ratio);
        //腮红
        [DllImport("Makeup.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, ExactSpelling = true)]
        private static extern int f_FaceBlush(byte* srcData, int width, int height, int stride, int[] srcFacePoints, byte* mskData, int mWidth, int mHeight, int mStride, int[] maskKeyPoints, bool isLeft, int ratio);
        //美颜
        [DllImport("Makeup.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, ExactSpelling = true)]
        private static extern int f_BeautyCamera(byte* srcData, int width, int height, int stride, byte* curveMap, int softRatio, int skinWhiteRatio, int skinColorRatio, int sharpenRatio);
        //自动瘦脸
        [DllImport("Makeup.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, ExactSpelling = true)]
        private static extern int f_AutoFacelift(byte* srcData, int width, int height, int stride, int[] facePoints, int intensity);
        //自动大眼
        [DllImport("Makeup.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, ExactSpelling = true)]
        private static extern int f_AutoBigeye(byte* srcData, int width, int height, int stride, int[] facePoints);
        //大眼
        [DllImport("Makeup.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, ExactSpelling = true)]
        private static extern int f_BigEye(byte* srcData, int width, int height, int stride, int cenX, int cenY, int radius, int intensity);
        //自动大眼
        public static Bitmap AutoBigEye(Bitmap src, int[] facePoints)
        {
            Bitmap a = new Bitmap(src);
            BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            f_AutoBigeye((byte*)srcData.Scan0, a.Width, a.Height, srcData.Stride, facePoints);
            a.UnlockBits(srcData);
            return a;
        }
        //自动瘦脸
        public static Bitmap AutoFaceLift(Bitmap src, int[] facePoints, int intensity)
        {
            Bitmap a = new Bitmap(src);
            BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            f_AutoFacelift((byte*)srcData.Scan0, a.Width, a.Height, srcData.Stride, facePoints, intensity);
            a.UnlockBits(srcData);
            return a;
        }
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //美颜
        public static Bitmap BeautyCamera(Bitmap src, Bitmap lut, int softRatio, int skinWhiteRatio, int skinColorRatio, int sharpenRatio)
        {
            Bitmap dst = new Bitmap(src);
            BitmapData srcData = dst.LockBits(new Rectangle(0, 0, dst.Width, dst.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            BitmapData lutData = lut.LockBits(new Rectangle(0, 0, lut.Width, lut.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            f_BeautyCamera((byte*)srcData.Scan0, dst.Width, dst.Height, srcData.Stride, (byte*)lutData.Scan0, softRatio, skinWhiteRatio, skinColorRatio, sharpenRatio);
            dst.UnlockBits(srcData);
            lut.UnlockBits(lutData);
            return dst;
        }
        //腮红
        public static Bitmap FaceBlush(Bitmap src, int[] srcPoints, Bitmap mask, int[] maskPoints, bool isLeft, int ratio)
        {
            Bitmap a = new Bitmap(src);
            BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            BitmapData maskData = mask.LockBits(new Rectangle(0, 0, mask.Width, mask.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            f_FaceBlush((byte*)srcData.Scan0, a.Width, a.Height, srcData.Stride, srcPoints, (byte*)maskData.Scan0, mask.Width, mask.Height, maskData.Stride, maskPoints, isLeft, ratio);
            a.UnlockBits(srcData);
            mask.UnlockBits(maskData);
            return a;
        }
        //美瞳
        public static Bitmap BeautyIris(Bitmap src, int[] srcPoints, Bitmap mask, int[] maskPoints, int ratio)
        {
            Bitmap a = new Bitmap(src);
            BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            BitmapData maskData = mask.LockBits(new Rectangle(0, 0, mask.Width, mask.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            f_BeautyIris((byte*)srcData.Scan0, a.Width, a.Height, srcData.Stride, srcPoints, (byte*)maskData.Scan0, mask.Width, mask.Height, maskData.Stride, maskPoints, ratio);
            a.UnlockBits(srcData);
            mask.UnlockBits(maskData);
            return a;
        }
        //眼影
        public static Bitmap EyeShadow(Bitmap src, int[] srcPoints, Bitmap mask, int[] maskPoints, bool isLeft, int mode, int ratio)
        {
            Bitmap a = new Bitmap(src);
            BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            BitmapData maskData = mask.LockBits(new Rectangle(0, 0, mask.Width, mask.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            f_EyeShadow((byte*)srcData.Scan0, a.Width, a.Height, srcData.Stride, srcPoints, (byte*)maskData.Scan0, mask.Width, mask.Height, maskData.Stride, maskPoints, isLeft, mode, ratio);
            a.UnlockBits(srcData);
            mask.UnlockBits(maskData);
            return a;
        }
        //双眼皮
        public static Bitmap EyeLid(Bitmap src, int[] srcPoints, Bitmap mask, int[] maskPoints, bool isLeft, int ratio)
        {
            Bitmap a = new Bitmap(src);
            BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            BitmapData maskData = mask.LockBits(new Rectangle(0, 0, mask.Width, mask.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            f_EyeLid((byte*)srcData.Scan0, a.Width, a.Height, srcData.Stride, srcPoints, (byte*)maskData.Scan0, mask.Width, mask.Height, maskData.Stride, maskPoints, isLeft, ratio);
            a.UnlockBits(srcData);
            mask.UnlockBits(maskData);
            return a;
        }
        //眼线
        public static Bitmap EyeLine(Bitmap src, int[] srcPoints, Bitmap mask, int[] maskPoints, bool isLeft, int ratio)
        {
            Bitmap a = new Bitmap(src);
            BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            BitmapData maskData = mask.LockBits(new Rectangle(0, 0, mask.Width, mask.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            f_EyeLine((byte*)srcData.Scan0, a.Width, a.Height, srcData.Stride, srcPoints, (byte*)maskData.Scan0, mask.Width, mask.Height, maskData.Stride, maskPoints, isLeft, ratio);
            a.UnlockBits(srcData);
            mask.UnlockBits(maskData);
            return a;
        }
        //眼睫毛
        public static Bitmap EyeLash(Bitmap src, int[] srcPoints, Bitmap mask, int[] maskPoints, bool isLeft, int ratio)
        {
            Bitmap a = new Bitmap(src);
            BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            BitmapData maskData = mask.LockBits(new Rectangle(0, 0, mask.Width, mask.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            f_EyeLash((byte*)srcData.Scan0, a.Width, a.Height, srcData.Stride, srcPoints, (byte*)maskData.Scan0, mask.Width, mask.Height, maskData.Stride, maskPoints, isLeft, ratio);
            a.UnlockBits(srcData);
            mask.UnlockBits(maskData);
            return a;
        }
        //眉毛
        public static Bitmap EyeBrow(Bitmap src, int[] srcPoints, Bitmap mask, int[] maskPoints, bool isLeft, int ratio)
        {
            Bitmap a = new Bitmap(src);
            BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            BitmapData maskData = mask.LockBits(new Rectangle(0, 0, mask.Width, mask.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            f_EyeBrow((byte*)srcData.Scan0, a.Width, a.Height, srcData.Stride, srcPoints, (byte*)maskData.Scan0, mask.Width, mask.Height, maskData.Stride, maskPoints, isLeft, ratio);
            a.UnlockBits(srcData);
            mask.UnlockBits(maskData);
            return a;
        }
        //修鼻
        public static Bitmap SteroNose(Bitmap src, int[] srcPoints, Bitmap mask, int[] maskPoints, int ratio)
        {
            Bitmap a = new Bitmap(src);
            BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            BitmapData maskData = mask.LockBits(new Rectangle(0, 0, mask.Width, mask.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            f_SteroNose((byte*)srcData.Scan0, a.Width, a.Height, srcData.Stride, srcPoints, (byte*)maskData.Scan0, mask.Width, mask.Height, maskData.Stride, maskPoints, ratio);
            a.UnlockBits(srcData);
            mask.UnlockBits(maskData);
            return a;
        }
        //唇妆
        public static Bitmap LipsRecolor(Bitmap src, int[] srcPoints, Bitmap mask, int[] maskPoints, Bitmap lipsColorLut, int ratio)
        {
            Bitmap a = new Bitmap(src);
            BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            BitmapData maskData = mask.LockBits(new Rectangle(0, 0, mask.Width, mask.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            BitmapData lutData = lipsColorLut.LockBits(new Rectangle(0, 0, lipsColorLut.Width, lipsColorLut.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            f_LipsRecolor((byte*)srcData.Scan0, a.Width, a.Height, srcData.Stride, srcPoints, (byte*)maskData.Scan0, mask.Width, mask.Height, maskData.Stride, maskPoints, (byte*)lutData.Scan0, ratio);
            a.UnlockBits(srcData);
            mask.UnlockBits(maskData);
            lipsColorLut.UnlockBits(lutData);
            return a;
        }
    }
}
