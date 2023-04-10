using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Imaging;
using System.IO;

namespace TestDemo
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        #region  变量声明
        //图像路径
        private String curFileName = null;
        //当前图像变量
        private Bitmap curBitmap = null;
        //原始图像变量
        private Bitmap srcBitmap = null;
        //当前图像人脸关键点数组变亮
        private int[] srcFacePoints = null;
        //眼影程度变量
        private int MK_EYESHADOW_RATIO = 0;
        //眼线程度变量
        private int MK_EYELINE_RATIO = 0;
        //眼睫毛程度变量
        private int MK_EYELASH_RATIO = 0;
        //双眼皮程度变量
        private int MK_EYELID_RATIO = 0;
        //眉毛程度变量
        private int MK_EYEBROW_RATIO = 0;
        //腮红程度变量
        private int MK_FACEBLUSH_RATIO = 0;
        //美瞳程度变量
        private int MK_BEAUTYIRIS_RATIO = 0;
        //修鼻程度变量
        private int MK_HIGHNOSE_RATIO = 0;
        //唇彩程度变量
        private int MK_COLORLIPS_RATIO = 0;
        //磨皮程度变量
        private int MK_SOFTSKIN_RATIO = 0;
        //美白程度变量
        private int MK_SKINWHITE_RATIO = 0;
        //肤色调节变量
        private int MK_SKINCOLOR_RATIO = 0;
        //瘦脸程度变量
        private int MK_FACELIFT_RATIO = 0;
        //眼影图层混合1模式
        private int MK_EYESHADOW_1_MODE = 0;
        //眼影图层混合2模式
        private int MK_EYESHADOW_2_MODE = 0;
        #endregion

        #region  图像打开保存模块
        //打开图像
        public void OpenFile()
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "所有图像文件 | *.bmp; *.pcx; *.png; *.jpg; *.gif;" +
                   "*.tif; *.ico; *.dxf; *.cgm; *.cdr; *.wmf; *.eps; *.emf|" +
                   "位图( *.bmp; *.jpg; *.png;...) | *.bmp; *.pcx; *.png; *.jpg; *.gif; *.tif; *.ico|" +
                   "矢量图( *.wmf; *.eps; *.emf;...) | *.dxf; *.cgm; *.cdr; *.wmf; *.eps; *.emf";
            ofd.ShowHelp = true;
            ofd.Title = "打开图像文件";
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                curFileName = ofd.FileName;
                try
                {
                    curBitmap = (Bitmap)System.Drawing.Image.FromFile(curFileName);
                    srcBitmap = new Bitmap(curBitmap);
                    srcFacePoints = GetPoints(curFileName.Substring(0, curFileName.Length - 4) + ".txt");
                }
                catch (Exception exp)
                { MessageBox.Show(exp.Message); }
            }

        }
        //保存图像
        public void SaveFile()
        {
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.Filter = @"Bitmap文件(*.bmp)|*.bmp|Jpeg文件(*.jpg)|*.jpg|PNG文件(*.png)|*.png|所有合适文件(*.bmp,*.jpg,*.png)|*.bmp;*.jpg;*.png";
            sfd.FilterIndex = 3;
            sfd.RestoreDirectory = true;
            if (sfd.ShowDialog() == DialogResult.OK)
            {
                ImageFormat format = ImageFormat.Jpeg;
                switch (Path.GetExtension(sfd.FileName).ToLower())
                {
                    case ".jpg":
                        format = ImageFormat.Jpeg;
                        break;
                    case ".bmp":
                        format = ImageFormat.Bmp;
                        break;
                    case ".png":
                        format = ImageFormat.Png;
                        break;
                    default:
                        MessageBox.Show("Unsupported image format was specified!");
                        return;
                }
                pictureBox1.Image.Save(sfd.FileName, format);
            }
        }
        //获取人脸关键点
        private int[] GetPoints(string txtPath)
        {
            StreamReader rd = File.OpenText(txtPath);
            string s = rd.ReadLine();
            string[] ss = s.Split(',');
            int[] p = new int[ss.Length - 1];
            for (int i = 0; i < p.Length; i++)
            {
                p[i] = (int)((float.Parse(ss[i])));
            }
            return p;
        }
        //图像打开
        private void button9_Click(object sender, EventArgs e)
        {
            OpenFile();
            if (curBitmap != null)
            {
                pictureBox1.Image = (Image)curBitmap;
            }
        }
        //图像保存
        private void button10_Click(object sender, EventArgs e)
        {
            if (pictureBox1.Image != null)
                SaveFile();
        }
        //鼠标按下显示原图
        private void pictureBox1_MouseDown(object sender, MouseEventArgs e)
        {
            if (srcBitmap != null)
            {
                pictureBox1.Image = srcBitmap;
            }
        }
        //鼠标抬起显示效果图
        private void pictureBox1_MouseUp(object sender, MouseEventArgs e)
        {
            if (curBitmap != null)
            {
                pictureBox1.Image = curBitmap;
            }
        }

        //MyBlog :这个世界上只有一种成功，就是以自己喜欢的方式过一生
        private void linkLabel1_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            System.Diagnostics.Process.Start("https://blog.csdn.net/trent1985");
        }
        #endregion

        #region 美妆模块

        private void GetMakeupParameters()
        {
            try
            {
                MK_EYESHADOW_RATIO = Convert.ToInt32(textBox1.Text);
                MK_EYELINE_RATIO = Convert.ToInt32(textBox2.Text);
                MK_EYELASH_RATIO = Convert.ToInt32(textBox3.Text);
                MK_EYELID_RATIO = Convert.ToInt32(textBox4.Text);
                MK_EYEBROW_RATIO = Convert.ToInt32(textBox5.Text);
                MK_HIGHNOSE_RATIO = Convert.ToInt32(textBox6.Text);
                MK_COLORLIPS_RATIO = Convert.ToInt32(textBox7.Text);
                MK_FACEBLUSH_RATIO = Convert.ToInt32(textBox8.Text);
                MK_BEAUTYIRIS_RATIO = Convert.ToInt32(textBox13.Text);
                MK_FACEBLUSH_RATIO = Convert.ToInt32(textBox8.Text);
                MK_SOFTSKIN_RATIO = Convert.ToInt32(textBox12.Text);
                MK_SKINWHITE_RATIO = Convert.ToInt32(textBox11.Text);
                MK_FACELIFT_RATIO = Convert.ToInt32(textBox9.Text);
                MK_SKINCOLOR_RATIO = Convert.ToInt32(textBox14.Text);
            }
            catch { MessageBox.Show("Parameters input error!"); }
        }
        private void Makeup(string modelPath)
        {
            try
            {
                GetMakeupParameters();

                //美颜-磨皮-美白-肤色调节
                curBitmap = ImageProcessDll.BeautyCamera(srcBitmap, new Bitmap(modelPath + "light.png"), MK_SOFTSKIN_RATIO, MK_SKINWHITE_RATIO, MK_SKINCOLOR_RATIO, 25);
                //美瞳
                int[] maskKeyPointsleft = new int[2 * 13];
                int[] maskKeyPointsright = new int[2 * 13];
                for (int i = 0; i < 24; i++)
                {
                    maskKeyPointsleft[i] = srcFacePoints[2 * 39 + i];
                    maskKeyPointsright[i] = srcFacePoints[2 * 51 + i];
                }
                maskKeyPointsleft[24] = srcFacePoints[2 * 95];
                maskKeyPointsleft[25] = srcFacePoints[2 * 95 + 1];
                maskKeyPointsright[24] = srcFacePoints[2 * 96];
                maskKeyPointsright[25] = srcFacePoints[2 * 96 + 1];
                Bitmap mask = new Bitmap(modelPath + "iris.png");
                int[] maskPoints = new int[] { mask.Width / 2, mask.Height / 2 };
                curBitmap = ImageProcessDll.BeautyIris(curBitmap, maskKeyPointsleft, mask, maskPoints, MK_BEAUTYIRIS_RATIO);
                curBitmap = ImageProcessDll.BeautyIris(curBitmap, maskKeyPointsright, mask, maskPoints, MK_BEAUTYIRIS_RATIO);
                //眼影
                mask = new Bitmap(modelPath + "shadow_l_mask.png");
                maskKeyPointsleft = new int[] { 201, 217, 219, 209, 238, 221, 218, 226 };
                maskKeyPointsright = new int[] { 292, 221, 311, 209, 329, 217, 314, 225 };
                //left eye
                curBitmap = ImageProcessDll.EyeShadow(curBitmap, srcFacePoints, mask, maskKeyPointsleft, true, MK_EYESHADOW_1_MODE, MK_EYESHADOW_RATIO);
                mask = new Bitmap(modelPath + "shadow_l_mask2.png");
                curBitmap = ImageProcessDll.EyeShadow(curBitmap, srcFacePoints, mask, maskKeyPointsleft, true, MK_EYESHADOW_2_MODE, MK_EYESHADOW_RATIO);
                //right eye
                mask = new Bitmap(modelPath + "shadow_r_mask.png");
                curBitmap = ImageProcessDll.EyeShadow(curBitmap, srcFacePoints, mask, maskKeyPointsright, false, MK_EYESHADOW_1_MODE, MK_EYESHADOW_RATIO);
                mask = new Bitmap(modelPath + "shadow_r_mask2.png");
                curBitmap = ImageProcessDll.EyeShadow(curBitmap, srcFacePoints, mask, maskKeyPointsright, false, MK_EYESHADOW_2_MODE, MK_EYESHADOW_RATIO);
                //眼线
                //left eye
                mask = new Bitmap(modelPath + "eyeline_l_mask.png");
                curBitmap = ImageProcessDll.EyeLine(curBitmap, srcFacePoints, mask, maskKeyPointsleft, true, MK_EYELINE_RATIO);
                //right eye
                mask = new Bitmap(modelPath + "eyeline_r_mask.png");
                curBitmap = ImageProcessDll.EyeLine(curBitmap, srcFacePoints, mask, maskKeyPointsright, false, MK_EYELINE_RATIO);
                //睫毛
                //left eye
                mask = new Bitmap(modelPath + "eyelash_l_mask.png");
                curBitmap = ImageProcessDll.EyeLash(curBitmap, srcFacePoints, mask, maskKeyPointsleft, true, MK_EYELASH_RATIO);
                //right eye
                mask = new Bitmap(modelPath + "eyelash_r_mask.png");
                curBitmap = ImageProcessDll.EyeLash(curBitmap, srcFacePoints, mask, maskKeyPointsright, false, MK_EYELASH_RATIO);
                //双眼皮
                //left eye
                mask = new Bitmap(modelPath + "eyelid_l_mask.png");
                curBitmap = ImageProcessDll.EyeLid(curBitmap, srcFacePoints, mask, maskKeyPointsleft, true, MK_EYELID_RATIO);
                //right eye
                mask = new Bitmap(modelPath + "eyelid_r_mask.png");
                curBitmap = ImageProcessDll.EyeLid(curBitmap, srcFacePoints, mask, maskKeyPointsright, false, MK_EYELID_RATIO);
                //眉毛
                mask = new Bitmap(modelPath + "eyebrow_l_mask.png");
                maskKeyPointsleft = new int[] { 184, 189, 220, 185, 245, 193 };
                maskKeyPointsright = new int[] { 287, 193, 311, 185, 346, 188 };
                //left eye
                curBitmap = ImageProcessDll.EyeBrow(curBitmap, srcFacePoints, mask, maskKeyPointsleft, true, MK_EYEBROW_RATIO);
                //right eye
                mask = new Bitmap(modelPath + "eyebrow_r_mask.png");
                curBitmap = ImageProcessDll.EyeBrow(curBitmap, srcFacePoints, mask, maskKeyPointsright, false, MK_EYEBROW_RATIO);
                //修鼻
                mask = new Bitmap(modelPath + "steronose_mask.png");
                int[] maskKeyPoints = new int[] { 265, 219, 242, 270, 288, 271 };
                curBitmap = ImageProcessDll.SteroNose(curBitmap, srcFacePoints, mask, maskKeyPoints, MK_HIGHNOSE_RATIO);
                //唇彩
                mask = new Bitmap(modelPath + "lips_mask.png");
                Bitmap lipslut = new Bitmap(modelPath + "lipsLut.png");
                maskKeyPoints = new int[2 * 28] { 10, 17, 22, 14, 32, 11, 41, 13, 50, 11, 60, 14, 70, 18, 62, 26, 53, 32, 41, 34, 29, 32, 19, 26, 15, 18, 29, 19, 41, 21, 53, 19, 66, 18, 53, 20, 41, 21, 29, 20, 0, 0, 0, 22, 0, 42, 39, 42, 78, 42, 78, 22, 78, 0, 39, 0 };
                curBitmap = ImageProcessDll.LipsRecolor(curBitmap, srcFacePoints, mask, maskKeyPoints, lipslut, MK_COLORLIPS_RATIO);
                //腮红
                mask = new Bitmap(modelPath + "mask_blush_left.png");
                maskKeyPointsleft = new int[] { 170, 232, 211, 332, 254, 240 };
                maskKeyPointsright = new int[] { 520 - 170, 232, 520 - 211, 332, 520 - 254, 240 };
                curBitmap = ImageProcessDll.FaceBlush(curBitmap, srcFacePoints, mask, maskKeyPointsleft, true, MK_FACEBLUSH_RATIO);
                mask = new Bitmap(modelPath + "mask_blush_right.png");
                curBitmap = ImageProcessDll.FaceBlush(curBitmap, srcFacePoints, mask, maskKeyPointsright, false, MK_FACEBLUSH_RATIO);
                //瘦脸
                curBitmap = ImageProcessDll.AutoFaceLift(curBitmap, srcFacePoints, MK_FACELIFT_RATIO);
                ////大眼
                //curBitmap = ImageProcessDll.AutoBigEye(curBitmap, srcFacePoints);
                pictureBox1.Image = curBitmap;
                mask.Dispose();
                //lipslut.Dispose();
                GC.Collect();
            }
            catch (Exception exp)
            {
                MessageBox.Show("Operation error!");
            }

        }
        //这个世界上只有一种成功，就是以自己喜欢的方式过一生,世界上只有一条永远通向成功的路，那就是坚持不懈的努力 
        //妆容A
        private void pictureBox2_Click(object sender, EventArgs e)
        {
            if (pictureBox1.Image != null)
            {
                string modelPath = Application.StartupPath + "\\Makeup\\1\\";
                MK_EYESHADOW_1_MODE = 1;
                MK_EYESHADOW_2_MODE = 0;
                Makeup(modelPath);
            }
        }
        //妆容B
        private void pictureBox3_Click(object sender, EventArgs e)
        {
            if (pictureBox1.Image != null)
            {
                string modelPath = Application.StartupPath + "\\Makeup\\2\\";
                MK_EYESHADOW_1_MODE = 1;
                MK_EYESHADOW_2_MODE = 0;
                Makeup(modelPath);
            }
        }
        //妆容C
        private void pictureBox4_Click(object sender, EventArgs e)
        {
            if (pictureBox1.Image != null)
            {
                string modelPath = Application.StartupPath + "\\Makeup\\3\\";
                MK_EYESHADOW_1_MODE = 0;
                MK_EYESHADOW_2_MODE = 0;
                Makeup(modelPath);
            }
        }
        //妆容D
        private void pictureBox5_Click(object sender, EventArgs e)
        {
            if (pictureBox1.Image != null)
            {
                string modelPath = Application.StartupPath + "\\Makeup\\4\\";
                MK_EYESHADOW_1_MODE = 0;
                MK_EYESHADOW_2_MODE = 0;
                Makeup(modelPath);
            }
        }
        //妆容E
        private void pictureBox6_Click(object sender, EventArgs e)
        {
            if (pictureBox1.Image != null)
            {
                string modelPath = Application.StartupPath + "\\Makeup\\5\\";
                MK_EYESHADOW_1_MODE = 0;
                MK_EYESHADOW_2_MODE = 0;
                Makeup(modelPath);
            }
        }
        //妆容F
        private void pictureBox7_Click(object sender, EventArgs e)
        {
            if (pictureBox1.Image != null)
            {
                string modelPath = Application.StartupPath + "\\Makeup\\6\\";
                MK_EYESHADOW_1_MODE = 1;
                MK_EYESHADOW_2_MODE = 0;
                Makeup(modelPath);
            }
        }
        #endregion

        





    }
}
