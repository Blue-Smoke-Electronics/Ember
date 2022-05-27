using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;

namespace Powersupply_automatic_tests
{
    internal class KonradPsu
    {
        SerialPort port;

        public KonradPsu()
        {
            port = new SerialPort("COM15", 9600, Parity.None, 8, StopBits.One);
            port.RtsEnable = true;
            port.DtrEnable = true;
        }
        ~KonradPsu()
        {
            port.Close();
        }
        public Boolean IsConnected
        {
            get { return port.IsOpen; }
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

        public void Vset(float V)
        {
            if (port.IsOpen)
            {
                port.WriteLine("ISET1: " + V.ToString());
            }
            else
            {
                Console.WriteLine("pbp: com port is not open");
            }

        }

        public float Vget()
        {
            if (port.IsOpen)
            {
                port.Write("VOUT1?\n");

                string data = port.ReadLine();
                return float.Parse(data);
            }
            else
            {
                return 0.0f;
            }
        }
        public float Iget()
        {
            if (port.IsOpen)
            {

                port.WriteLine("IOUT1?");

                string response = port.ReadLine();

                return float.Parse(response)*1000;
            }
            else
            {
                return 0.0f;
            }
        }
        public void EnableOutput()
        {

        }
        public void DisableOutput()
        {

        }

    }
}
