#include "Analog.h"
#include "Pcb.h"
#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "flash.h"
float Analog::zeroCurrentReading = 0.0f; 
float Analog::zeroVoltageReading = 0.0f; 

float Analog::voltage_calibrations[21]; 
float Analog::current_callibrations[16]; 
void Analog::Init(){
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_gpio_init(Pcb::booster_voltage_sens_pin);
    adc_gpio_init(Pcb::battery_voltage_sens_pin);
    adc_gpio_init(Pcb::output_voltage_sens_pin);
    adc_gpio_init(Pcb::output_current_sens_pin);

    zeroCurrentReading = GetOutputCurrent();
    zeroVoltageReading = GetOutputVoltage();

    for(int i =0; i < 21;i++){
        voltage_calibrations[i] = Flash::GetVoltageCalibrations()[i];
        if (voltage_calibrations[i] > i + 0.5  || voltage_calibrations[i] < i - 0.5 ||isnan(voltage_calibrations[i]) ){
            voltage_calibrations[i] =i; 
        }
    }
        for(int i =0; i < 16;i++){
        current_callibrations[i] = Flash::GetCurrentCalibrations()[i];
        if (current_callibrations[i] > i*100 + 50  || current_callibrations[i] < i*100.0f - 50.0f || isnan(current_callibrations[i])){
            current_callibrations[i] =i*100; 
        }
        
    }

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
    
    int16_t raw_output_current = adc_read();
    int16_t adjusted_raw_output_current = fix_adc_unliniarity(raw_output_current);


    float output_current = (adjusted_raw_output_current * 3.3f / (1 << 12) / 2.5f)*1000.0f;
    output_current -=  4.3f; // current trought constat current circuit 
    output_current = calibrate_current(output_current);
    if (output_current < 0.0f)
        output_current = 0;



    //output_current = output_current*(1-0.01); // ajust for messured slope in ouput current graph. might be doue to resistance in solder joints
    
    return output_current;// - zeroCurrentReading;
}

float Analog::GetOutputVoltage(){
    adc_select_input(Pcb::output_voltage_sens_adc_channal);
    int16_t raw_output_voltage = adc_read();
    int16_t adjusted_raw_output_voltage = fix_adc_unliniarity(raw_output_voltage);

    float output_voltage = adjusted_raw_output_voltage * 3.3f / (1 << 12) * 6.0f;
    float calibrated_voltage = calibrate_voltage(output_voltage);
    if (calibrated_voltage < 0.0f)
        calibrated_voltage = 0.0f;

    if (calibrated_voltage > 20.0f)
        calibrated_voltage = 20.0f;
      
    

    return calibrated_voltage;//- zeroVoltageReading;
}

float Analog::GetBoosterVoltage(){
    adc_select_input(Pcb::booster_voltage_sens_adc_channal);
    int16_t booster_voltage_raw = adc_read();
    int16_t adjusted_raw_output_voltage; 

    float output_voltage = fix_adc_unliniarity(booster_voltage_raw) * 3.3f / (1 << 12) * 11.0f;
    if (output_voltage < 0.0f)
        output_voltage = 0.0f;
    
    if (output_voltage > 25.0f)
        output_voltage = 25.0f;
      
    return output_voltage;
}

float Analog::GetBatteryVoltage(){
    adc_select_input(Pcb::battery_voltage_sens_adc_channal);
    uint16_t battery_voltage_raw = adc_read(); 
    float battery_voltage =   (float)battery_voltage_raw * 3.3f/(1<<12) *2; // reading to high values ??? 
    if(battery_voltage < 0.0f){
        return 0.0f; 
    }
    if(battery_voltage > 5.0f){
        return 5.0f; 
    }
    return battery_voltage; 
}

uint16_t Analog::fix_adc_unliniarity(uint16_t raw_adc_output){
    if (raw_adc_output < 512){
        return raw_adc_output-5;
    }
    if ( raw_adc_output < 1536){
        return raw_adc_output+6;
    }
    if ( raw_adc_output < 2560){
        return raw_adc_output+14;
    }
    if ( raw_adc_output < 3584){
        return raw_adc_output+22;
    }
    return raw_adc_output+30;
    
}

float Analog::calibrate_voltage(float voltage){
    int x0 = std::floor(voltage); 
    int x1 = std::ceil(voltage);
    float y0 = voltage_calibrations[x0];
    float y1 = voltage_calibrations[x1];


    return interpolate(voltage,x0,  y0,  x1, y1);
}

float Analog::calibrate_current(float current){
    int x0 = std::floor(current/100); 
    int x1 = std::ceil(current/100);
    float y0 = current_callibrations[x0];
    float y1 = current_callibrations[x1];


    return interpolate(current/100,x0,  y0,  x1, y1);
}

float Analog::interpolate(float x, float x0, float y0, float x1, float y1){
    return y0 + ((y1-y0)/(x1-x0)) * (x - x0);
}

void Analog::VoltageCalibration(int voltage, float diff){
    voltage_calibrations[voltage] = voltage-diff; 
    Flash::SaveVoltageCalibrations(voltage_calibrations);
}
void Analog::CurrentCalibration(int current, float diff){
    current_callibrations[current/100] = current - diff; 
    Flash::SaveCurrentCalibrations(current_callibrations);
}

