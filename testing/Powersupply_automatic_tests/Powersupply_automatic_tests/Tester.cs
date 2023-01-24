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
        const int stablisationTime = 10000; //ms
        const int stablisationTime_current = 1000; //ms
        const float maxVoltageError = 0.5f;
        const float maxCurrentError = 10.0f;
        
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
            System.Threading.Thread.Sleep(stablisationTime_current);

            bool failed = false;

            for (float i = 0.0f; i < maxCurrent+1; i += stepsize)
            {
                Console.WriteLine("ISET " + i.ToString() + "\n");
                pbp.Iset(i);
                System.Threading.Thread.Sleep(stablisationTime_current);
                float ampMeas = konradLoad.Iget();

                Console.WriteLine("diff: " + (Math.Abs(i - ampMeas)).ToString());
                if (Math.Abs(i - ampMeas) > 10)
                {
                    failed = true;
                    MessageBox.Show("ERROR to hight at " + i.ToString() + "mA");
                }
                timeData.Add(timeNow());
                currentData.Add(konradLoad.Iget());
                currentDataTarget.Add(i);
                voltageData.Add(konradLoad.Vget());
                voltageDataTarget.Add(0);
            }

            pbp.Vset(0);
            pbp.DisableOutput();
            konradLoad.DisableOutput();
            ScottPlot.FormsPlot formsPlot = new ScottPlot.FormsPlot();
            formsPlot.Plot.AddScatter(timeData.ToArray(), voltageData.ToArray(),color: System.Drawing.Color.Blue);
            formsPlot.Plot.AddScatter(timeData.ToArray(), voltageDataTarget.ToArray(), color: System.Drawing.Color.Red);
            formsPlot.Plot.AddScatter(timeData.ToArray(),currentData.ToArray(), color: System.Drawing.Color.Blue);
            formsPlot.Plot.AddScatter(timeData.ToArray(), currentDataTarget.ToArray(), color: System.Drawing.Color.Red);
            ScottPlot.FormsPlotViewer viewer = new ScottPlot.FormsPlotViewer(formsPlot.Plot);
            viewer.Show();

            if (!failed)
            {
                MessageBox.Show("ConstantVoltageShortedLoadVarCurrentTest Success!!!");
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
            int successCnt  = 0; 
            bool failed = false;
         
           
            Random random = new Random();
            for (int i = 0; i < testCnt; i++)
            {
                double voltage = random.NextDouble() * maxVoltage;
                double current = random.NextDouble() * (maxCurrent-5)+5;
                double resistance = Math.Pow(random.NextDouble(), 4) * (konradLoad.maxResistance);

                if (resistance < current / 200)
                {
                    resistance = current / 200;
                }
                bool isVoltagelimited = false; // always voltage or current limited
                if (voltage < (current / 1000.0f) * resistance){
                    isVoltagelimited = true; 
                }

                teoreticalVoltage.Add( Math.Min(voltage, (current/1000.0f) * resistance));
                teoreticalCurrent.Add(Math.Min(current, (voltage / resistance)*1000));

                
                
                konradLoad.SetCR((float)resistance);
                pbp.Iset((float)current);
                pbp.Vset((float)voltage);
                
                double timeOutStart = timeNow();
                successCnt = 0;
                while (timeNow() - timeOutStart < stablisationTime && successCnt < 3)
                {
                    double V = konradLoad.Vget();
                    double I = konradLoad.Iget();
                    if (isVoltagelimited)
                    {
                        if (Math.Abs(teoreticalVoltage.Last() - V) < maxVoltageError ){
                            successCnt++;
                        }
                        else
                        {
                            successCnt = 0;
                        }
                    }
                    else  { // current limited 
                        if (Math.Abs(teoreticalCurrent.Last() - I) < maxCurrentError) {
                            successCnt++;
                        }
                        else
                        {
                            successCnt = 0;
                        }
                    }
                    

                    System.Threading.Thread.Sleep(100);
                }

                // warn of errors 
                if (successCnt < 3)
                {
                    MessageBox.Show("Random test failed!!!!\n" +
                        " expected: \n "+ teoreticalVoltage.Last() +"V \n"+
                        teoreticalCurrent.Last() + "A");
                    failed = true; 
                }

                /*acutalVoltage.Add(konradLoad.Vget());
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
                */


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

            /*ScottPlot.Plot plot = new ScottPlot.Plot();
            plot.AddScatter(acutalResistance.ToArray(), u.ToArray(), color: System.Drawing.Color.Red,lineStyle:ScottPlot.LineStyle.None);
            plot.AddScatter(acutalResistance.ToArray(), u2.ToArray(), color: System.Drawing.Color.Blue, lineStyle: ScottPlot.LineStyle.None);
            //plot.AddScatter(acutalVoltage.ToArray(), acutalResistance.ToArray(), color: System.Drawing.Color.Blue);
            //plot.AddScatter(timeData.ToArray(), voltageDataTarget.ToArray(), color: System.Drawing.Color.Red);
            //plot.AddScatter(timeData.ToArray(), currentData.ToArray(), color: System.Drawing.Color.Blue);
            //plot.AddScatter(timeData.ToArray(), currentDataTarget.ToArray(), color: System.Drawing.Color.Red);
            ScottPlot.FormsPlotViewer viewer = new ScottPlot.FormsPlotViewer(plot);
            viewer.Show();*/

            if (!failed)
            {
                MessageBox.Show("Random test compleated successfully :)");
            }


        } 
        private double timeNow()
        {
            return (System.DateTime.Now - this.startTime).TotalMilliseconds; 
        }

        public void discharge_and_charge_test()
        {      
            // setup plot
            double[] v = new double[100_000];
            double[] t = new double[100_000];
            double[] charge = new double[100_000];
            int sampleCnt = 0;            
            
            ScottPlot.Plot plot = new ScottPlot.Plot();
            plot.AddScatter(t, v);
            plot.SetAxisLimits(0, 100, 2.8, 4.2);
            ScottPlot.FormsPlotViewer viewer = new ScottPlot.FormsPlotViewer(plot);
            viewer.Show();

            ScottPlot.Plot plot1 = new ScottPlot.Plot();
            plot1.AddScatter(t, charge);
            plot1.SetAxisLimits(0, 100, -1, 101);
            ScottPlot.FormsPlotViewer viewer1 = new ScottPlot.FormsPlotViewer(plot1);
            viewer1.Show();

            double startTime = timeNow();
            bool failed = false;

            // wait for battery to charge up
            while (pbp.BattGetV() < 4.1)
            {
                t[sampleCnt] = (timeNow() - startTime) / 1000.0f;
                v[sampleCnt] = pbp.BattGetV();
                charge[sampleCnt] = pbp.BattGetP();
                viewer.formsPlot1.Plot.AxisAutoX();
                viewer.formsPlot1.Render();
                viewer1.formsPlot1.Plot.AxisAutoX();
                viewer1.formsPlot1.Render();
                sampleCnt++;
                System.Threading.Thread.Sleep(1000);
            }

            // set load to 15 ohm
            konradLoad.SetCR(15.0f);
            konradLoad.EnableOutput();

            // turn on powersupply 
            pbp.Vset(15);
            pbp.Iset(999);
            pbp.EnableOutput();
            System.Threading.Thread.Sleep(7000);

            // messure and plot batteryvoltage untill empty

            double drainStarteTimer = timeNow(); 
            while (konradLoad.Vget() > 14.5)
            {

                t[sampleCnt] = (timeNow() - startTime) / 1000.0f;
                v[sampleCnt] = pbp.BattGetV();
                charge[sampleCnt] = pbp.BattGetP();
                viewer.formsPlot1.Plot.AxisAutoX();
                viewer.formsPlot1.Render();

                viewer1.formsPlot1.Plot.AxisAutoX();
                viewer1.formsPlot1.Render();

                sampleCnt++;

                System.Threading.Thread.Sleep(1000);


            }

            pbp.DisableOutput();
            konradLoad.DisableOutput();

            
            // error if ontime is less than 10 min
            if ((timeNow() - drainStarteTimer) / (1000.0f * 60.0f) < 10)  // 10 min
            {
                MessageBox.Show("discharge test failed, used only " + ((timeNow() - drainStarteTimer) / (1000.0f * 60.0f)).ToString() + " min to drain battery");
                failed = true;
            }

            double chargeStartTimer = timeNow();

            // mesure and plot battery voltage while charging
            while (pbp.BattGetV() < 4.1f)
            {
                t[sampleCnt] = (timeNow() - startTime) / 1000.0f;
                v[sampleCnt] = pbp.BattGetV();
                charge[sampleCnt] = pbp.BattGetP();
                viewer.formsPlot1.Plot.AxisAutoX();
                viewer.formsPlot1.Render();
                sampleCnt++;
                System.Threading.Thread.Sleep(1000);
            }


            // error if charging time is grater than 4 hours 
            if ((timeNow() - chargeStartTimer) / (1000.0f * 60.0f*60) > 5)  // 5 houers
            {
                MessageBox.Show("charging test failed, used " + ((timeNow() - chargeStartTimer) / (1000.0f * 60.0f)).ToString() + " min to charge battery");
                failed = true;
            }

            if (!failed)
            {
                MessageBox.Show("Charring and discharge test Success :)");
            }
        }

    }


    
}
