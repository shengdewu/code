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
                    facePoints = GetPoints(curFileName.Substring(0, curFileName.Length - 4) + ".txt");
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
                try
                {

                    DateTime start = DateTime.Now;
                    curBitmap = ImageProcessDll.AutoFaceLift(srcBitmap, facePoints, hScrollBar1.Value);
                    DateTime end = DateTime.Now;
                    label4.Text = "Time cost: " + (end - start).ToString();
                    pictureBox1.Image = curBitmap;
                }
                catch (Exception exp)
                { MessageBox.Show("请输入点位文件！"); };

            }
        }

        private void hScrollBar1_Scroll(object sender, ScrollEventArgs e)
        {
            textBox1.Text = hScrollBar1.Value.ToString();
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

        private void Form1_Load(object sender, EventArgs e)
        {
            srcBitmap = new Bitmap(Application.StartupPath + "\\Model.jpg");
            curBitmap = new Bitmap(srcBitmap);
            facePoints = new int[202] { 384, 435, 384, 487, 393, 536, 407, 586, 425, 633, 451, 675, 483, 707, 515, 741, 554, 765, 601, 775, 649, 763, 688, 734, 719, 701, 748, 665, 769, 620, 782, 574, 791, 523, 795, 474, 794, 424, 410, 403, 432, 383, 460, 376, 493, 379, 521, 385, 544, 406, 516, 406, 489, 401, 463, 399, 437, 401, 635, 405, 658, 383, 686, 375, 718, 370, 747, 375, 770, 394, 743, 394, 717, 394, 690, 398, 664, 404, 452, 457, 462, 447, 476, 441, 493, 440, 511, 442, 526, 451, 537, 465, 523, 469, 508, 472, 492, 474, 476, 472, 463, 466, 651, 462, 661, 447, 676, 439, 693, 436, 710, 436, 725, 440, 736, 449, 726, 458, 713, 464, 696, 468, 681, 467, 666, 465, 567, 458, 566, 502, 560, 545, 536, 566, 543, 595, 582, 604, 609, 603, 648, 594, 653, 564, 627, 544, 619, 501, 617, 457, 520, 645, 547, 644, 573, 642, 596, 646, 619, 642, 644, 642, 671, 644, 649, 668, 627, 684, 595, 689, 563, 683, 540, 668, 529, 649, 565, 656, 597, 662, 626, 656, 661, 648, 627, 658, 596, 664, 564, 658, 496, 455, 690, 449, 592, 456, 593, 537, 593, 576, 595, 604 };
            pictureBox1.Image = curBitmap;
        }


        
       

    }
}
