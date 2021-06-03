using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace ImageProcess
{
    //剪切界面
    public partial class FormCut : Form
    {

        bool isClip = false;//标示变量
        Rectangle rec = new Rectangle(new Point(0, 0), new Size(0, 0));//定义矩形
        int result = 0;
        Point startp, endp;
        Graphics gra;


        public FormCut()
        {
            InitializeComponent();
        }
        public FormCut(Bitmap bitmap)
        {
            InitializeComponent();
            pictureBox1.Image = bitmap;
            Invalidate();
        }

        private void pictureBox1_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                if ((isClip = !isClip) == true)
                {
                    startp = new Point(e.X, e.Y);
                    endp = new Point(e.X, e.Y);
                }
            }
        }

        private void pictureBox1_MouseMove(object sender, MouseEventArgs e)
        {
            Graphics gt = this.CreateGraphics();
            Pen mypen = new Pen(Color.Black, 1);
            if (isClip == true)
            {
                gt.DrawRectangle(mypen, startp.X, startp.Y, e.X - startp.X, e.Y - startp.Y);
                //绘制矩形
            }
        }

        private void pictureBox1_MouseUp(object sender, MouseEventArgs e)
        {
            if (isClip == true)
            {
                isClip = false;
                gra = pictureBox1.CreateGraphics();
                gra.DrawRectangle(new Pen(Color.Black, 1), startp.X, startp.Y, e.X - startp.X, e.Y - startp.Y);
                endp.X = e.X;
                endp.Y = e.Y;
                if(result==0)
                rec = new Rectangle(startp.X, startp.Y, e.X - startp.X, e.Y - startp.Y);
                result = 1;
            }
        }

        public Rectangle retRect() {
       
            return rec;
        }

        private void FormCut_Load(object sender, EventArgs e)
        {

        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {

        }

        private void pictureBox1_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            this.DialogResult = DialogResult.OK;
        }


        
    }
}
