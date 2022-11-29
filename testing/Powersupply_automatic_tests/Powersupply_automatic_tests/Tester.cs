using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Powersupply_automatic_tests
{
    internal class Tester
    {


        const float maxCurrent = 1000; // mA 
        const float maxVoltage = 15; // V
        const int stablisationTime = 5000; //ms

        const float maxVoltageError = 0.5f;
        const float maxCurrentError = 5.0f;
        
        Pbp pbp;
        KonradLoad konradLoad;
        KonradPsu konradPsu;

        System.DateTime startTime;

        public Tester(Pbp pbp, KonradLoad konradLoad, KonradPsu konradPsu)
        {
            this.pbp = pbp;
            this.konradLoad = konradLoad;
            this.konradPsu = konradPsu; 
            
            startTime = System.DateTime.Now;

        }
       
        public void ConstantVoltageShortedLoadVarCurrentTest(float Voltage, float stepsize= 50)
        {
            List<double> timeData = new List<double>();
            List<double> currentData = new List<double>();
            List<double> currentDataTarget = new List<double>();
            List<double> voltageData = new List<double>();
            List<double> voltageDataTarget = new List<double>();
            // set load to short
            konradLoad.SetCR(0);
            konradLoad.EnableOutput();
            
            // turn on device 
            pbp.Vset(Voltage);
            pbp.Iset(0.0f);
            pbp.EnableOutput();
            System.Threading.Thread.Sleep(stablisationTime);

            bool failed = false;

            for (float i = 0.0f; i < maxCurrent; i += stepsize)
            {
                Console.WriteLine("ISET " + i.ToString() + "\n");
                pbp.Iset(i);
                System.Threading.Thread.Sleep(stablisationTime);
                float ampMeas = konradLoad.Iget();

                Console.WriteLine("diff: " + (Math.Abs(i - ampMeas)).ToString());
                if (Math.Abs(i - ampMeas) > 25)
                {
                    failed = true;
                    Console.WriteLine("ERROR to hight at " + i.ToString() + "mA");
                }
                timeData.Add(timeNow());
                currentData.Add(konradLoad.Iget());
                currentDataTarget.Add(i);
                voltageData.Add(konradLoad.Vget());
                voltageDataTarget.Add(0);
            }

            pbp.Vset(0);
            konradLoad.DisableOutput();
            ScottPlot.FormsPlot formsPlot = new ScottPlot.FormsPlot();
            formsPlot.Plot.AddScatter(timeData.ToArray(), voltageData.ToArray(),color: System.Drawing.Color.Blue);
            formsPlot.Plot.AddScatter(timeData.ToArray(), voltageDataTarget.ToArray(), color: System.Drawing.Color.Red);
            formsPlot.Plot.AddScatter(timeData.ToArray(),currentData.ToArray(), color: System.Drawing.Color.Blue);
            formsPlot.Plot.AddScatter(timeData.ToArray(), currentDataTarget.ToArray(), color: System.Drawing.Color.Red);
            ScottPlot.FormsPlotViewer viewer = new ScottPlot.FormsPlotViewer(formsPlot.Plot);
            viewer.Show();

            if (failed)
            {
                MessageBox.Show("ERROR ConstantVoltageShortedLoadVarCurrentTest FAILED!!!");
            }
        }



        public void Random(int testCnt)
        {
            // enable load 
            konradLoad.SetCR((int)konradLoad.maxResistance);
            konradLoad.EnableOutput();

            // enable pbp
            pbp.Vset(3.3f);
            pbp.Vset(100);
            pbp.EnableOutput();
            List <double> teoreticalVoltage = new List<double>();
            List<double> teoreticalCurrent = new List<double>();
            List<double> acutalVoltage = new List<double>();
            List<double> acutalCurrent = new List<double>();
            List<double> acutalResistance = new List<double>();
            bool failed = false; 
           
            Random random = new Random();
            for (int i = 0; i < testCnt; i++)
            {
                double voltage = random.NextDouble() * maxVoltage;
                double current = random.NextDouble() * (maxCurrent-5)+5;
                double resistance = Math.Pow(random.NextDouble(), 4) * (konradLoad.maxResistance);

                if (resistance < current / 1000)
                {
                    resistance = current / 1000;
                }

                teoreticalVoltage.Add( Math.Min(voltage, (current/1000.0f) * resistance));
                teoreticalCurrent.Add(Math.Min(current, (voltage / resistance)*1000));

                konradLoad.SetCR((float)resistance);
                pbp.Iset((float)current);
                pbp.Vset((float)voltage);

                // wait for output to stabilize 
                System.Threading.Thread.Sleep(stablisationTime);    

                acutalVoltage.Add(konradLoad.Vget());
                acutalCurrent.Add(konradLoad.Iget());
                acutalResistance.Add(resistance);


                if (Math.Abs(teoreticalVoltage.Last() - acutalVoltage.Last()) > maxVoltageError && Math.Abs(acutalCurrent.Last() - current) > maxCurrentError)
                {
                    Console.WriteLine("failed!! --target  Voltage: "+ teoreticalVoltage.Last().ToString() + "  Current: " + teoreticalCurrent.Last().ToString() + " Power: " + (teoreticalVoltage.Last() * teoreticalCurrent.Last() / 1000).ToString());
                    Console.WriteLine("failed!! --actual Voltage: " + acutalVoltage.Last().ToString() + "  Current: " + acutalCurrent.Last().ToString() + " Power: " + (acutalVoltage.Last() * acutalCurrent.Last() / 1000).ToString());
                    failed = true;
                    
                }
                if (Math.Abs(teoreticalCurrent.Last() - acutalCurrent.Last()) > maxCurrentError && Math.Abs(acutalVoltage.Last() - voltage) > maxVoltageError)
                {
                    Console.WriteLine("failed!! --actual Voltage: " + acutalVoltage.Last().ToString() + "  Current: " + acutalCurrent.Last().ToString() + " Power: " + (acutalVoltage.Last() * acutalCurrent.Last() / 1000).ToString());
                    Console.WriteLine("failed!! --target  Voltage: " + teoreticalVoltage.Last().ToString() + "  Current: " + teoreticalCurrent.Last().ToString() + " Power: " + (teoreticalVoltage.Last() * teoreticalCurrent.Last() / 1000).ToString());
                    failed = true;
                }

            }

            //turn off pbp
            pbp.DisableOutput();

            // turn off load
            konradLoad.DisableOutput();


            // plot resouts 
            List<double> u = new List<double>(); 
            for(int i = 0; i < acutalVoltage.Count; i++)
            {
                u.Add(acutalVoltage[i]*acutalCurrent[i]);
            }
            List<double> u2 = new List<double>();
            for (int i = 0; i < teoreticalVoltage.Count; i++)
            {
                u2.Add(teoreticalVoltage[i] * teoreticalCurrent[i]);
            }

            ScottPlot.Plot plot = new ScottPlot.Plot();
            plot.AddScatter(acutalResistance.ToArray(), u.ToArray(), color: System.Drawing.Color.Red,lineStyle:ScottPlot.LineStyle.None);
            plot.AddScatter(acutalResistance.ToArray(), u2.ToArray(), color: System.Drawing.Color.Blue, lineStyle: ScottPlot.LineStyle.None);
            //plot.AddScatter(acutalVoltage.ToArray(), acutalResistance.ToArray(), color: System.Drawing.Color.Blue);
            //plot.AddScatter(timeData.ToArray(), voltageDataTarget.ToArray(), color: System.Drawing.Color.Red);
            //plot.AddScatter(timeData.ToArray(), currentData.ToArray(), color: System.Drawing.Color.Blue);
            //plot.AddScatter(timeData.ToArray(), currentDataTarget.ToArray(), color: System.Drawing.Color.Red);
            ScottPlot.FormsPlotViewer viewer = new ScottPlot.FormsPlotViewer(plot);
            viewer.Show();


            // warn of errors 
            if (failed)
            {
                MessageBox.Show("Random test failed!!!!");
            }

        } 
        private double timeNow()
        {
            return (System.DateTime.Now - this.startTime).TotalMilliseconds; 
        }

    }


    
}
