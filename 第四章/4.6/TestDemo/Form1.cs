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
        //FaceLift
        //手动拖动终点坐标
        private Point mPoint;
        //瘦脸变形区域圆心点坐标
        private Point centerPoint;
        //瘦脸开关变量
        private bool faceliftSwitch = false;
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

        private void linkLabel1_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            System.Diagnostics.Process.Start("https://blog.csdn.net/trent1985");
        }


        private void button1_Click_1(object sender, EventArgs e)
        {
            if (pictureBox1.Image != null)
            {
                try
                {
                    OpenFile();
                    if (curBitmap != null)
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
            pictureBox1.Image = curBitmap;
        }

        private void pictureBox1_MouseDown(object sender, MouseEventArgs e)
        {
            if (pictureBox1.Image != null)
            {
                centerPoint = e.Location;
                faceliftSwitch = true;
            }
        }

        private void pictureBox1_MouseUp(object sender, MouseEventArgs e)
        {
            if (pictureBox1.Image != null)
            {
                faceliftSwitch = false;
                srcBitmap = new Bitmap(curBitmap);
                GC.Collect();
            }
        }
        private void pictureBox1_MouseMove(object sender, MouseEventArgs e)
        {
            if (pictureBox1.Image != null && faceliftSwitch)
            {
                mPoint = e.Location;
                curBitmap = ImageProcessDll.FaceLift(srcBitmap, centerPoint.X, centerPoint.Y, Convert.ToInt32(textBox1.Text), mPoint.X, mPoint.Y, Convert.ToInt32(textBox2.Text));
                pictureBox1.Image = curBitmap;
                GC.Collect();
                pictureBox1.Invalidate();
            }
        }



        
       

    }
}
