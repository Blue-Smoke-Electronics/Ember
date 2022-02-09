/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

/* [] END OF FILE */
#include <project.h>
#include "eeprom.h"


void eeprom_init(){
    EEPROM_Start();
    EEPROM_UpdateTemperature();
}

int eeprom_read_int(int address){
    return (EEPROM_ReadByte(address)<<0)|(EEPROM_ReadByte(address+1)<<8)|(EEPROM_ReadByte(address+2)<<16)|(EEPROM_ReadByte(address+3)<<24) ; 
}
void eeprom_write_int(int address,int data){
    EEPROM_WriteByte(data & 0xFF,address);
    EEPROM_WriteByte((data>>8) & 0xFF,address+1);
    EEPROM_WriteByte((data>>16) & 0xFF,address+2);
    EEPROM_WriteByte((data>>24) & 0xFF,address+3);
}


int eeprom_read_max_voltage_value(){
    return eeprom_read_int(0);
}


int eeprom_read_max_current_value(){
    return eeprom_read_int(4);
}


int eeprom_read_encoder_gain_voltage(){
    return eeprom_read_int(8);
}


int eeprom_read_encoder_gain_current(){
    return eeprom_read_int(12);
}
int eeprom_read_battery_capasity(){
    return eeprom_read_int(16);
}



void eeprom_write_max_voltage_value(int mV){
    eeprom_write_int(0,mV);
}


void eeprom_write_max_current_value(int mA){
    eeprom_write_int(4,mA);
}


void eeprom_write_encoder_gain_voltage(int gain){
    eeprom_write_int(8,gain);
}


void eeprom_write_encoder_gain_current(int gain){
    eeprom_write_int(12,gain);
}

void eeprom_write_battery_capasity(int capasity){
    eeprom_write_int(16,capasity);
}


