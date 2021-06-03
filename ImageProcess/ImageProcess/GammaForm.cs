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
    public partial class GammaForm : Form
    {
        Bitmap curBitmap;
        public GammaForm()
        {
            InitializeComponent();
        }

        public GammaForm(Bitmap bitmap)
        {
            InitializeComponent();
            curBitmap = bitmap;
            pictureBox1.Image = curBitmap;
        }

        public void SetGamma(double red, double green, double blue)
        {
            Color c;
            byte[] redgamma = CreateGammaArray(red);
            byte[] greengamma = CreateGammaArray(green);
            byte[] bluegamma = CreateGammaArray(blue);
            for (int i = 0; i < curBitmap.Width; i++) {
                for (int j = 0; j < curBitmap.Height; j++) {
                    c = curBitmap.GetPixel(i, j);
                    curBitmap.SetPixel(i, j,Color.FromArgb(redgamma[c.R],greengamma[c.G],bluegamma[c.B]));
                }
            }
            pictureBox1.Image = (Bitmap)curBitmap.Clone();
        }

        private byte[] CreateGammaArray(double color) {
            byte[] gammaArray = new byte[256];
            for (int i = 0; i < 256; i++) {
                gammaArray[i] = (byte)Math.Min(255, (int)((255.0 * Math.Pow(i / 255.0, 1.0 / color)) + 0.5));
            }
            return gammaArray;
        }

        private void hScrollBar1_Scroll(object sender, ScrollEventArgs e)
        {
            label5.Text = string.Format("Red={0},Green={1},Blue={2}", hScrollBar1.Value, hScrollBar2.Value, hScrollBar3.Value);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            SetGamma(hScrollBar1.Value, hScrollBar2.Value, hScrollBar3.Value);
        }

        private void hScrollBar2_Scroll(object sender, ScrollEventArgs e)
        {
            label5.Text = string.Format("Red={0},Green={1},Blue={2}", hScrollBar1.Value, hScrollBar2.Value, hScrollBar3.Value);
        }

        private void hScrollBar3_Scroll(object sender, ScrollEventArgs e)
        {
            label5.Text = string.Format("Red={0},Green={1},Blue={2}", hScrollBar1.Value, hScrollBar2.Value, hScrollBar3.Value);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
        }

        public Bitmap retBitmap() {
            return curBitmap;
        }

        private void GammaForm_Load(object sender, EventArgs e)
        {

        }
    }

    
}
