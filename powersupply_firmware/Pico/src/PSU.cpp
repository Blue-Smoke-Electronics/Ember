#include "PSU.h"
#include "LinReg.h"
#include "Booster.h"
#include "Pcb.h"
#include "Flash.h"
#include <algorithm>

uint32_t PSU::update_timer = 0;
float PSU::targetVoltage = 0;
float PSU::targetCurrent = 0;
bool PSU::enabled = false;

const float PSU::maxVoltage = 15.0f;
const float PSU::maxCurrent = 999.0f;

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
            Booster::SetVoltage(std::min(LinReg::GetVoltage() + 3.0f, targetVoltage+2.2f));
            gpio_put(Pcb::ouput_on_off_led_pin, true);
        }else{
            LinReg::SetVoltage(0);
            LinReg::SetCurrent(0);
            Booster::SetVoltage(0);
            gpio_put(Pcb::ouput_on_off_led_pin, false);
        }
    }
}

float PSU::getVoltage(){
    return LinReg::GetVoltage();
}

float PSU::getCurrent(){
    return LinReg::GetCurrent();
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
