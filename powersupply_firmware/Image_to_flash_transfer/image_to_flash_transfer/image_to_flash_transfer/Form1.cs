using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;

namespace image_to_flash_transfer
{
    public partial class Form1 : Form
    {

        SerialPort port;
        public Form1()
        {
            InitializeComponent();
            port = new SerialPort("COM22", 115200, Parity.None, 8, StopBits.One);
            port.RtsEnable = true;
            port.DtrEnable = true; 
            port.Close();
            port.Open();
        }

        private byte[] bitmap_to_data(Bitmap img)
        {
            byte[] data = new byte[img.Width * img.Height * 3];
            int i = 0;
            for (int y = 0; y < img.Height; y++)
            {
                for (int x = 0; x < img.Width; x++)
                {
                    data[i] = img.GetPixel(x, y).B;
                    data[i + 1] = img.GetPixel(x, y).G;
                    data[i + 2] = img.GetPixel(x, y).R;
                    i += 3;
                }
            }

            return data; 
        }

        private void load_to_flash(string loadComand,byte[] data)
        {
            port.Write(loadComand +"\n");
            string line = port.ReadLine();
            while (!line.Contains(loadComand))
            {
                line = port.ReadLine(); 
            }
            int i = 0; 
            while (i < data.Length)
            {
                port.Write(data, i, 1);
                line = port.ReadLine();
                try
                {
                    int resived_cout = Int32.Parse(line);
                    Console.WriteLine("sendt: " + i.ToString() + " resived: " + resived_cout.ToString() + " of: " + data.Length.ToString());
                    if (resived_cout == i)
                    {
                        i++;
                    }
                }
                catch
                {
                    continue;
                }

            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Bitmap img = new Bitmap("../../../../logo.png");
            load_to_flash("FLASHLOADLOGO", bitmap_to_data(img));
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Bitmap img = new Bitmap("../../../../bootscreen.png");
            load_to_flash("FLASHLOADBOOTSCREEN", bitmap_to_data(img));

        }

        private byte[] font_to_data(Font font)
        {
            Bitmap bitmap = new Bitmap((int)font.Size, font.Height);
            Graphics graphics = Graphics.FromImage(bitmap);
            
            Console.WriteLine(bitmap.GetPixel(0, 0));

            byte[] data = { };
            for (char c = (char)32; c <= 126; c++)
            {
                graphics.FillRectangle(Brushes.White, 0, 0, bitmap.Width, bitmap.Height);
                graphics.DrawString(c.ToString(), font, Brushes.Black, new PointF(0, 0));
                pictureBox1.Image = bitmap;
                data = data.Concat(bitmap_to_data(bitmap)).ToArray();

            }
            return data; 
        }

        private void button3_Click(object sender, EventArgs e)
        {
            load_to_flash("FLASHLOADSMALLFONT", font_to_data(new Font("Consolas", 12)));

        }

        private void button4_Click(object sender, EventArgs e)
        {
            load_to_flash("FLASHLOADBIGFONT", font_to_data(new Font("Consolas", 24)));
        }

        private void button5_Click(object sender, EventArgs e)
        {
            Bitmap img = new Bitmap("../../../../output_on_symbol.png");
            load_to_flash("FLASHLOADOUTPUTONSYMBOL", bitmap_to_data(img));
        }

        private void button6_Click(object sender, EventArgs e)
        {
            Bitmap img = new Bitmap("../../../../output_off_symbol.png");
            load_to_flash("FLASHLOADOUTPUTOFFSYMBOL", bitmap_to_data(img));
        }

        private void button7_Click(object sender, EventArgs e)
        {
            Bitmap img = new Bitmap("../../../../battery_low.png");
            load_to_flash("FLASHLOADBATTERYSYMBOLLOW", bitmap_to_data(img));
        }

        private void button8_Click(object sender, EventArgs e)
        {
            Bitmap img = new Bitmap("../../../../battery_med_low.png");
            load_to_flash("FLASHLOADBATTERYSYMBOLMEDLOW", bitmap_to_data(img));
        }

        private void button9_Click(object sender, EventArgs e)
        {
            Bitmap img = new Bitmap("../../../../battery_med_high.png");
            load_to_flash("FLASHLOADBATTERYSYMBOLMEDHIGH", bitmap_to_data(img));
        }

        private void button10_Click(object sender, EventArgs e)
        {
            Bitmap img = new Bitmap("../../../../battery_high.png");
            load_to_flash("FLASHLOADBATTERYSYMBOLHIGH", bitmap_to_data(img));
        }

        private void button11_Click(object sender, EventArgs e)
        {
            Bitmap img = new Bitmap("../../../../battery_empty.png");
            load_to_flash("FLASHLOADBATTERYSYMBOLEMPTY", bitmap_to_data(img));
        }

        private void button12_Click(object sender, EventArgs e)
        {
            Bitmap img = new Bitmap("../../../../selectedmarker.png");
            load_to_flash("FLASHLOADSELECTEDMARKER", bitmap_to_data(img));
        }

        private void button13_Click(object sender, EventArgs e)
        {
            Bitmap img = new Bitmap("../../../../charging_symbol.png");
            load_to_flash("FLASHLOADBATTERYCHARGINGSYMBOL", bitmap_to_data(img));
        }
    }
}
