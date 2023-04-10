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
                    int[] srcFacePoints = new int[] { 170, 205, 170, 229, 174, 252, 179, 276, 186, 298, 197, 317, 211, 332, 227, 345, 245, 355, 266, 360, 286, 356, 304, 345, 318, 331, 331, 317, 343, 298, 350, 277, 355, 253, 358, 230, 359, 206, 184, 191, 194, 181, 207, 178, 221, 179, 235, 182, 245, 193, 232, 192, 220, 189, 208, 188, 196, 189, 287, 193, 296, 183, 310, 180, 324, 179, 337, 182, 346, 189, 334, 190, 323, 189, 311, 190, 299, 193, 202, 216, 206, 211, 213, 208, 221, 208, 229, 209, 235, 214, 240, 222, 233, 223, 226, 224, 219, 225, 212, 223, 206, 220, 291, 222, 295, 214, 302, 209, 310, 208, 318, 209, 324, 211, 328, 217, 324, 221, 318, 224, 311, 225, 304, 225, 297, 223, 255, 218, 255, 238, 252, 259, 241, 269, 245, 283, 261, 284, 272, 284, 287, 283, 291, 270, 281, 259, 277, 238, 276, 218, 235, 308, 247, 305, 257, 302, 266, 304, 275, 302, 285, 305, 295, 309, 287, 317, 278, 323, 266, 325, 254, 323, 244, 317, 240, 309, 254, 310, 266, 312, 278, 310, 291, 309, 278, 311, 266, 312, 254, 311, 222, 215, 309, 216, 266, 217, 267, 254, 267, 271, 267, 284 };
                    Bitmap mask = new Bitmap(Application.StartupPath + "\\" + "shadow_l_mask.png");
                    int[] maskKeyPointsleft = new int[] { 201, 217, 219, 209, 238,221, 218, 226};
                    int[] maskKeyPointsright = new int[] {292, 221, 311, 209, 329, 217, 314, 225 };
                    int ratio = Convert.ToInt32(textBox1.Text);
                    DateTime start = DateTime.Now;
                    //left eye
                    curBitmap = ImageProcessDll.EyeShadow(srcBitmap, srcFacePoints, mask, maskKeyPointsleft,true, 1, ratio);
                    mask = new Bitmap(Application.StartupPath + "\\" + "shadow_l_mask2.png");
                    curBitmap = ImageProcessDll.EyeShadow(curBitmap, srcFacePoints, mask, maskKeyPointsleft, true, 0, ratio);
                    //right eye
                    mask = new Bitmap(Application.StartupPath + "\\" + "shadow_r_mask.png");
                    curBitmap = ImageProcessDll.EyeShadow(curBitmap, srcFacePoints, mask, maskKeyPointsright, false, 1, ratio);
                    mask = new Bitmap(Application.StartupPath + "\\" + "shadow_r_mask2.png");
                    curBitmap = ImageProcessDll.EyeShadow(curBitmap, srcFacePoints, mask, maskKeyPointsright, false, 0, ratio);
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
            srcBitmap = new Bitmap(Application.StartupPath + "\\Test.bmp");
            pictureBox1.Image = srcBitmap;
        }

        private void button2_Click_1(object sender, EventArgs e)
        {
            if (pictureBox1.Image != null)
                SaveFile();
        }
        //eyeline
        private void button3_Click(object sender, EventArgs e)
        {
            if (pictureBox1.Image != null)
            {
                try
                {
                    int[] srcFacePoints = new int[] { 170, 205, 170, 229, 174, 252, 179, 276, 186, 298, 197, 317, 211, 332, 227, 345, 245, 355, 266, 360, 286, 356, 304, 345, 318, 331, 331, 317, 343, 298, 350, 277, 355, 253, 358, 230, 359, 206, 184, 191, 194, 181, 207, 178, 221, 179, 235, 182, 245, 193, 232, 192, 220, 189, 208, 188, 196, 189, 287, 193, 296, 183, 310, 180, 324, 179, 337, 182, 346, 189, 334, 190, 323, 189, 311, 190, 299, 193, 202, 216, 206, 211, 213, 208, 221, 208, 229, 209, 235, 214, 240, 222, 233, 223, 226, 224, 219, 225, 212, 223, 206, 220, 291, 222, 295, 214, 302, 209, 310, 208, 318, 209, 324, 211, 328, 217, 324, 221, 318, 224, 311, 225, 304, 225, 297, 223, 255, 218, 255, 238, 252, 259, 241, 269, 245, 283, 261, 284, 272, 284, 287, 283, 291, 270, 281, 259, 277, 238, 276, 218, 235, 308, 247, 305, 257, 302, 266, 304, 275, 302, 285, 305, 295, 309, 287, 317, 278, 323, 266, 325, 254, 323, 244, 317, 240, 309, 254, 310, 266, 312, 278, 310, 291, 309, 278, 311, 266, 312, 254, 311, 222, 215, 309, 216, 266, 217, 267, 254, 267, 271, 267, 284 };
                    Bitmap mask = new Bitmap(Application.StartupPath + "\\" + "eyeline_l_mask.png");
                    int[] maskKeyPointsleft = new int[] { 201, 217, 219, 209, 238, 221, 218, 226 };
                    int[] maskKeyPointsright = new int[] { 292, 221, 311, 209, 329, 217, 314, 225 };
                    int ratio = Convert.ToInt32(textBox2.Text);
                    DateTime start = DateTime.Now;
                    //left eye
                    curBitmap = ImageProcessDll.EyeLine(srcBitmap, srcFacePoints, mask, maskKeyPointsleft, true, ratio);
                    ////right eye
                    mask = new Bitmap(Application.StartupPath + "\\" + "eyeline_r_mask.png");
                    curBitmap = ImageProcessDll.EyeLine(curBitmap, srcFacePoints, mask, maskKeyPointsright, false, ratio);
                    DateTime end = DateTime.Now;
                    label4.Text = "Time cost: " + (end - start).ToString();
                    pictureBox1.Image = curBitmap;
                }
                catch (Exception exp)
                { MessageBox.Show("操作错误！"); };

            }
        }
        //eye lash
        private void button4_Click(object sender, EventArgs e)
        {
            if (pictureBox1.Image != null)
            {
                try
                {
                    int[] srcFacePoints = new int[] { 170, 205, 170, 229, 174, 252, 179, 276, 186, 298, 197, 317, 211, 332, 227, 345, 245, 355, 266, 360, 286, 356, 304, 345, 318, 331, 331, 317, 343, 298, 350, 277, 355, 253, 358, 230, 359, 206, 184, 191, 194, 181, 207, 178, 221, 179, 235, 182, 245, 193, 232, 192, 220, 189, 208, 188, 196, 189, 287, 193, 296, 183, 310, 180, 324, 179, 337, 182, 346, 189, 334, 190, 323, 189, 311, 190, 299, 193, 202, 216, 206, 211, 213, 208, 221, 208, 229, 209, 235, 214, 240, 222, 233, 223, 226, 224, 219, 225, 212, 223, 206, 220, 291, 222, 295, 214, 302, 209, 310, 208, 318, 209, 324, 211, 328, 217, 324, 221, 318, 224, 311, 225, 304, 225, 297, 223, 255, 218, 255, 238, 252, 259, 241, 269, 245, 283, 261, 284, 272, 284, 287, 283, 291, 270, 281, 259, 277, 238, 276, 218, 235, 308, 247, 305, 257, 302, 266, 304, 275, 302, 285, 305, 295, 309, 287, 317, 278, 323, 266, 325, 254, 323, 244, 317, 240, 309, 254, 310, 266, 312, 278, 310, 291, 309, 278, 311, 266, 312, 254, 311, 222, 215, 309, 216, 266, 217, 267, 254, 267, 271, 267, 284 };
                    Bitmap mask = new Bitmap(Application.StartupPath + "\\" + "eyelash_l_mask.png");
                    int[] maskKeyPointsleft = new int[] { 201, 217, 219, 209, 238, 221, 218, 226 };
                    int[] maskKeyPointsright = new int[] { 292, 221, 311, 209, 329, 217, 314, 225 };
                    int ratio = Convert.ToInt32(textBox3.Text);
                    DateTime start = DateTime.Now;
                    //left eye
                    curBitmap = ImageProcessDll.EyeLash(srcBitmap, srcFacePoints, mask, maskKeyPointsleft, true, ratio);
                    //right eye
                    mask = new Bitmap(Application.StartupPath + "\\" + "eyelash_r_mask.png");
                    curBitmap = ImageProcessDll.EyeLash(curBitmap, srcFacePoints, mask, maskKeyPointsright, false, ratio);
                    DateTime end = DateTime.Now;
                    label4.Text = "Time cost: " + (end - start).ToString();
                    pictureBox1.Image = curBitmap;
                }
                catch (Exception exp)
                { MessageBox.Show("操作错误！"); };

            }
        }
        //eyelid
        private void button5_Click(object sender, EventArgs e)
        {
            if (pictureBox1.Image != null)
            {
                try
                {
                    int[] srcFacePoints = new int[] { 170, 205, 170, 229, 174, 252, 179, 276, 186, 298, 197, 317, 211, 332, 227, 345, 245, 355, 266, 360, 286, 356, 304, 345, 318, 331, 331, 317, 343, 298, 350, 277, 355, 253, 358, 230, 359, 206, 184, 191, 194, 181, 207, 178, 221, 179, 235, 182, 245, 193, 232, 192, 220, 189, 208, 188, 196, 189, 287, 193, 296, 183, 310, 180, 324, 179, 337, 182, 346, 189, 334, 190, 323, 189, 311, 190, 299, 193, 202, 216, 206, 211, 213, 208, 221, 208, 229, 209, 235, 214, 240, 222, 233, 223, 226, 224, 219, 225, 212, 223, 206, 220, 291, 222, 295, 214, 302, 209, 310, 208, 318, 209, 324, 211, 328, 217, 324, 221, 318, 224, 311, 225, 304, 225, 297, 223, 255, 218, 255, 238, 252, 259, 241, 269, 245, 283, 261, 284, 272, 284, 287, 283, 291, 270, 281, 259, 277, 238, 276, 218, 235, 308, 247, 305, 257, 302, 266, 304, 275, 302, 285, 305, 295, 309, 287, 317, 278, 323, 266, 325, 254, 323, 244, 317, 240, 309, 254, 310, 266, 312, 278, 310, 291, 309, 278, 311, 266, 312, 254, 311, 222, 215, 309, 216, 266, 217, 267, 254, 267, 271, 267, 284 };
                    Bitmap mask = new Bitmap(Application.StartupPath + "\\" + "eyelid_l_mask.png");
                    int[] maskKeyPointsleft = new int[] { 201, 217, 219, 209, 238, 221, 218, 226 };
                    int[] maskKeyPointsright = new int[] { 292, 221, 311, 209, 329, 217, 314, 225 };
                    int ratio = Convert.ToInt32(textBox4.Text);
                    DateTime start = DateTime.Now;
                    //left eye
                    curBitmap = ImageProcessDll.EyeLid(srcBitmap, srcFacePoints, mask, maskKeyPointsleft, true, ratio);
                    //right eye
                    mask = new Bitmap(Application.StartupPath + "\\" + "eyelid_r_mask.png");
                    curBitmap = ImageProcessDll.EyeLid(curBitmap, srcFacePoints, mask, maskKeyPointsright, false, ratio);
                    DateTime end = DateTime.Now;
                    label4.Text = "Time cost: " + (end - start).ToString();
                    pictureBox1.Image = curBitmap;
                }
                catch (Exception exp)
                { MessageBox.Show("操作错误！"); };

            }
            
        }

        private void button6_Click(object sender, EventArgs e)
        {
            if (pictureBox1.Image != null)
            {
                try
                {
                    int[] srcFacePoints = new int[] { 170, 205, 170, 229, 174, 252, 179, 276, 186, 298, 197, 317, 211, 332, 227, 345, 245, 355, 266, 360, 286, 356, 304, 345, 318, 331, 331, 317, 343, 298, 350, 277, 355, 253, 358, 230, 359, 206, 184, 191, 194, 181, 207, 178, 221, 179, 235, 182, 245, 193, 232, 192, 220, 189, 208, 188, 196, 189, 287, 193, 296, 183, 310, 180, 324, 179, 337, 182, 346, 189, 334, 190, 323, 189, 311, 190, 299, 193, 202, 216, 206, 211, 213, 208, 221, 208, 229, 209, 235, 214, 240, 222, 233, 223, 226, 224, 219, 225, 212, 223, 206, 220, 291, 222, 295, 214, 302, 209, 310, 208, 318, 209, 324, 211, 328, 217, 324, 221, 318, 224, 311, 225, 304, 225, 297, 223, 255, 218, 255, 238, 252, 259, 241, 269, 245, 283, 261, 284, 272, 284, 287, 283, 291, 270, 281, 259, 277, 238, 276, 218, 235, 308, 247, 305, 257, 302, 266, 304, 275, 302, 285, 305, 295, 309, 287, 317, 278, 323, 266, 325, 254, 323, 244, 317, 240, 309, 254, 310, 266, 312, 278, 310, 291, 309, 278, 311, 266, 312, 254, 311, 222, 215, 309, 216, 266, 217, 267, 254, 267, 271, 267, 284 };
                    Bitmap mask = new Bitmap(Application.StartupPath + "\\" + "eyebrow_l_mask.png");
                    int[] maskKeyPointsleft = new int[] { 184, 189, 220, 185, 245, 193 };
                    int[] maskKeyPointsright = new int[] { 287, 193, 311, 185, 346, 188};
                    int ratio = Convert.ToInt32(textBox5.Text);
                    DateTime start = DateTime.Now;
                    //left eye
                    curBitmap = ImageProcessDll.EyeBrow(srcBitmap, srcFacePoints, mask, maskKeyPointsleft, true, ratio);
                    //right eye
                    mask = new Bitmap(Application.StartupPath + "\\" + "eyebrow_r_mask.png");
                    curBitmap = ImageProcessDll.EyeBrow(curBitmap, srcFacePoints, mask, maskKeyPointsright, false, ratio);
                    DateTime end = DateTime.Now;
                    label4.Text = "Time cost: " + (end - start).ToString();
                    pictureBox1.Image = curBitmap;
                }
                catch (Exception exp)
                { MessageBox.Show("操作错误！"); };

            }
        }
        private void button7_Click(object sender, EventArgs e)
        {
            if (pictureBox1.Image != null)
            {
                try
                {
                    int[] srcFacePoints = new int[] { 170, 205, 170, 229, 174, 252, 179, 276, 186, 298, 197, 317, 211, 332, 227, 345, 245, 355, 266, 360, 286, 356, 304, 345, 318, 331, 331, 317, 343, 298, 350, 277, 355, 253, 358, 230, 359, 206, 184, 191, 194, 181, 207, 178, 221, 179, 235, 182, 245, 193, 232, 192, 220, 189, 208, 188, 196, 189, 287, 193, 296, 183, 310, 180, 324, 179, 337, 182, 346, 189, 334, 190, 323, 189, 311, 190, 299, 193, 202, 216, 206, 211, 213, 208, 221, 208, 229, 209, 235, 214, 240, 222, 233, 223, 226, 224, 219, 225, 212, 223, 206, 220, 291, 222, 295, 214, 302, 209, 310, 208, 318, 209, 324, 211, 328, 217, 324, 221, 318, 224, 311, 225, 304, 225, 297, 223, 255, 218, 255, 238, 252, 259, 241, 269, 245, 283, 261, 284, 272, 284, 287, 283, 291, 270, 281, 259, 277, 238, 276, 218, 235, 308, 247, 305, 257, 302, 266, 304, 275, 302, 285, 305, 295, 309, 287, 317, 278, 323, 266, 325, 254, 323, 244, 317, 240, 309, 254, 310, 266, 312, 278, 310, 291, 309, 278, 311, 266, 312, 254, 311, 222, 215, 309, 216, 266, 217, 267, 254, 267, 271, 267, 284 };
                    Bitmap mask = new Bitmap(Application.StartupPath + "\\" + "steronose_mask.png");
                    int[] maskKeyPoints = new int[] { 265, 219, 242, 270, 288, 271 };
                    int ratio = Convert.ToInt32(textBox4.Text);
                    DateTime start = DateTime.Now;
                    curBitmap = ImageProcessDll.SteroNose(srcBitmap, srcFacePoints, mask, maskKeyPoints, ratio);
                    DateTime end = DateTime.Now;
                    label4.Text = "Time cost: " + (end - start).ToString();
                    pictureBox1.Image = curBitmap;
                }
                catch (Exception exp)
                { MessageBox.Show("操作错误！"); };

            }
        }


        
       

    }
}
