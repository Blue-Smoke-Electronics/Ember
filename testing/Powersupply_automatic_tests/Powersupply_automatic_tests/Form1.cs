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
using ScottPlot; // downloawd with nuGet



namespace Powersupply_automatic_tests
{
    public partial class Form1 : Form
    {

        Pbp pbp;
        KonradPsu konradPsu;
        KonradLoad konradLoad;
        Tester tester;

        List <double> xData;
        List<double> yData;

        public Form1()
        {
            InitializeComponent();

            pbp = new Pbp();
            konradPsu = new KonradPsu();
            konradLoad = new KonradLoad();

            tester = new Tester(pbp,konradLoad,konradPsu);
            xData = new List<double>();
            yData = new List<double>();

        }


        private void button1_Click(object sender, EventArgs e)
        {
            isConnectedBlueSmoek.Checked = pbp.ConnectToSerial();
            isConnectedKoradLoad.Checked = konradLoad.ConnectToSerial();
            isConnectedKonradPSU.Checked = konradPsu.ConnectToSerial();
            //timer1.Start();

        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {

        }

        private void buttonVoltageTest_Click(object sender, EventArgs e)
        {
            konradLoad.SetCR(1000);
            konradLoad.EnableOutput();
            pbp.Iset(100);
            System.Threading.Thread.Sleep(1000);

            for (float v =0.0f; v < 12.01f; v+= 0.5f)
            {
             
                pbp.Vset(v);
                System.Threading.Thread.Sleep(1000);

                float voltMeas = konradLoad.Vget();

                Console.WriteLine("diff: " + (Math.Abs(v - voltMeas)).ToString());
                if (Math.Abs(v - voltMeas) > 0.1)
                {
                    MessageBox.Show("ERROR to hight at " + v.ToString()+"V");
                }

            }


            pbp.Vset(0);
            konradLoad.DisableOutput();
        }

        private void buttonCurrentTest_Click(object sender, EventArgs e)
        {

            if(pbp.IsConnected && konradLoad.IsConnected)
            {
                tester.ConstantVoltageShortedLoadVarCurrentTest(3.3f);
            }
            else
            {
                MessageBox.Show("connect to pbp and load first");
            }
            /*
            pbp.Vset(3.3f); 
            pbp.Iset(0.0f);
            konradLoad.SetCR(0);
            konradLoad.EnableOutput();
            pbp.EnableOutput();
            System.Threading.Thread.Sleep(1000);


            for (float i = 0.0f; i < 500f; i += 50f)
            {
                Console.WriteLine("ISET " + i.ToString() + "\n");
                pbp.Iset(i);
                System.Threading.Thread.Sleep(2000);
                float ampMeas = konradLoad.Iget();

                Console.WriteLine("diff: " + (Math.Abs(i - ampMeas)).ToString());
                if (Math.Abs(i - ampMeas) > 25)
                {
                    MessageBox.Show("ERROR to hight at " + i.ToString() + "mA");
                }

            }

            pbp.Vset(0);
            konradLoad.DisableOutput();*/
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

        int i=0;


        private void timer1_Tick(object sender, EventArgs e)
        {
            i++; 
            float pbpVolt = pbp.Vget();
            float pbpCurrent = pbp.Iget();
            voltMessTextbox.Text = pbpVolt.ToString();
            currentMessTextbox.Text = pbpCurrent.ToString();
            loadVoltageTextbox.Text =konradLoad.Vget().ToString();
            loadCurrentTextbox.Text = konradLoad.Iget().ToString();


        }

        private void chart1_Click(object sender, EventArgs e)
        {
            
        }

        private void button1_Click_1(object sender, EventArgs e)
        {
            

            

            
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void randomButton_Click(object sender, EventArgs e)
        {
            if (pbp.IsConnected && konradLoad.IsConnected)
            {
                tester.Random(200);
            }
            else
            {
                MessageBox.Show("connect to pbp and load first");
            }
        }

        private void button1_Click_2(object sender, EventArgs e)
        {
            
        }

        private void button1_Click_3(object sender, EventArgs e)
        {
            float P_in = this.konradPsu.Vget() * this.konradPsu.Iget();
            float P_out = this.konradLoad.Vget() * this.konradLoad.Iget();

            MessageBox.Show((P_out / P_in).ToString());
        }
    }
}
