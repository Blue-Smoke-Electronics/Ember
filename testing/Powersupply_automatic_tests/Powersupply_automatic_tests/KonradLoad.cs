using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;

namespace Powersupply_automatic_tests
{
    internal class KonradLoad
    {
        SerialPort port;
        public KonradLoad()
        {
            port = new SerialPort("COM17", 9600, Parity.None, 8, StopBits.One);
            port.RtsEnable = true;
            port.DtrEnable = true;
        }
        ~KonradLoad()
        {
            port.Close();
        }

        public Boolean IsConnected
        {
            get { return port.IsOpen; }
        }
        public float maxResistance
        {
            get { return 7500; }
        }
        public Boolean ConnectToSerial()
        {
            try
            {
                port.Close();
                port.Open();
                return true;

            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                return false;
            }
        }

        public void SetCC( float Ampere)
        {
            port.WriteLine(":FUNC CURR\n:CURR " + Ampere.ToString("N3") + "A");
            //port.WriteLine(":FUNC CURR\n:CURR 2.500A");
        }

        public void SetCR(float ohm)
        {
            if(ohm > 7500)
            {
                ohm = 7500; // limit of device
            }
            port.WriteLine(":FUNC RES\n:RES "+ ohm.ToString("F2")+ "OHM");
        }
        
        public void SetCV(float Voltage)
        {
            port.WriteLine(":FUNC VOLT\n:VOLT " + Voltage.ToString("F3") + "V");
        }
        public void SetCW(float Watt)
        {
            port.WriteLine(":FUNC POW\n:POW " + Watt.ToString("F3") + "W");
        }

        public float Vget()
        {
            
            if (port.IsOpen)
            {
                try
                    {port.WriteLine(":MEAS:VOLT?\n");
                    string voltMeasStr = port.ReadLine();
                    return float.Parse(voltMeasStr.Split('V')[0]); 

                }
                catch
                {
                    return 0.0f; 
                }
                
            }
            return 0;
        }
        public float Iget()
        {
            if (port.IsOpen)
            {
                try
                {
                    port.WriteLine(":MEAS:CURR?\n");
                    string currentMeasStr = port.ReadLine();
                    return float.Parse(currentMeasStr.Split('A')[0])*1000;

                }
                catch 
                {
                    return 0.0f; 
                }
            }   
            return 0;
        }

        public void EnableOutput()
        {
            port.WriteLine(":INP ON");
        }
        public void DisableOutput()
        {
            port.WriteLine(":INP OFF");
        }



    }
}
