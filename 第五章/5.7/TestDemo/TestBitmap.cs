using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Drawing.Imaging;

namespace TestDemo
{
    unsafe class TestBitmap
    {
        public Bitmap TestBmp(Bitmap src)
        {
            Bitmap a = new Bitmap(src);
            int w = a.Width;
            int h = a.Height;
            BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            byte* p = (byte*)srcData.Scan0;
            int r = 0, g = 0, b = 0, offset = srcData.Stride - w * 4;
            for (int j = 0; j < h; j++)
            {
                for (int i = 0; i < w; i++)
                {
                    b = p[0];
                    g = p[1];
                    r = p[2];
                    ////////////////Process image...
                    p += 4;
                }
                p += offset;
            }
            a.UnlockBits(srcData);
            return a;
        }
    }
}
