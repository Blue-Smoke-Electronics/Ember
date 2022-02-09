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

#ifndef EEPROM_H
#define EEPROM_H
    

void eeprom_init();

int eeprom_read_int(int address);
void eeprom_write_int(int address,int data);

int eeprom_read_max_voltage_value();
int eeprom_read_max_current_value();
int eeprom_read_encoder_gain_voltage();
int eeprom_read_encoder_gain_current();

void eeprom_write_max_voltage_value(int mV);
void eeprom_write_max_current_value(int mA);
void eeprom_write_encoder_gain_voltage(int gain);
void eeprom_write_encoder_gain_current(int gain);
void eeprom_write_battery_capasity(int capasity);
int eeprom_read_battery_capasity();
    
    
#endif