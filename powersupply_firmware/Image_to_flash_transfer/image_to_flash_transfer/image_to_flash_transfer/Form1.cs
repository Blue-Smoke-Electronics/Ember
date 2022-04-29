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
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Bitmap img = new Bitmap("../../../../logo.png");
            
            
            SerialPort port = new SerialPort("COM15", 115200, Parity.None, 8, StopBits.One);
            port.RtsEnable = true;
            port.DtrEnable = true;

            port.Close();
            port.Open();


            byte[] data = new byte[img.Width* img.Height*3];
            int i = 0; 
            for (int y = 0; y < img.Height; y++)
            {
                for (int x = 0; x < img.Width; x++)
                {
                    data[i] = img.GetPixel(x, y).B;
                    data[i+1] = img.GetPixel(x, y).G;
                    data[i+2] = img.GetPixel(x, y).R;
                    i += 3; 
                }
            }
            i = 0;

            port.Write("FLASHLOADLOGO\n");
            string line = port.ReadLine();
            while (i < img.Width * img.Height * 3)
            {
                port.Write(data, i, 1);
                line = port.ReadLine();
                try
                {
                    int resived_cout = Int32.Parse(line); 
                    Console.WriteLine("sendt: " +i.ToString() +" resived: " + resived_cout.ToString() + " of: " + (img.Width * img.Height * 3).ToString());
                    if (resived_cout == i )
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

        private void button2_Click(object sender, EventArgs e)
        {
            Bitmap img = new Bitmap("../../../../bootscreen.png");


            SerialPort port = new SerialPort("COM15", 115200, Parity.None, 8, StopBits.One);
            port.RtsEnable = true;
            port.DtrEnable = true;

            port.Close();
            port.Open();


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
            i = 0;

            port.Write("FLASHLOADBOOTSCREEN\n");
            string line = port.ReadLine();
            while (i < img.Width * img.Height * 3)
            {
                port.Write(data, i, 1);
                line = port.ReadLine();
                try
                {
                    int resived_cout = Int32.Parse(line);
                    Console.WriteLine("sendt: " + i.ToString() + " resived: " + resived_cout.ToString() + " of: " + (img.Width * img.Height * 3).ToString());
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
    }
}
