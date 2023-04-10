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

        #region  Member Variables
        //image name
        private String curFileName = null;
        //current image 
        private Bitmap curBitmap = null;
        //source image 
        private Bitmap srcBitmap = null;
        TestBitmap zp = new TestBitmap();
        //resource path
        private string startPath = null;
        #endregion

        #region  Image Open&Save Methods
        //open
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
        //save
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
        ////get points of face
        //private int[] GetPoints(string txtPath)
        //{
        //    StreamReader rd = File.OpenText(txtPath);
        //    string s = rd.ReadLine();
        //    string[] ss = s.Split(',');
        //    int[] p = new int[ss.Length - 1]; 
        //    for (int i = 0; i < p.Length; i++)
        //    {
        //        p[i] = int.Parse(ss[i]);
        //    }
        //    return p;
        //}
        //open image
        private void openBtn_Click(object sender, EventArgs e)
        {
            OpenFile();
            if (curBitmap != null)
            {
                pictureBox1.Image = (Image)curBitmap;
            }
        }
        //save image
        private void saveBtn_Click(object sender, EventArgs e)
        {
            if (pictureBox1.Image != null)
                SaveFile();
        }
        private void pictureBox1_MouseDown(object sender, MouseEventArgs e)
        {
            if (srcBitmap != null)
                pictureBox1.Image = srcBitmap;
        }

        private void pictureBox1_MouseUp(object sender, MouseEventArgs e)
        {
            if (curBitmap != null)
                pictureBox1.Image = curBitmap;
        }
        #endregion

        #region ImageProcess

        #endregion

        private int[] hist = null;
        private bool drawHist = false;
        private int histMode = 0;
        private void button1_Click(object sender, EventArgs e)
        {
            if (pictureBox1.Image != null)
            {
                hist = new int[256];
                histMode = 0;
                zp.Histagram(srcBitmap, ref hist, histMode);
                pictureBox1.Image = curBitmap;
                drawHist = true;
                this.Invalidate();
            }
        }



        private void button2_Click(object sender, EventArgs e)
        {
            if (pictureBox1.Image != null)
            {
                hist = new int[256];
                histMode = 1;
                zp.Histagram(srcBitmap, ref hist, histMode);
                pictureBox1.Image = curBitmap;
                drawHist = true;
                this.Invalidate();
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (pictureBox1.Image != null)
            {
                hist = new int[256];
                histMode = 2;
                zp.Histagram(srcBitmap, ref hist, histMode);
                pictureBox1.Image = curBitmap;
                drawHist = true;
                this.Invalidate();
            }
        }

        private void button4_Click(object sender, EventArgs e)
        {
            if (pictureBox1.Image != null)
            {
                hist = new int[256];
                histMode = 3;
                zp.Histagram(srcBitmap, ref hist, histMode);
                pictureBox1.Image = curBitmap;
                drawHist = true;
                this.Invalidate();
            }
        }

        private void button5_Click(object sender, EventArgs e)
        {
            drawHist = false;
            this.pictureBox1 .Invalidate();
        }

        private void pictureBox1_Paint(object sender, PaintEventArgs e)
        {
            if (drawHist)
            {
                Graphics g = e.Graphics;
                Pen curPen = null;
                switch (histMode)
                {
                    case 0:
                        curPen = new Pen(Brushes.Black, 1);
                        break;
                    case 1:
                        curPen = new Pen(Brushes.Red, 1);
                        break;
                    case 2:
                        curPen = new Pen(Brushes.Green, 1);
                        break;
                    case 3:
                        curPen = new Pen(Brushes.Blue, 1);
                        break;
                    default:
                        break;
                }
                //绘制坐标系
                g.DrawLine(curPen, 50, 240, 320, 240);
                g.DrawLine(curPen, 50, 240, 50, 30);
                g.DrawLine(curPen, 100, 240, 100, 242);
                g.DrawLine(curPen, 150, 240, 150, 242);
                g.DrawLine(curPen, 200, 240, 200, 242);
                g.DrawLine(curPen, 250, 240, 250, 242);
                g.DrawLine(curPen, 300, 240, 300, 242);
                g.DrawString("0", new Font("New Timer", 8), Brushes.Black, new PointF(46, 242));
                g.DrawString("50", new Font("New Timer", 8), Brushes.Black, new PointF(92, 242));
                g.DrawString("100", new Font("New Timer", 8), Brushes.Black, new PointF(139, 242));
                g.DrawString("150", new Font("New Timer", 8), Brushes.Black, new PointF(189, 242));
                g.DrawString("200", new Font("New Timer", 8), Brushes.Black, new PointF(239, 242));
                g.DrawString("250", new Font("New Timer", 8), Brushes.Black, new PointF(289, 242));
                g.DrawLine(curPen, 48, 40, 50, 40);
                g.DrawString("0", new Font("New Timer", 8), Brushes.Black, new PointF(34, 234));
                //double temp;
                //绘制灰度直方图
                int maxPixelGray = hist.Max();
                g.DrawString(maxPixelGray.ToString(), new Font("New Timer", 8), Brushes.Black, new PointF(18, 34));
                double temp = 0;
                for (int i = 0; i < 256; i++)
                {
                    temp = 200.0 * hist[i] / maxPixelGray;

                    g.DrawLine(curPen, 50 + i, 240, 50 + i, 240 - (int)temp);
                }
                curPen.Dispose();
            }
        }


    }
}
