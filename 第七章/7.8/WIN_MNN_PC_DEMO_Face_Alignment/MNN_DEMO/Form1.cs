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
                    curBitmap = new Bitmap(curBitmap, curBitmap.Width / 2, curBitmap.Height / 2);
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
                int[] faceRectsInfo = new int[17];
                float[] facePoints = new float[1 + 4 * 202];
                curBitmap = ImageProcessDll.FaceDetection(srcBitmap, faceRectsInfo);
                facePoints[0] = faceRectsInfo[0];
                if (faceRectsInfo[0] > 0)
                {
                    Graphics g = Graphics.FromImage(curBitmap);
                    for (int j = 0; j < faceRectsInfo[0]; j++)
                    {
                        g.DrawRectangle(new Pen(Color.Green, 4), new Rectangle(faceRectsInfo[1 + j * 4], faceRectsInfo[1 + j * 4 + 1], faceRectsInfo[1 + j * 4 + 2], faceRectsInfo[1 + j * 4 + 3]));
                        float[] facePointsSingle = new float[202];
                        int[] faceRect = new int[] { faceRectsInfo[1 + j * 4], faceRectsInfo[1 + j * 4 + 1], faceRectsInfo[1 + j * 4 + 2], faceRectsInfo[1 + j * 4 + 3]};
                        ImageProcessDll.FaceAlignment(srcBitmap, faceRect, facePointsSingle);
                        //Font drawFont = new Font("Arial", 8);
                        //SolidBrush drawBrush = new SolidBrush(Color.Black);                     
                        for (int i = 0; i < 101; i++)
                        {
                            g.DrawRectangle(new Pen(Color.Red, 2), new Rectangle((int)(facePointsSingle[2 * i] * srcBitmap.Width) - 1, (int)(facePointsSingle[2 * i + 1] * srcBitmap.Height) - 1, 2, 2));
                            facePoints[1 + j * 202 + 2 * i] = facePointsSingle[2 * i];
                            facePoints[1 + j * 202 + 2 * i + 1] = facePointsSingle[2 * i + 1];
                            //g.DrawString(i.ToString(), drawFont, drawBrush, facePointsSingle[2 * i] * srcBitmap.Width - 3, facePointsSingle[2 * i + 1] * srcBitmap.Height - 3);
                        }
                    }
                    
                    g.Dispose();
                }
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

        private void Form1_Load(object sender, EventArgs e)
        {
            ImageProcessDll.MNN_Initialize(Application.StartupPath + "\\faceskin_segment.mnn", Application.StartupPath + "\\face_alignment.mnn", 256, 0, 1);
        }
    }
}
