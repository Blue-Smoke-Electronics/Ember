using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;

namespace Powersupply_automatic_tests
{

    
    internal class Pbp
    {
        SerialPort port;
        public Pbp()
        {
            port = new SerialPort("COM34", 115200, Parity.None, 8, StopBits.One);
            port.RtsEnable = true;
            port.DtrEnable = true;
            //port.WriteTimeout = 1000; 
        }
        ~Pbp()
        {
            port.Close();   
        }
        public Boolean IsConnected
        {
            get { return port.IsOpen; }
        }
        public Boolean ConnectToSerial(string COMport)
        {
            try
            {
                port.Close();
                port = new SerialPort(COMport, 115200, Parity.None, 8, StopBits.One);
                port.RtsEnable = true;
                port.DtrEnable = true;
                port.Open();
                return true;

            } catch (Exception ex){
                Console.WriteLine(ex.Message);
                return false;
            }   
        } 

        public void Iset ( float mA)
        {
            if (port.IsOpen)
            {
   
                port.WriteLine("ISET " + mA.ToString());
                System.Threading.Thread.Sleep(100);
                port.DiscardInBuffer(); // remove reply form inputbuffer
 
            }
            else
            {
                Console.WriteLine("pbp: com port is not open");
            }
            
        }

        public void Vset(float V)
        {
            if (port.IsOpen)
            {
                
                
                port.WriteLine("VSET " + V.ToString());
                System.Threading.Thread.Sleep(100);
                port.DiscardInBuffer(); // remove reply form inputbuffer
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
                System.Threading.Thread.Sleep(200);
                port.DiscardInBuffer();
                System.Threading.Thread.Sleep(200);
                port.Write("VGET\n");
                string echo = port.ReadLine();
                //string chargereturn1 = port.ReadLine();
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
                System.Threading.Thread.Sleep(100);
                port.DiscardInBuffer();
                port.WriteLine("Iget");
                string echo = port.ReadLine();
                //string chargereturn1 = port.ReadLine();
                string response = port.ReadLine();
                
                return float.Parse(response);
            }
            else
            {
                return 0.0f;
            }
        }

        public float BattGetV()
        {
            if (port.IsOpen)
            {
                System.Threading.Thread.Sleep(100);
                port.DiscardInBuffer();
                port.WriteLine("BATGETV");
                string echo = port.ReadLine();
                //string chargereturn1 = port.ReadLine();
                string response = port.ReadLine();
                float volt;
                try
                {
                    volt = float.Parse(response);
                }
                catch
                {
                    volt = 0.0f;
                }

                return volt;
                
            }
            else
            {
                return 0.0f;
            }
        }

        public float BattGetP()
        {
            if (port.IsOpen)
            {
                System.Threading.Thread.Sleep(100);
                port.DiscardInBuffer();
                port.WriteLine("BATGETP");
                string echo = port.ReadLine();
                //string chargereturn1 = port.ReadLine();
                string response = port.ReadLine();

                float volt;
                try
                {
                    volt = float.Parse(response);
                }
                catch
                {
                    volt = 0.0f;
                }

                return volt;
            }
            else
            {
                return 0.0f;
            }
        }

        public float TempGet()
        {
            if (port.IsOpen)
            {
                System.Threading.Thread.Sleep(100);
                port.DiscardInBuffer();
                port.WriteLine("TEMPGET");
                string echo = port.ReadLine();
                //string chargereturn1 = port.ReadLine();
                string response = port.ReadLine();

                float temp;
                try
                {
                    temp = float.Parse(response);
                }
                catch
                {
                    temp = 0.0f;
                }

                return temp;
            }
            else
            {
                return 0.0f;
            }
        }
        public void EnableOutput()
        {
            if (port.IsOpen)
            {
                bool success = false;
                while (!success)
                {
                    try
                    {
                        port.WriteLine("ON");
                        System.Threading.Thread.Sleep(100);
                        port.DiscardInBuffer(); // remove reply form inputbuffer
                        success = true;
                    }
                    catch
                    {
                    success = false;
                    }
                }
            }
            else
            {
                Console.WriteLine("pbp: com port is not open");
            }
        }
        public void DisableOutput()
        {
            if (port.IsOpen)
            {
                bool success = false;
                while (!success)
                {
                    try
                    {
                        port.WriteLine("OFF");
                        System.Threading.Thread.Sleep(100);
                        port.DiscardInBuffer(); // remove reply form inputbuffer
                        success = true;
                    }
                    catch {
                        success = false;
                    }

                }
                

                
            }
            else
            {
                Console.WriteLine("pbp: com port is not open");
            }
        }

    }
}
