#include "CommandlineInterface.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Flash.h"
#include "PSU.h"
#include "Battery.h"
#include "Onoff.h"
#include "Analog.h"
#include "Powersaver.h"

int CommandLineInterface::update_timer;
std::string CommandLineInterface::rx_data;
int CommandLineInterface::rx_data_ptr;

void CommandLineInterface::Init(){
    rx_data_ptr = 0;
}

void CommandLineInterface::Update(){
    if(time_us_32() - update_timer > update_freq_us)
    {
        update_timer = time_us_32();
        // printf("CLI update \r\n");

        // do nothing if no new charicter is detected
        int raw_char = getchar_timeout_us(0);
        if (raw_char == PICO_ERROR_TIMEOUT)
            return;

        char new_char = (char)raw_char; // cast int to char
        new_char = toupper((char)raw_char); // convert to uper case

        if (new_char == 13 || new_char == '\n'){ // ENTER
            printf("\r\n");
            desifre_command(rx_data);
            rx_data.clear();
            Onoff::Turn_on_device();
        }

        if (new_char == 127){ // BACKSPACE
            if(rx_data.length() >0) // do not pop when rxdata is empty
                rx_data.pop_back();

            printf("%c",new_char);
        }

        if(!((new_char < 65 || new_char > 90)/*A-Z*/ && (new_char < 97 || new_char > 122)/*a-z*/ && (new_char < 48 || new_char > 57)/*0-9*/ && (new_char != 46)/*.*/ && (new_char != 32) /* space*/&& (new_char != 45 )/* - */ )){ // remove not statndard symbols
            rx_data.push_back(new_char);
            printf("%c",new_char);
        }
        Powersaver::Reset_idle_timer();
    }
}

void CommandLineInterface::desifre_command(std::string command){
    std::string compare = "VSET";
    if (command.rfind(compare.c_str(), 0) == 0)
    {
        std::string value_str = command.substr(compare.length());
        float voltValue = 0;
        try
        {
            voltValue = std::stof(value_str); 
        }
        catch(const std::exception& e)
        {
            printf("invalid input: got %s, expected a number\r\n", value_str.c_str());
            return; 
        }

        if (voltValue > PSU::maxVoltage)
            voltValue = PSU::maxVoltage;

        printf("setting max Volt to %f \n\r",voltValue);
        PSU::SetVoltage(voltValue);
        return;
    }
    
    compare = "ISET";
    if (command.rfind(compare.c_str(), 0) == 0)
    {
        std::string value_str = command.substr(compare.length());
        float curretnValue = 0;
        try
        {
            curretnValue = std::stof(value_str); 
        }
        catch(const std::exception& e)
        {
            printf("invalid input: got %s, expected a number\r\n", value_str.c_str() );
            return; 
        }

        if (curretnValue > PSU::maxCurrent)
            curretnValue = PSU::maxCurrent;

        printf("setting current limit to %f\n\r", curretnValue);
        PSU::SetCurrent(curretnValue);
        return;
    }

    compare = "ON";
    if (command.rfind(compare.c_str(), 0) == 0)
    {
        printf("turning on Powersupply\n\r");
        PSU::Enable();
        return;
    }

    compare = "OFF";
    if (command.rfind(compare.c_str(), 0) == 0)
    {
        printf("turning off Powersupply\n\r");
        PSU::Disable();
        return;
    }

    compare = "IGET";
    if (command.rfind(compare.c_str(), 0) == 0)
    {
        float current = PSU::getCurrentSmooth();
        char s[20];
        sprintf(s, "%f\r\n",current);
        printf(s);
        return;
    }

    compare = "VGET";
    if (command.rfind(compare.c_str(), 0) == 0)
    {
        float current = PSU::getVoltageSmooth();
        char s[20];
        sprintf(s, "%f\r\n",current);
        printf(s);
        return;
    }

    compare = "BATGETP";
    if (command.rfind(compare.c_str(), 0) == 0)
    {
        float current = Battery::GetBatteryProcentage();
        char s[20];
        sprintf(s, "%f\r\n",current);
        printf(s);
        return;
    }

    compare = "BATGETV";
    if (command.rfind(compare.c_str(), 0) == 0)
    {
        float current = Battery::GetVoltage();
        char s[20];
        sprintf(s, "%f\r\n",current);
        printf(s);
        return;
    }
    
    compare = "TEMPGET";
    if (command.rfind(compare.c_str(), 0) == 0)
    {
        float temp = Analog::GetTemp();
        char s[20];
        sprintf(s, "%f\r\n",temp);
        printf(s);
        return;
    }

    compare = "CALIBRATEVOLT";
    if (command.rfind(compare.c_str(), 0) == 0)
    {
        std::string value_str = command.substr(compare.length());
        float target_volt = 0;
        float diff =0; 
        try
        {
            std::string par1 = value_str.substr(0,value_str.find(' ',2));
            std::string par2 = value_str.substr(value_str.find(' ',2),value_str.length());

            target_volt = std::stof(par1); 
            diff = std::stof(par2); 
        }
        catch(const std::exception& e)
        {
            printf("invalid input: got %s, expected a number\r\n", value_str.c_str() );
            return; 
        }

        if (target_volt != std::round(target_volt) && target_volt > 0 && target_volt < 21){
            printf("invalint target voltage");
            return;
        }
        
        
        printf("setting %f to %f\n\r", target_volt, diff);
        Analog::VoltageCalibration(int(target_volt),diff);

        return;
    }
    compare = "CALIBRATECURRENT";
    if (command.rfind(compare.c_str(), 0) == 0)
    {
        std::string value_str = command.substr(compare.length());
        float target = 0;
        float diff =0; 
        try
        {
            std::string par1 = value_str.substr(0,value_str.find(' ',2));
            std::string par2 = value_str.substr(value_str.find(' ',2),value_str.length());

            target = std::stof(par1); 
            diff = std::stof(par2); 
        }
        catch(const std::exception& e)
        {
            printf("invalid input: got %s, expected a number\r\n", value_str.c_str() );
            return; 
        }

        if (target/100 != std::round(target/100) && target > 0 && target < 1600){
            printf("invalint target voltage");
            return;
        }
        
        
        printf("setting %f to %f\n\r", target, diff);
        Analog::CurrentCalibration(int(target),diff);

        return;
    }


    compare = "H";
    if (command.rfind(compare.c_str(), 0) == 0)
    {
        printf("VSET 3.3 # sets target voltage to 3.3V \n\r");
        printf("VGET     # returns measured output voltage in V \n\r");
        printf("ISET 100 # sets target output current to 100mA\n\r");
        printf("IGET     # return measured output current in mA\n\r");
        printf("ON       # turns on the powersupply output\n\r");
        printf("OFF      # turns off the powersupply output\n\r");
        printf("BATGETP  # return battery capasity left in %\n\r");
        printf("BATGETV  # returns voltage across battery in V\n\r");
        printf("TEMPGET  # returns temperature in C\n\r");
        printf("H        # shows this text\n\r");
        printf("CALIBRATEVOLT 3 0.03   # used to recalibrate voltage sensor (warning:: errases calibration data)\n\r");
        printf("CALIBRATECURRENT 300 -2.32   # used to recalibrate current sensor (warning:: errases calibration data)\n\r");
        printf("HELP     # shows this text\n\r");
        return;
    }

    printf("invalid command: %s \r\n", command.c_str());
}
