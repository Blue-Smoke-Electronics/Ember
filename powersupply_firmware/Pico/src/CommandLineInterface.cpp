#include "CommandlineInterface.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include <string.h>
#include "Flash.h"
#include "PSU.h"
#include "Battery.h"

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
        }

        if (new_char == 127){ // BACKSPACE
            if(rx_data.length() >0) // do not pop when rxdata is empty
                rx_data.pop_back();

            printf("%c",new_char);
        }

        if(!((new_char < 65 || new_char > 90)/*A-Z*/ && (new_char < 97 || new_char > 122)/*a-z*/ && (new_char < 48 || new_char > 57)/*0-9*/ && (new_char != 46)/*.*/ && (new_char != 32))/* */){ // remove not statndard symbols
            rx_data.push_back(new_char);
            printf("%c",new_char);
        }
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
        float current = PSU::getCurrent();
        char s[20];
        sprintf(s, "%f\r\n",current);
        printf(s);
        return;
    }

    compare = "VGET";
    if (command.rfind(compare.c_str(), 0) == 0)
    {
        float current = PSU::getVoltage();
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
        printf("H        # shows this text\n\r");
        printf("HELP     # shows this text\n\r");
        return;
    }

    printf("invalid command: %s \r\n", command.c_str());
}
