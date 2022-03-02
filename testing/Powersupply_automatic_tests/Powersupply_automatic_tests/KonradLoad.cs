using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Powersupply_automatic_tests
{
    internal class KonradLoad
    {
        public KonradLoad()
        {
            port = new SerialPort("COM17", 9600, Parity.None, 8, StopBits.One);
        }
        ~KonradLoad()
        {
            port.Close();
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

        public void SetCC( float mA)
        {

        }

        public void SetRes(float ohm)
        {

        }
        
        public void SetCV(float V)
        {

        }
        public void EnableOutput()
        {

        }
        public void DisableOutput()
        {

        }



    }
}
