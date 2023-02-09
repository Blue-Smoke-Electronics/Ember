#include "LinReg.h"
#include "Analog.h"
#include "Pcb.h"
#include "hardware/pwm.h"
#include "stdio.h"

uint LinReg::voltage_limit_pwm_slice_num;
uint LinReg::current_limit_pwm_slice_num;

uint32_t LinReg::update_timer; 

float LinReg::targetVoltage = 0;
float LinReg::targetCurrent = 0;

float LinReg::voltageCorrection = 0;
float LinReg::currentCorrection = 0;

void LinReg::Init(){
    // enable voltage pwm
    gpio_set_function(Pcb::voltage_limit_pwm_pin, GPIO_FUNC_PWM);
    voltage_limit_pwm_slice_num = pwm_gpio_to_slice_num(Pcb::voltage_limit_pwm_pin);
    pwm_set_wrap(voltage_limit_pwm_slice_num, 10000); // 13.3kHz
    pwm_set_enabled(voltage_limit_pwm_slice_num, true);

    // enabel current pwm
    gpio_set_function(Pcb::current_limit_pwm_pin, GPIO_FUNC_PWM);
    current_limit_pwm_slice_num = pwm_gpio_to_slice_num(Pcb::current_limit_pwm_pin);
    pwm_set_wrap(current_limit_pwm_slice_num, 10000); // 13.3kHz
    pwm_set_enabled(current_limit_pwm_slice_num, true);

    // compansate for constant current draw
    currentCorrection = -5; 
}
 
void LinReg::Update(){
    if(time_us_32() - update_timer > update_freq_us){
        update_timer = time_us_32();

        voltageCorrection += (targetVoltage - GetVoltage()) * 0.1f;
        if(voltageCorrection > 0.5f)
            voltageCorrection = 0.5f;

        if(voltageCorrection < -0.5f)
            voltageCorrection = -0.5f;

        currentCorrection += (targetCurrent - GetCurrent()) * 0.002f;
        if(currentCorrection > 20.0f)
            currentCorrection = 20.0f;

        if(currentCorrection < -20.0f)
            currentCorrection = -20.0f;

        pwm_set_chan_level(voltage_limit_pwm_slice_num, pwm_gpio_to_channel(Pcb::voltage_limit_pwm_pin), ((targetVoltage + voltageCorrection) / 4.7037) * (10000 / 3.3f));
        pwm_set_chan_level(current_limit_pwm_slice_num, pwm_gpio_to_channel(Pcb::current_limit_pwm_pin), ((targetCurrent + currentCorrection) * 2.5f) * (10 / 3.3f));
        }
}

float LinReg::GetVoltage(){
    return Analog::GetOutputVoltage();
}

float LinReg::GetCurrent(){
    return Analog::GetOutputCurrent();
}

void LinReg::SetVoltage(float voltage_V){
    targetVoltage = voltage_V;
    /*targetVoltage =(targetVoltage *100.0f + 0.5f); 
    int tv = (int)targetVoltage;
    targetVoltage = (tv + 0.5f)/100;
    if(voltage_V < 0.01f){
        targetVoltage =0; 
    }*/
}

void LinReg::SetCurrent(float current_mA){
    targetCurrent = current_mA;
    /*targetCurrent =(targetCurrent *100.0f + 0.5f);
    int tc = (int)targetCurrent;
    targetCurrent = (tc + 0.5f)/100;
    if (current_mA < 1){
        targetCurrent =0; 
    }*/
}
