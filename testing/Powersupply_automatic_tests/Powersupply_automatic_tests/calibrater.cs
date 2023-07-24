using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Powersupply_automatic_tests
{
    internal class calibrater
    {

        Pbp pbp;
        KonradLoad konradLoad;

        public calibrater(Pbp pbp, KonradLoad konradLoad)
        {
            this.pbp = pbp;
            this.konradLoad = konradLoad;

        }

        public void calibrate_voltage()
        {
            if (!pbp.IsConnected)
            {
                MessageBox.Show("pbp not connected");
                return;
            }
            if (!konradLoad.IsConnected)
            {
                MessageBox.Show("load not connected");
                return;
            }


            konradLoad.SetCR(1000);
            konradLoad.EnableOutput();
            pbp.Vset(0);
            pbp.Iset(100);
            pbp.EnableOutput();
            for (int v = 0; v < 16; v++) {
                pbp.CalibrateVolt(v,0);
                pbp.Vset(v);
                System.Threading.Thread.Sleep(3000);

                float Vpbp = pbp.Vget();
                float Vload = konradLoad.Vget(); 
                float diff = Vpbp - Vload;
                pbp.CalibrateVolt(v, diff);

            }

            pbp.DisableOutput();
            konradLoad.DisableOutput();
        }
        public void calibrate_current()
        {
            if(!pbp.IsConnected)
            {
                MessageBox.Show("pbp not connected");
                return; 
            }
            if (!konradLoad.IsConnected)
            {
                MessageBox.Show("load not connected");
                return;
            }

            konradLoad.SetCR(0);
            konradLoad.EnableOutput();
            pbp.Vset(5);
            pbp.Iset(0);
            pbp.EnableOutput();
            for (int i = 0; i < 1100; i+=100)
            {
                pbp.CalibrateCurrent(i, 0);
                pbp.Iset(i);
                System.Threading.Thread.Sleep(3000);
                pbp.CalibrateCurrent(i, pbp.Iget()- konradLoad.Iget());

            }

            pbp.DisableOutput();
            konradLoad.DisableOutput();

        }



    }
}
