﻿using System;
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

        KonradPsu()
        {
            port = new SerialPort("COM15", 9600, Parity.None, 8, StopBits.One);
        }
        ~KonradPsu(){
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
                port.Write("VGET\n");

                string echo = port.ReadLine();
                string chargereturn1 = port.ReadLine();
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

                port.WriteLine("Iget");
                string echo = port.ReadLine();
                string chargereturn1 = port.ReadLine();
                string response = port.ReadLine();

                return float.Parse(response);
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