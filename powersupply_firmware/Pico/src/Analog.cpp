#include "Analog.h"
#include "Pcb.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

void Analog::Init(){
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_gpio_init(Pcb::booster_voltage_sens_pin);
    adc_gpio_init(Pcb::battery_voltage_sens_pin);
    adc_gpio_init(Pcb::output_voltage_sens_pin);
    adc_gpio_init(Pcb::output_current_sens_pin);
}
float Analog::GetTemp(){
    adc_select_input(Pcb::temprature_adc_channal);
    uint16_t raw_temp_data = adc_read();
    float voltage = raw_temp_data * 3.3f/(1<<12);
    float temp = 27-(voltage-0.706)/0.001721;
    return temp; 
}
float Analog::GetOutputCurrent(){ // output in mA
    adc_select_input(Pcb::output_current_sens_adc_channal);
    uint16_t raw_output_current = adc_read();
    float output_current = raw_output_current * 3.3f/(1<<12)/2.5f;
    return output_current*1000; 
    
}
float Analog::GetOutputVoltage(){
    adc_select_input(Pcb::output_voltage_sens_adc_channal);
    uint16_t raw_output_voltage = adc_read();
    float output_voltage = raw_output_voltage * 3.3f/(1<<12)*11.0f;
    return output_voltage; 
    
}
float Analog::GetBoosterVoltage(){
    adc_select_input(Pcb::booster_voltage_sens_adc_channal);
    uint16_t booster_voltage_raw = adc_read(); 
    float booster_voltage =   (float)booster_voltage_raw * 3.3f/(1<<12) *11; // reading to high values ??? 
    return booster_voltage; 
}
float Analog::GetBatteryVoltage(){
    adc_select_input(Pcb::battery_voltage_sens_adc_channal);
    uint16_t battery_voltage_raw = adc_read(); 
    float battery_voltage =   (float)battery_voltage_raw * 3.3f/(1<<12) *11; // reading to high values ??? 
    return battery_voltage; 
}