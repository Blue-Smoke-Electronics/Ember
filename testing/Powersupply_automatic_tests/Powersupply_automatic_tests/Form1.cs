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
using ScottPlot; // downloawd with nuGet
using System.Threading;


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
            pbp.EnableOutput();
            System.Threading.Thread.Sleep(1000);
            bool failed = false;

            for (float v =0.0f; v < 15.01f; v+= 0.5f)
            {
             
                pbp.Vset(v);
                System.Threading.Thread.Sleep(1000);

                float voltMeas = konradLoad.Vget();

                Console.WriteLine("diff: " + (Math.Abs(v - voltMeas)).ToString());
                if (Math.Abs(v - voltMeas) > 0.05)
                {
                    MessageBox.Show("ERROR to hight at " + v.ToString()+"V");
                    failed = true;
                }

            }


            pbp.Vset(0);
            pbp.DisableOutput();
            konradLoad.DisableOutput();
            if (!failed)
            {
                MessageBox.Show("Voltage test compleated successfully"); 
            }
        }

        private void buttonCurrentTest_Click(object sender, EventArgs e)
        {

            if(pbp.IsConnected && konradLoad.IsConnected)
            {
                tester.ConstantVoltageShortedLoadVarCurrentTest(3.3f,5);
            }
            else
            {
                MessageBox.Show("connect to pbp and load first");
            }
            
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
                tester.Random(500);
            }
            else
            {
                MessageBox.Show("connect to pbp and load first");
            }
        }


        private void button1_Click_3(object sender, EventArgs e)
        {
            float P_in = this.konradPsu.Vget() * this.konradPsu.Iget();
            float P_out = this.konradLoad.Vget() * this.konradLoad.Iget();

            MessageBox.Show((P_out / P_in).ToString());
        }

        private void buttonBatteryTest_Click(object sender, EventArgs e)
        {
            if (!konradLoad.IsConnected)
            {
                MessageBox.Show("load is not connected");
                return;
            }

            if (konradLoad.Vget() < 4.0f)
            {
                MessageBox.Show("Battery is not fully charged");
                return;
            }
            float testcurrent_A = 0.10000f;
            float time = 0; 
            konradLoad.SetCC(testcurrent_A);
            konradLoad.EnableOutput();

            StreamWriter file = new StreamWriter("battery_test_log.txt");
            System.Diagnostics.Stopwatch watch = new System.Diagnostics.Stopwatch();
            watch.Start();
            while (konradLoad.Vget() > 2.7)
            {

                float volt = konradLoad.Vget();
                time = watch.ElapsedMilliseconds/1000.0f;

                file.WriteLine(time.ToString() + " " + volt.ToString());
                file.Flush();
                Thread.Sleep(1000);
            }
            konradLoad.DisableOutput();
            watch.Stop();
            file.Close();
            MessageBox.Show("Battery test finished: " + (testcurrent_A * time / 3600).ToString()+ " Ah");

        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (pbp.IsConnected && konradLoad.IsConnected)
            {
                tester.discharge_and_charge_test();
            }
            else
            {
                MessageBox.Show("connect to pbp and load first");
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            konradLoad.SetCR(1000);
            konradLoad.EnableOutput();
            pbp.Iset(100);
            pbp.Vset(0);
            pbp.EnableOutput();

            List<double> targetVolts = new List<double>();
            List<double> pbpVolts = new List<double>();
            List<double> konradVolts = new List<double>();
            List<double> diffVolts = new List<double>();

            for (float v = 0; v < 15; v += 0.1f)
            {
                pbp.Vset(v);
                System.Threading.Thread.Sleep(100);
                targetVolts.Add(v);
                pbpVolts.Add(pbp.Vget());
                konradVolts.Add(konradLoad.Vget());
                diffVolts.Add( pbp.Vget() - konradLoad.Vget());
            }

            ScottPlot.FormsPlot formsPlot = new ScottPlot.FormsPlot();
            formsPlot.Plot.AddScatter(targetVolts.ToArray(), diffVolts.ToArray(), color: System.Drawing.Color.Blue);
            //formsPlot.Plot.AddScatter(targetVolts.ToArray(), konradVolts.ToArray(), color: System.Drawing.Color.Red);
            ScottPlot.FormsPlotViewer viewer = new ScottPlot.FormsPlotViewer(formsPlot.Plot);
            viewer.Show();


            konradLoad.DisableOutput();
            pbp.DisableOutput();
        }
    }
}
