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
        private int[] facePoints = null;
        private int cenX = 0;
        private int cenY = 0;
        private int radius = 20;
        private int intensity = 12;
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
                    //facePoints = GetPoints(curFileName.Substring(0, curFileName.Length - 4) + ".txt");
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

        private void button1_Click(object sender, EventArgs e)
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
            if (checkBox1.Checked)
            {
                cenX = e.X;
                cenY = e.Y;
            }
            else if (srcBitmap != null)
            {
                pictureBox1.Image = srcBitmap;
            }
        }

        private void pictureBox1_MouseUp(object sender, MouseEventArgs e)
        {
            if (checkBox1.Checked)
            {
                curBitmap = ImageProcessDll.BigEye(srcBitmap, cenX, cenY, radius, intensity);
                pictureBox1.Image = curBitmap;
            }
            else if (curBitmap != null)
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
                try
                {
                    checkBox1.Checked = false;
                    DateTime start = DateTime.Now;
                    curBitmap = ImageProcessDll.AutoBigEye(srcBitmap, facePoints);
                    DateTime end = DateTime.Now;
                    label4.Text = "Time cost: " + (end - start).ToString();
                    pictureBox1.Image = curBitmap;
                }
                catch (Exception exp)
                { MessageBox.Show("请输入正确的H矩阵！"); };

            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            srcBitmap = new Bitmap(Application.StartupPath + "\\Model.jpg");
            curBitmap = new Bitmap(srcBitmap);
            facePoints = new int[] { 164, 189, 167, 210, 172, 229, 177, 250, 183, 268, 194, 284, 205, 298, 219, 311, 233, 322, 253, 326, 274, 322, 289, 311, 302, 298, 312, 283, 322, 266, 328, 246, 332, 225, 336, 205, 338, 183, 174, 175, 183, 164, 196, 162, 209, 164, 222, 167, 233, 177, 220, 176, 208, 174, 196, 173, 185, 173, 271, 176, 280, 166, 293, 161, 306, 159, 319, 161, 329, 170, 318, 169, 307, 170, 295, 172, 283, 175, 193, 195, 198, 190, 204, 189, 211, 188, 218, 189, 224, 193, 229, 199, 223, 200, 217, 201, 211, 202, 204, 201, 198, 198, 277, 198, 282, 192, 288, 188, 295, 187, 302, 187, 308, 189, 312, 193, 308, 196, 303, 199, 296, 200, 290, 200, 283, 199, 241, 197, 241, 215, 239, 233, 230, 240, 233, 253, 248, 258, 257, 258, 274, 253, 279, 239, 268, 233, 264, 215, 263, 196, 220, 272, 232, 273, 244, 272, 254, 275, 262, 272, 274, 272, 287, 272, 278, 283, 268, 291, 253, 294, 240, 291, 229, 283, 227, 275, 240, 278, 254, 280, 267, 278, 281, 275, 268, 279, 254, 282, 240, 279, 212, 195, 294, 193, 252, 195, 252, 228, 254, 247, 253, 258 };
            pictureBox1.Image = srcBitmap;
            pictureBox1.Width = srcBitmap.Width;
            pictureBox1.Height = srcBitmap.Height;
        }

        private void hScrollBar1_Scroll(object sender, ScrollEventArgs e)
        {
            textBox1.Text = hScrollBar1.Value.ToString();
            intensity = hScrollBar1.Value;
        }
        //get points of face
        private int[] GetPoints(string txtPath)
        {
            StreamReader rd = File.OpenText(txtPath);
            string s = rd.ReadLine();
            string[] ss = s.Split(',');
            int[] p = new int[ss.Length - 1];
            for (int i = 0; i < p.Length; i++)
            {
                p[i] = int.Parse(ss[i]);
            }
            return p;
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            curBitmap = new Bitmap(srcBitmap);
            pictureBox1.Image = curBitmap;
        }

        private void hScrollBar2_Scroll(object sender, ScrollEventArgs e)
        {
            textBox2.Text = hScrollBar2.Value.ToString();
            radius = hScrollBar2.Value;
        }

        private void button1_Click_1(object sender, EventArgs e)
        {
            if (pictureBox1.Image != null)
                SaveFile();
        }


        
       

    }
}
