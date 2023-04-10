using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MNN_DEMO
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        //图像路径
        private String curFileName = null;
        //当前图像变量
        private Bitmap curBitmap = null;
        //原始图像变量
        private Bitmap srcBitmap = null;

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
        private void button1_Click(object sender, EventArgs e)
        {
            OpenFile();
            if (curBitmap != null)
            {
                pictureBox1.Image = (Image)curBitmap;
            }
        }
        //保存图像
        private void button3_Click(object sender, EventArgs e)
        {
            if (pictureBox1.Image != null)
                SaveFile();
        }


        #endregion

        private void button2_Click(object sender, EventArgs e)
        {
            if (curBitmap != null)
            {
                ImageProcessDll.MNN_Initialize(Application.StartupPath + "\\portrait_segment.mnn", 256, 0, 1);
                curBitmap = ImageProcessDll.FocusVirtualFilter(srcBitmap, hScrollBar1.Value);
                pictureBox1.Image = curBitmap;
                GC.Collect();
            }
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

        private void hScrollBar1_Scroll(object sender, ScrollEventArgs e)
        {
            textBox1.Text = hScrollBar1.Value.ToString();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            if (curBitmap != null)
            {
                ImageProcessDll.MNN_Initialize(Application.StartupPath + "\\portrait_segment.mnn", 256, 0, 1);
                curBitmap = ImageProcessDll.PortraitEffect(srcBitmap, hScrollBar1.Value, 0);
                pictureBox1.Image = curBitmap;
                GC.Collect();
            }
        }

        private void button5_Click(object sender, EventArgs e)
        {
            if (curBitmap != null)
            {
                ImageProcessDll.MNN_Initialize(Application.StartupPath + "\\portrait_segment.mnn", 256, 0, 1);
                curBitmap = ImageProcessDll.PortraitEffect(srcBitmap, hScrollBar1.Value, 1);
                pictureBox1.Image = curBitmap;
                GC.Collect();
            }
        }

        private void button6_Click(object sender, EventArgs e)
        {
            if (curBitmap != null)
            {
                ImageProcessDll.MNN_Initialize(Application.StartupPath + "\\portrait_segment.mnn", 256, 0, 1);
                curBitmap = ImageProcessDll.PortraitEffect(srcBitmap, hScrollBar1.Value, 2);
                pictureBox1.Image = curBitmap;
                GC.Collect();
            }
        }

        private void button7_Click(object sender, EventArgs e)
        {
            if (curBitmap != null)
            {
                ImageProcessDll.MNN_Initialize(Application.StartupPath + "\\portrait_segment.mnn", 256, 0, 1);
                curBitmap = ImageProcessDll.PortraitEffect(srcBitmap, hScrollBar1.Value, 3);
                pictureBox1.Image = curBitmap;
                GC.Collect();
            }
        }
    }
}
