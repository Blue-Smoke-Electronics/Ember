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

namespace Powersupply_automatic_tests
{
    public partial class Form1 : Form
    {

        SerialPort portBlueSmoke;
        SerialPort portKoradPSU;
        SerialPort portKoradLoad;
        public Form1()
        {
            InitializeComponent();

            portBlueSmoke = new SerialPort("COM3", 9600, Parity.None, 8, StopBits.One);
            portKoradPSU = new SerialPort("COM15", 9600, Parity.None, 8, StopBits.One);
            portKoradLoad = new SerialPort("COM17", 9600, Parity.None, 8, StopBits.One);
        }


        private void button1_Click(object sender, EventArgs e)
        {


            try
            {
                portBlueSmoke.Close();
                portBlueSmoke.Open();
                isConnectedBlueSmoek.Checked = true;
            }
            catch
            {
                isConnectedBlueSmoek.Checked = false;
            }

            try
            {
                portKoradPSU.Close();
                portKoradPSU.Open();
                isConnectedKonradPSU.Checked = true;
            }
            catch(Exception ex)
            {
                Console.WriteLine(ex.Message);
                isConnectedKonradPSU.Checked = false;
            }

            try
            {
                portKoradLoad.Close();
                portKoradLoad.Open();
                isConnectedKoradLoad.Checked = true;
            }
            catch ( Exception ex )
            {
                Console.WriteLine(ex.Message);
                isConnectedKoradLoad.Checked = false;
            }

        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            portBlueSmoke.Close();
            portKoradPSU.Close();
            portKoradLoad.Close();
        }

        private void buttonVoltageTest_Click(object sender, EventArgs e)
        {
            portKoradLoad.WriteLine(":FUNC RES\n:RES 1000OHM\n:INP ON");
            portBlueSmoke.WriteLine("ISET 0.1\n");
            System.Threading.Thread.Sleep(1000);

            for (float v =0.0f; v < 12.01f; v+= 0.5f)
            {
                Console.WriteLine("VSET " + v.ToString() + "\n");
                portBlueSmoke.WriteLine("VSET "+v.ToString() +"\n");
                System.Threading.Thread.Sleep(1000);
                portKoradLoad.WriteLine(":MEAS:VOLT?\n");
                string voltMeasStr = portKoradLoad.ReadLine();
                float voltMeas = float.Parse(voltMeasStr.Split('V')[0]);

                Console.WriteLine("diff: " + (Math.Abs(v - voltMeas)).ToString());
                if (Math.Abs(v - voltMeas) > 0.1)
                {
                    MessageBox.Show("ERROR to hight at " + v.ToString()+"V");
                }

            }



            portBlueSmoke.WriteLine("VSET 0\n");
            portKoradLoad.WriteLine(":INP OFF\n");
        }

        private void buttonCurrentTest_Click(object sender, EventArgs e)
        {
            
            portBlueSmoke.WriteLine("VSET 3.3\n");
            portBlueSmoke.WriteLine("ISET 0\n");
            portKoradLoad.WriteLine(":RES 0OHM\n:INP ON");
            System.Threading.Thread.Sleep(1000);


            for (float i = 0.0f; i < 1.01f; i += 0.1f)
            {
                Console.WriteLine("ISET " + i.ToString() + "\n");
                portBlueSmoke.WriteLine("iSET " + i.ToString() + "\n");
                System.Threading.Thread.Sleep(1000);
                portKoradLoad.WriteLine(":MEAS:CURR?\n");
                string ampMeasStr = portKoradLoad.ReadLine();
                float ampMeas = float.Parse(ampMeasStr.Split('A')[0]);

                Console.WriteLine("diff: " + (Math.Abs(i - ampMeas)).ToString());
                if (Math.Abs(i - ampMeas) > 0.02)
                {
                    MessageBox.Show("ERROR to hight at " + i.ToString() + "A");
                }

            }


            portBlueSmoke.WriteLine("VSET 0\n");
            portKoradLoad.WriteLine(":INP OFF\n");
        }
    }
}
