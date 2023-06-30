#include "PSU.h"
#include "LinReg.h"
#include "Booster.h"
#include "Pcb.h"
#include "Flash.h"
#include <algorithm>
#include <math.h>
#include "Powersaver.h"
#include "Battery.h"

uint32_t PSU::update_timer = 0;
float PSU::targetVoltage = 0;
float PSU::targetCurrent = 0;
bool PSU::enabled = false;
float PSU::linregDrop = 1; 

const float PSU::maxVoltage = 15.0f;
const float PSU::maxCurrent = 1000.0f;

float PSU::lowpassFilterMemoryVoltage = 0.0f;
float PSU::lowpassFilterMemoryCurrent =0.0f; 

bool PSU::isCC = false; 

void PSU::Init(){
    LinReg::Init();
    Booster::Init();

    gpio_init(Pcb::ouput_on_off_led_pin);
    gpio_set_dir(Pcb::ouput_on_off_led_pin, GPIO_OUT);

    enabled = false;
    SetVoltage(Flash::GetOutputVoltage());
    SetCurrent(Flash::GetOutputCurrent());
}

void PSU::Update(){
    Booster::Update();
    LinReg::Update();

    if(time_us_32() - update_timer > update_freq_us){
        update_timer = time_us_32();

        if(enabled){
            LinReg::SetVoltage(targetVoltage);
            LinReg::SetCurrent(targetCurrent);
            /*
            if(LinReg::GetVoltage() >targetVoltage){
                linregDrop-= 0.0001;
            }else{
                linregDrop+= 0.0001;
            }


            if(linregDrop>1.0f){
                linregDrop = 1.0;
            }
            if (linregDrop < 0.5){
                linregDrop = 0.2;
            }*/
            
            linregDrop = 0.9f; // debug
            float booster_voltage =targetVoltage+linregDrop;
            float output_voltage = LinReg::GetVoltage(); 
            booster_voltage = std::min(output_voltage+ linregDrop, booster_voltage); // drop voltage in current limiting mode 
            float battery_voltage = Battery::GetVoltage();
            if (booster_voltage < battery_voltage){  // boster does never start regulating if boos
                booster_voltage = battery_voltage;
            }

            Booster::SetVoltage(booster_voltage);
            gpio_put(Pcb::ouput_on_off_led_pin, true);
            Powersaver::Reset_idle_timer();

        }else{
            LinReg::SetVoltage(0);
            LinReg::SetCurrent(0);
            Booster::SetVoltage(0);
            gpio_put(Pcb::ouput_on_off_led_pin, false);
        }
        
        // make sure lowpass filter is updated 
        getVoltage();
        getCurrent();
        
        // check for constant current mode
        isCC = (targetCurrent-getCurrentSmooth() <= 2) && (targetVoltage - getVoltageSmooth() >= 0 && enabled);

    }
}

float PSU::getVoltage(){
    float v = LinReg::GetVoltage();
    lowpassFilterMemoryVoltage += (v - lowpassFilterMemoryVoltage)*0.1;
    return v;
}

float PSU::getCurrent(){
    float i = LinReg::GetCurrent();
    lowpassFilterMemoryCurrent += (i - lowpassFilterMemoryCurrent)*0.1;
    return i; 
}

float PSU::getVoltageSmooth(){
    return lowpassFilterMemoryVoltage;
}

float PSU::getCurrentSmooth(){
    return lowpassFilterMemoryCurrent;
}

float PSU::getPower(){
    return (getCurrent() / 1000) * getVoltage();
}

float PSU::getTargetVoltage(){
    return targetVoltage;
}

float PSU::getTargetCurrent(){
    return targetCurrent;
}

void PSU::SetVoltage(float voltage_V){
    targetVoltage = voltage_V;
}

void PSU::SetCurrent(float current_mA){
    targetCurrent = current_mA;
}

void PSU::Enable(){
    enabled = true;
}

void PSU::Disable(){
    enabled = false;
}

bool PSU::IsEnabled(){
    return enabled;
}

void PSU::ChangeVoltage(float voltage_v){
    targetVoltage += voltage_v;
    if (targetVoltage > maxVoltage)
        targetVoltage = maxVoltage;

    if(targetVoltage < 0)
        targetVoltage = 0;
}

void PSU::ChangeCurrent(float current_mA){
    targetCurrent += current_mA;
    if (targetCurrent > maxCurrent)
        targetCurrent = maxCurrent;

    if(targetCurrent < 0)
        targetCurrent = 0;
}

bool PSU::IsCC(){
    return isCC;
}