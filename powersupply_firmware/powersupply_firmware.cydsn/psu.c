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

#include "psu.h"
#include "booster.h"
#include "linReg.h"
#include "analog.h"


int output_state; 
int current_limit; 
int volt_limit; 

float current_reg =0; 
float volt_reg=0;

void psu_init(){
    LinReg_init();
    Booster_init();
    analog_init();
    psu_stop(); 
}


void psu_regulator_loop(){
    
    if(output_state == psu_status_OFF){
        return;
    }
    
    Booster_set_voltage(analog_get_voltage()+2500);
    if(analog_get_current() < current_limit){
        volt_reg += (volt_limit- analog_get_voltage())*0.2;
        if(volt_reg > 500){
            volt_reg = 500;
        }
        if (volt_reg < -500){
            volt_reg = -500;
        }
        linReg_voltage_limit_set(volt_limit +volt_reg); 
    }
        if(analog_get_voltage() < volt_limit){
        current_reg += (current_limit- analog_get_current())*0.8;
        if(current_reg > 150){
            current_reg = 150;
        }
        if (current_reg < -150){
            current_reg = -150;
        }
        linReg_current_limit_set(current_limit +current_reg); 
    }
    
}

void psu_start(){
    
    output_state = psu_status_ON; 
    psu_set_voltage(volt_limit);
    psu_set_current(current_limit);
    
}

void psu_stop(){
    output_state = psu_status_OFF;
    linReg_voltage_limit_set(0);
}

int psu_get_status(){
    
    return output_state; 
}

void psu_set_voltage(int mV){
    volt_limit = mV; 
    
    if(output_state == psu_status_ON){
        Booster_set_voltage(volt_limit+2000);
        linReg_voltage_limit_set(volt_limit+165);
    }
    
}
void psu_set_current(int mA){
    current_limit = mA;
    if(output_state == psu_status_ON){
        Booster_set_current_limit(current_limit+100);
        linReg_current_limit_set(current_limit);
    }
     
}

int psu_get_voltage(){
    return analog_get_voltage();   
}
int psu_get_current(){
    return analog_get_current();   
}
int psu_get_power(){
    return (psu_get_voltage()*psu_get_current()/1000.0f);
}
