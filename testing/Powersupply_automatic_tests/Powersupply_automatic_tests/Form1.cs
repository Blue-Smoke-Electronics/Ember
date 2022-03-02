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

        Pbp pbp;
        KonradPsu konradPsu;
        KonradLoad konradLoad;
        public Form1()
        {
            InitializeComponent();

            pbp = new Pbp();
            konradPsu = new KonradPsu();
            KonradLoad = new KonradLoad();
            
        }


        private void button1_Click(object sender, EventArgs e)
        {
            isConnectedBlueSmoek.Checked = pbp.ConnectToSerial();
            isConnectedKoradLoad.Checked = konradPsu.ConnectToSerial();

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
            timer1.Start();

        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
           
            portKoradLoad.Close();
        }

        private void buttonVoltageTest_Click(object sender, EventArgs e)
        {
            portKoradLoad.WriteLine(":FUNC RES\n:RES 1000OHM\n:INP ON");
            pbp.Iset(0.1f);
            System.Threading.Thread.Sleep(1000);

            for (float v =0.0f; v < 12.01f; v+= 0.5f)
            {
             
                pbp.Vset(v);
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


            pbp.Vset(0);
            portKoradLoad.WriteLine(":INP OFF\n");
        }

        private void buttonCurrentTest_Click(object sender, EventArgs e)
        {

            pbp.Vset(3.3f); 
            pbp.Iset(0.0f);
            portKoradLoad.WriteLine(":RES 0OHM\n:INP ON");
            System.Threading.Thread.Sleep(1000);


            for (float i = 0.0f; i < 1.01f; i += 0.1f)
            {
                Console.WriteLine("ISET " + i.ToString() + "\n");
                pbp.Iset(i);
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

            pbp.Vset(0);
            portKoradLoad.WriteLine(":INP OFF\n");
        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void label5_Click(object sender, EventArgs e)
        {

        }

        private void label6_Click(object sender, EventArgs e)
        {

        }

        private void label7_Click(object sender, EventArgs e)
        {

        }

        private void textBox3_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox4_TextChanged(object sender, EventArgs e)
        {

        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            voltBessTextbox.Text = pbp.Vget().ToString();
            currentMessTextbox.Text = pbp.Iget().ToString();
        }
    }
}
