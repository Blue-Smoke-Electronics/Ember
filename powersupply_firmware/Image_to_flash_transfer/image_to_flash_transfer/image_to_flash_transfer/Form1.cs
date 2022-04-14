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
                    data[i] = img.GetPixel(x, y).R;
                    data[i+1] = img.GetPixel(x, y).G;
                    data[i+2] = img.GetPixel(x, y).B;
                    i += 3; 
                }
            }
            i = 0; 
            while (i < img.Width * img.Height * 3)
            {
                port.Write(data, i, 1);
                string line = port.ReadLine();
                try
                {
                    int resived_cout = Int32.Parse(line); 
                    Console.WriteLine("sendt: " +i.ToString() +" resived: " + resived_cout.ToString());
                    if (resived_cout == i + 1)
                    {
                        i++;
                    }
                }
                catch
                {
                    continue; 
                }

            }
            /*
            byte[] data = new byte[3];
            for (int x = 0; x < img.Width; x++)
            {
                for (int y = 0; y < img.Height; y++)
                {
                    data[0] = img.GetPixel(x, y).R;
                    port.Write(data, 0, 1);
                    port.ReadLine();
                    data[1] = img.GetPixel(x, y).G;
                    port.Write(data, 0, 1);
                    port.ReadLine();
                    data[2] = img.GetPixel(x, y).B;
                    port.Write(data, 0, 1);
                    int resived_cout = Int32.Parse(port.ReadLine());

                    if ((x + 1) * (y + 1) * 3 != resived_cout)
                    {
                        throw new Exception("SEND ERROR -> resived counter do not match send couter - resived: "
                            + resived_cout.ToString() + " expected: " + (x + 1) * (y + 1) * 3);
                    }
                    Console.WriteLine(x.ToString() + " " + y.ToString());
                }
            }*/

        }
    }
}
