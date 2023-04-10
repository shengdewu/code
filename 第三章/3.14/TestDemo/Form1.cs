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
        //美白程度变量
        private int softskinRatio = 80;
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
                    if (curBitmap.Width * curBitmap.Height > 800 * 800)
                    {
                        srcBitmap = new Bitmap(curBitmap, curBitmap.Width / 2, curBitmap.Height / 2);
                        curBitmap = new Bitmap(srcBitmap);
                    }
                    else
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


        private void button1_Click(object sender, EventArgs e)
        {
            if (pictureBox1.Image != null)
            {
                DateTime start = DateTime.Now;
                softskinRatio = Convert.ToInt32(textBox1.Text);
                curBitmap = ImageProcessDll.SoftSkin(srcBitmap, 0, softskinRatio);//.SkinColor(srcBitmap, new Bitmap(Application.StartupPath + "\\SKIN_COLOR_MAP_TANGSHUI.png"), skinwhiteRatio);
                DateTime end = DateTime.Now;
                label4.Text = "Time cost: " + (end - start).ToString();
                pictureBox1.Image = curBitmap;
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

        private void button3_Click(object sender, EventArgs e)
        {
            if (pictureBox1.Image != null)
            {
                DateTime start = DateTime.Now;
                softskinRatio = Convert.ToInt32(textBox3.Text);
                curBitmap = ImageProcessDll.SoftSkin_ChannelMethod(srcBitmap, new Bitmap(Application.StartupPath + "\\light.png"), softskinRatio);
                DateTime end = DateTime.Now;
                label4.Text = "Time cost: " + (end - start).ToString();
                pictureBox1.Image = curBitmap;
            }
        }

        private void button4_Click(object sender, EventArgs e)
        {
            if (pictureBox1.Image != null)
            {
                DateTime start = DateTime.Now;
                softskinRatio = Convert.ToInt32(textBox4.Text);
                int textureRatio = Convert.ToInt32(textBox2.Text);
                curBitmap = ImageProcessDll.SoftSkin_HPMethod(srcBitmap,textureRatio, softskinRatio);//.SkinColor(srcBitmap, new Bitmap(Application.StartupPath + "\\SKIN_COLOR_MAP_TANGSHUI.png"), skinwhiteRatio);
                DateTime end = DateTime.Now;
                label4.Text = "Time cost: " + (end - start).ToString();
                pictureBox1.Image = curBitmap;
            }
        }

        private void button5_Click(object sender, EventArgs e)
        {
            if (pictureBox1.Image != null)
            {
                DateTime start = DateTime.Now;
                softskinRatio = Convert.ToInt32(textBox5.Text);
                int textureRatio = Convert.ToInt32(textBox6.Text);
                curBitmap = ImageProcessDll.SoftSkin_MixMethod(srcBitmap, new Bitmap(Application.StartupPath + "\\light.png"), textureRatio, softskinRatio);//.SkinColor(srcBitmap, new Bitmap(Application.StartupPath + "\\SKIN_COLOR_MAP_TANGSHUI.png"), skinwhiteRatio);
                DateTime end = DateTime.Now;
                label4.Text = "Time cost: " + (end - start).ToString();
                pictureBox1.Image = curBitmap;
            }
        }

        private void button6_Click(object sender, EventArgs e)
        {
            if (pictureBox1.Image != null)
            {
                DateTime start = DateTime.Now;
                softskinRatio = Convert.ToInt32(textBox7.Text);
                float K = Convert.ToInt32(textBox8.Text) / 100.0f;
                curBitmap = ImageProcessDll.SoftSkin_DetailsAddingMethod(srcBitmap, softskinRatio, K);
                DateTime end = DateTime.Now;
                label4.Text = "Time cost: " + (end - start).ToString();
                pictureBox1.Image = curBitmap;
            }
        }

    }
}
