#include "ComandlineInterface.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include <string.h>



ComandLineInterface::ComandLineInterface(){ 
    rx_data_ptr =0; 

}
void ComandLineInterface::Update(){
    if(time_us_32() - update_timer > update_freq_us )
    {
        update_timer = time_us_32(); 
       // printf("CLI update \r\n");
        
        // read new data and save to buffer
        while (uart_is_readable){       
            // remove unvalid characters
            char new_char = toupper(getchar());
            if (new_char == 13){ // ENTER
                printf("\n");
                desifre_command(rx_data); 
                rx_data =""; 

            }
            if (new_char == 127){ // BACKSPACE
                if(rx_data_ptr > 0)
                
                rx_data.pop_back();
                printf("%c",new_char);
            }
            if(!((new_char < 65 || new_char > 90)/*A-Z*/ &&(new_char < 97 || new_char > 122)/*a-z*/ && (new_char< 48 || new_char>57)/*0-9*/ && (new_char != 46)/*.*/&& (new_char != 32))/* */){ // remove not statndard symbols
                if(rx_data_ptr < 100){ // make sure input buffer do not overflow
                    rx_data.push_back(new_char); 
                    rx_data_ptr ++; 
                    rx_data[rx_data_ptr] = '\0'; 
                    printf("%c",new_char);
                }
                
  
            }

        }
        


    }
}

void ComandLineInterface::desifre_command(std::string command){

    printf("resived comand: %s\n", command); 

    if (command.rfind("VSET", 0) == 0)
    {
        float voltValue = std::stof(command.substr(0, 4));
        if (voltValue > 12){
            voltValue = 12.0f;    
        }
        printf("setting max Volt to %f \n\r",voltValue);
        //psu_set_voltage(voltValue*1000);
        //GUI_Draw_voltage_limit(voltValue*1000,false,0);
        //eeprom_write_max_voltage_value(voltValue*1000);
        return;
    }
    
   /* 
    c = "ISET";
    isEqual = true;
    for (uint i = 0 ; i < strlen(c);i++){
        if (toupper(comand[i]) != toupper(c[i])){
            isEqual = false; 
        }
    }
    
    if (isEqual){
        float curretnValue = atof((char*)comand+strlen(c));
        if (curretnValue > 500){
            curretnValue = 500.0f;   
        }
        printf("setting max Current to %f \n\r",curretnValue);
        //psu_set_current(curretnValue);
        //GUI_Draw_current_limit(curretnValue,false,0);
        //eeprom_write_max_current_value(curretnValue);

        return;
    }
    
    c = "ON";
    isEqual = true;
    for (uint i = 0 ; i < strlen(c);i++){
        if (toupper(comand[i]) != toupper(c[i])){
            isEqual = false; 
        }
    }
    
    if (isEqual){
        printf("turning on Powersupply \n\r");
        //psu_start();
        //GUI_Draw_on_off(true);
        
        return;
    }
    
        c = "OFF";
    isEqual = true;
    for (uint i = 0 ; i < strlen(c);i++){
        if (toupper(comand[i]) != toupper(c[i])){
            isEqual = false; 
        }
    }
    
    if (isEqual){
        printf("turning off Powersupply \n\r");
        //psu_stop();
        //GUI_Draw_on_off(false);
        return;
    }
    
    c = "IGET";
    isEqual = true;
    for (uint i = 0 ; i < strlen(c);i++){
        if (toupper(comand[i]) != toupper(c[i])){
            isEqual = false; 
        }
    }
    
    if (isEqual){
        
        float current = 1337;//analog_get_current();
        char  s [20]; 
        sprintf(s,"\r\n%f\r\n",current);
        
        printf(s);
        return;
    }
    
    c = "VGET";
    isEqual = true;
    for (uint i = 0 ; i < strlen(c);i++){
        if (toupper(comand[i]) != toupper(c[i])){
            isEqual = false; 
        }
    }
    
    if (isEqual){
        float volt = 1337;//analog_get_voltage();
        char  s [20]; 
        sprintf(s,"\r\n%f\r\n",volt/1000);
        printf(s);
        return;
    }
    
        c = "BATGET";
    isEqual = true;
    for (uint i = 0 ; i < strlen(c);i++){
        if (toupper(comand[i]) != toupper(c[i])){
            isEqual = false; 
        }
    }
    
    if (isEqual){
        float cap = 1337;//battery_get_procentage();
        char  s [20]; 
        sprintf(s,"\r\n%f\r\n",cap);
        printf(s);
        return;
    }
    
    
    printf("invalid comand: \r\n"); */



}