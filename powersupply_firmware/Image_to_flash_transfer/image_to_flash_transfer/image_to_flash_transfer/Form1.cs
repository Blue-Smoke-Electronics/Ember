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
using System.IO;

namespace image_to_flash_transfer
{
    public partial class Form1 : Form
    {

        SerialPort port;
        public Form1()
        {
            InitializeComponent();
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
        private void save_to_H_file(string name, Bitmap img)
        {
            
            using (StreamWriter writetext = new StreamWriter("../../../../../Pico/flash_data/" + name +".h"))
            {
                writetext.WriteLine("//Autogenereated by c# script");
                
                writetext.WriteLine("static const uint8_t " + name + "_SETUP [2] ={" +
                    img.Width.ToString() + "," +
                    img.Height.ToString() + "};");

                byte[] data = bitmap_to_data(img);
                writetext.WriteLine("static const uint8_t " + name+"_DATA ["+data.Length.ToString() + "] ={");
                writetext.Write("\t");

                int i = 0; 
                for (int x =0; x < img.Width; x++)
                {
                    for (int y =0; y < img.Height; y++)
                    {
                        for (int c = 0; c < 3; c++)
                        {
                            writetext.Write("" + data[i].ToString() + ",");
                            i++;
                        }
                    }
                    writetext.WriteLine();
                    writetext.Write("\t");
                }
                writetext.WriteLine("");
                writetext.WriteLine("};");  
            }
        }

        private void save_to_H_file(string name, Font font, Brush color, char [] includedChars)
        {
            includedChars = includedChars.Distinct().ToArray();
            Console.WriteLine(includedChars);

            using (StreamWriter writetext = new StreamWriter("../../../../../Pico/flash_data/" + name + ".h"))
            {
                // array whit all chars
                writetext.WriteLine("//Autogenereated by c# script");

                writetext.WriteLine("static const uint8_t " + name + "_SETUP [3] ={" + 
                    font.Size.ToString()+","+
                    font.Height.ToString() + ","
                    + includedChars.Length.ToString()+"};");

                writetext.Write("static const char " + name + "_CHARS ["+includedChars.Length.ToString()+"] ={");
                foreach (char c in includedChars)
                {
                    writetext.Write("'" + c.ToString() + "',");
                }
                writetext.WriteLine("};");


                // data array
                Bitmap bitmap = new Bitmap((int)font.Size, font.Height);
                Graphics graphics = Graphics.FromImage(bitmap);
                
                writetext.WriteLine("static const uint8_t " + name + "_DATA [" + 
                    (includedChars.Length* bitmap_to_data(bitmap).Length).ToString() + 
                    "] ={");



                foreach (char c in includedChars)
                {
                    writetext.Write("\t");
                    graphics.FillRectangle(Brushes.White, 0, 0, bitmap.Width, bitmap.Height);
                    graphics.DrawString(c.ToString(), font, color, new PointF(0, 0));
                    foreach (byte b in bitmap_to_data(bitmap)){
                        writetext.Write("" + b.ToString() + ",");
                    }
                    writetext.WriteLine("");
                }
                writetext.WriteLine("};");

            }
        }

        private void button14_Click(object sender, EventArgs e)
        {
            string[] files = Directory.GetFiles(@"../../../../", "*.png");
            foreach (var file in files)
            {
                Bitmap img = new Bitmap(file);
                Console.WriteLine(System.IO.Path.GetFileNameWithoutExtension(file));
                save_to_H_file(System.IO.Path.GetFileNameWithoutExtension(file), img);
            }

            save_to_H_file("smallFont", new Font("Consolas", 12),Brushes.Black, "VmA01923456789hms.% ".ToCharArray());
            save_to_H_file("smallFontRed", new Font("Consolas", 12), Brushes.Red, "Off".ToCharArray());
            save_to_H_file("bigFont", new Font("Consolas", 24), Brushes.Black, "1234567890. ".ToCharArray());
        }
    }
}
