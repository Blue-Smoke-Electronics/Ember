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
            port = new SerialPort("COM3", 9600, Parity.None, 8, StopBits.One);

        }
        ~Pbp()
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
                port.WriteLine("ISET " + V.ToString());
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
                port.WriteLine(""); // writes a newline in case of a uncompleated comand beeing in the pbp read buffer
                port.WriteLine("Vget");
                string response = port.ReadLine();
                return float.Parse(response);
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
                port.WriteLine(""); // writes a newline in case of a uncompleated comand beeing in the pbp read buffer
                port.WriteLine("Iget");
                string response = port.ReadLine();
                
                return float.Parse(response);
            }
            else
            {
                return 0.0f;
            }
        }

    }
}
