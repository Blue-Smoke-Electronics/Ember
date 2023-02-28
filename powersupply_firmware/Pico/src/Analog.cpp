#include "Analog.h"
#include "Pcb.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

float Analog::zeroCurrentReading = 0.0f; 
float Analog::zeroVoltageReading = 0.0f; 

void Analog::Init(){
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_gpio_init(Pcb::booster_voltage_sens_pin);
    adc_gpio_init(Pcb::battery_voltage_sens_pin);
    adc_gpio_init(Pcb::output_voltage_sens_pin);
    adc_gpio_init(Pcb::output_current_sens_pin);

    zeroCurrentReading = GetOutputCurrent();
    zeroVoltageReading = GetOutputVoltage();

}


float Analog::GetTemp(){
    adc_select_input(Pcb::temprature_adc_channal);
    uint16_t raw_temp_data = adc_read();
    float voltage = raw_temp_data * 3.3f / (1 << 12);
    float temp = 27 - (voltage - 0.706) / 0.001721;
    return temp; 
}

float Analog::GetOutputCurrent(){ // output in mA
    adc_select_input(Pcb::output_current_sens_adc_channal);
    
    int16_t raw_output_current = adc_read()-6;
    int16_t adjusted_raw_output_current; 
    // compansate for bad DNL  on spesific values 
    if (raw_output_current < 512){
        adjusted_raw_output_current = raw_output_current-5;
    }
    else if ( raw_output_current< 1536){
        adjusted_raw_output_current = raw_output_current+6;
    }
    else if ( raw_output_current < 2560){
        adjusted_raw_output_current = raw_output_current+14;
    }
    else if ( raw_output_current < 3584){
        adjusted_raw_output_current = raw_output_current+22;
    }
    else {
        adjusted_raw_output_current = raw_output_current+30;
    }


    float output_current = adjusted_raw_output_current * 3.3f / (1 << 12) / 2.5f;
    output_current -=  0.0043f; // current trought constat current circuit 
    if (output_current < 0.0f)
        output_current = 0;

    output_current = output_current*(1-0.01); // ajust for messured slope in ouput current graph. might be doue to resistance in solder joints
    return output_current * 1000 - zeroCurrentReading;
}

float Analog::GetOutputVoltage(){
    adc_select_input(Pcb::output_voltage_sens_adc_channal);
    int16_t raw_output_voltage = adc_read();
    int16_t adjusted_raw_output_voltage; 
    // compansate for bad DNL  on spesific values 
    if (raw_output_voltage < 512){
        adjusted_raw_output_voltage = raw_output_voltage-5;
    }
    else if ( raw_output_voltage < 1536){
        adjusted_raw_output_voltage = raw_output_voltage+6;
    }
    else if ( raw_output_voltage < 2560){
        adjusted_raw_output_voltage = raw_output_voltage+14;
    }
    else if ( raw_output_voltage < 3584){
        adjusted_raw_output_voltage = raw_output_voltage+22;
    }
    else {
        adjusted_raw_output_voltage = raw_output_voltage+30;
    }
 


    float output_voltage = adjusted_raw_output_voltage * 3.3f / (1 << 12) * 6.0f;
    if (output_voltage < 0.0f)
        output_voltage = 0.0f;
      
    return output_voltage - zeroVoltageReading;
}

float Analog::GetBoosterVoltage(){
    adc_select_input(Pcb::booster_voltage_sens_adc_channal);
    uint16_t booster_voltage_raw = adc_read(); 
    float booster_voltage =   (float)booster_voltage_raw * 3.3f/(1<<12) *11; // reading to high values ??? 
    if(booster_voltage < 0.0f){
        booster_voltage = 0.0f;
    }
    return booster_voltage; 
}

float Analog::GetBatteryVoltage(){
    adc_select_input(Pcb::battery_voltage_sens_adc_channal);
    uint16_t battery_voltage_raw = adc_read(); 
    float battery_voltage =   (float)battery_voltage_raw * 3.3f/(1<<12) *2; // reading to high values ??? 
    if(battery_voltage < 0.0f){
        return 0.0f; 
    }
    return battery_voltage; 
}
