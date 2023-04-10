using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Imaging;

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
        #endregion

        #region  图像打开保存模块
        //打开图像函数
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
                }
                catch (Exception exp)
                { MessageBox.Show(exp.Message); }
            }
        }
        //保存图像函数
        public void SaveFile()
        {
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.Filter = "PNG文件(*.png)|*.png";
            if (sfd.ShowDialog() == DialogResult.OK)
            {
                pictureBox1.Image.Save(sfd.FileName, ImageFormat.Png);
            }

        }
        //打开图像
        private void openBtn_Click(object sender, EventArgs e)
        {
            OpenFile();
            if (curBitmap != null)
            {
                pictureBox1.Image = (Image)curBitmap;
            }
        }
        //保存图像
        private void saveBtn_Click(object sender, EventArgs e)
        {
            if (pictureBox1.Image != null)
                SaveFile();
        }
        #endregion


        private void button2_Click(object sender, EventArgs e)
        {
            if (pictureBox1.Image != null)
            {
                SaveFile();
            }
        }


        private void pictureBox1_MouseDown(object sender, MouseEventArgs e)
        {
            if (srcBitmap != null)
            {
                pictureBox1.Image = srcBitmap;
            }
        }

        private void pictureBox1_MouseUp(object sender, MouseEventArgs e)
        {
            if (curBitmap != null)
            {
                pictureBox1.Image = curBitmap;
            }
        }

        private void linkLabel1_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            System.Diagnostics.Process.Start("https://blog.csdn.net/trent1985");
        }

        //这个世界上只有一种成功，就是以自己喜欢的方式过一生
        private void button1_Click_1(object sender, EventArgs e)
        {
            if (pictureBox1.Image != null)
            {
                try
                {
                    int[] srcFacePoints = new int[] { 123, 199, 124, 226, 128, 249, 134, 273, 141, 296, 152, 318, 165, 337, 182, 356, 200, 372, 225, 379, 250, 376, 272, 364, 291, 347, 306, 328, 319, 306, 327, 283, 335, 258, 342, 233, 344, 206, 135, 181, 148, 169, 164, 167, 181, 168, 197, 172, 209, 186, 193, 183, 178, 180, 164, 179, 150, 179, 249, 185, 262, 173, 279, 170, 296, 170, 312, 174, 324, 186, 310, 184, 295, 182, 280, 182, 265, 184, 152, 206, 158, 199, 166, 196, 174, 196, 184, 198, 192, 204, 198, 213, 190, 214, 182, 215, 173, 215, 165, 213, 158, 210, 258, 215, 264, 207, 272, 202, 282, 200, 291, 201, 298, 205, 304, 211, 298, 215, 291, 217, 283, 218, 274, 218, 266, 217, 214, 209, 213, 233, 208, 257, 196, 267, 201, 283, 219, 288, 231, 289, 250, 285, 255, 270, 243, 258, 241, 234, 242, 210, 187, 313, 201, 309, 215, 308, 226, 313, 237, 310, 250, 312, 266, 315, 255, 327, 242, 335, 225, 339, 208, 335, 196, 325, 193, 316, 210, 318, 226, 320, 242, 318, 259, 317, 242, 319, 226, 321, 209, 318, 176, 205, 281, 209, 227, 208, 227, 248, 226, 274, 226, 289 };
                    Bitmap mask = new Bitmap(Application.StartupPath + "\\" +  "mask_blush_left.png");
                    int[] maskKeyPointsleft = new int[] { 170, 232, 211, 332, 254, 240 };
                    int[] maskKeyPointsright = new int[] { 520 - 170, 232, 520 - 211, 332, 520 - 254, 240 };
                    int ratio = Convert.ToInt32(textBox1.Text);
                    DateTime start = DateTime.Now;
                    curBitmap = ImageProcessDll.FaceBlush(srcBitmap, srcFacePoints, mask, maskKeyPointsleft,true, ratio);
                    mask = new Bitmap(Application.StartupPath + "\\" + "mask_blush_right.png");
                    curBitmap = ImageProcessDll.FaceBlush(curBitmap, srcFacePoints, mask, maskKeyPointsright, false, ratio);
                    DateTime end = DateTime.Now;
                    label4.Text = "Time cost: " + (end - start).ToString();
                    pictureBox1.Image = curBitmap;
                }
                catch (Exception exp)
                { MessageBox.Show("操作错误！"); };

            }
        }

        

        private void Form1_Load(object sender, EventArgs e)
        {
            srcBitmap = new Bitmap(Application.StartupPath + "\\Test.png");
            pictureBox1.Image = srcBitmap;
        }

        private void button2_Click_1(object sender, EventArgs e)
        {
            if (pictureBox1.Image != null)
                SaveFile();
        }


        
       

    }
}
