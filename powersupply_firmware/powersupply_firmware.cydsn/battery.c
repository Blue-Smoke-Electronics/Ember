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

#include "project.h"
#include "battery.h"
#include "analog.h"
#include "psu.h"
#include "eeprom.h"
#include <stdio.h>

float battery_capasity_left; 
uint32 last_update_time ; 
uint32 last_eeprom_save_time; 
uint32 last_usb_check_time; 

void battery_init(){
    // load battery capaisty from eprom 
    eeprom_init();
    battery_capasity_left = eeprom_read_battery_capasity();
    analog_init();
    
}

void battery_liftime_update_loop(){
    int delta_t =last_update_time - Timer_now_ReadCounter(); 
    if ( delta_t < 500){
        return;    
    }
    last_update_time = Timer_now_ReadCounter();
    //printf("delta_t %d \r\n",delta_t);
    battery_capasity_left -=delta_t/1000.0f *  battery_get_total_power_draw();
    
    // handele owerflow and underflow errors
    if (battery_capasity_left > battery_get_max_capasity()){
        battery_capasity_left = battery_get_max_capasity();
    }
    if (battery_capasity_left < 0){
        battery_capasity_left = 0;
    }
    
    // reset when battery is full
    if (battery_get_voltage() > 4095){ // adc caped at 4096
        battery_capasity_left = battery_get_max_capasity();
    }
    if(battery_get_voltage() > 3920){
        battery_capasity_left = battery_get_max_capasity()*0.8f > battery_capasity_left ? battery_get_max_capasity()*0.8f :  battery_capasity_left; 
    }
     if(battery_get_voltage() > 3770){
        battery_capasity_left = battery_get_max_capasity()*0.6f > battery_capasity_left ? battery_get_max_capasity()*0.6f :  battery_capasity_left; 
    }
     if(battery_get_voltage() > 3700){
        battery_capasity_left = battery_get_max_capasity()*0.4f > battery_capasity_left ? battery_get_max_capasity()*0.4f :  battery_capasity_left; 
    }
     if(battery_get_voltage() > 3630){
        battery_capasity_left = battery_get_max_capasity()*0.2f > battery_capasity_left ? battery_get_max_capasity()*0.2f :  battery_capasity_left; 
    }
    
    if (battery_get_voltage() < 3300){
        battery_capasity_left = 0;
    }
    
    
    
    // save to eeprom once per 150 secound- this gives 5 years of contius on time before eeprom is used up
    if ( last_eeprom_save_time - Timer_now_ReadCounter() > 150000){
        last_eeprom_save_time = Timer_now_ReadCounter();
        eeprom_write_battery_capasity(battery_capasity_left);
    }
    
    //printf("capacity: %d\r\n",battery_capasity_left);
     
}


float battery_get_quisent_power(){
    //mW
    return 150*battery_get_voltage()/1000.0f; 
}
float battery_get_psu_quisent_power(){
    //mW
    return 10*psu_get_voltage()/1000.0f; 
}
float battery_get_charging_power(){
    //mW
    return 5*450;    
}

int usb_status; 
int battery_charger_connected(){ 
    if (last_usb_check_time - Timer_now_ReadCounter()  > 500){
         last_usb_check_time = Timer_now_ReadCounter();
        usb_status = USBUART_CheckActivity();
    }

    return usb_status; 
}

int battery_isCharging(){
    return battery_get_total_power_draw() <= 0; 
}

float battery_get_total_power_draw(){
    float totalpower =0; 
    
    totalpower += battery_get_quisent_power(); 
    if(psu_get_status()== psu_status_ON){ 
        totalpower += battery_get_psu_quisent_power() + psu_get_power();
    }
    if(battery_charger_connected()){
        totalpower -= battery_get_charging_power();
    }
    
    //printf("total power draw %d \r\n",totalpower);
    return totalpower;
}
float battery_get_max_capasity(){
    // in mJ
    return 3.7*4000*60*60; 
}

float battery_get_voltage(){
    return analog_get_battery_voltage();   
}



int battery_get_lifetime_s(){
    int lifetime=0;
    float powerdraw = battery_get_total_power_draw(); 
    if(powerdraw < 0){
        
       lifetime= (battery_get_max_capasity()- battery_capasity_left)/(-powerdraw);    
    }
    else{
        lifetime= battery_capasity_left/powerdraw;
    }
    if (lifetime > 172800) { // cap at 48 houers
        lifetime = 172800; 
    }
    if (lifetime < 0){
        lifetime =0; 
    }
    
    return lifetime; 
}

int battery_get_procentage(){
    printf("cap : %f maxcap: %f \r\n",battery_get_voltage(), battery_capasity_left);
    return (battery_capasity_left/battery_get_max_capasity())*100.0f;
}