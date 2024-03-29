#include "Booster.h"
#include "Analog.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "Pcb.h"
#include "stdio.h"

float Booster::targetVoltage;
uint32_t Booster::update_timer;
float Booster::pwm_value;
uint Booster::pwm_slice_num;

void Booster::Init(){
    Analog::Init();

    // enable booster pwm
    gpio_set_function(Pcb::booster_pwm_pin,GPIO_FUNC_PWM);
    pwm_slice_num = pwm_gpio_to_slice_num(Pcb::booster_pwm_pin);
    pwm_set_wrap(pwm_slice_num, 1000); // 133kHz
    pwm_set_chan_level(pwm_slice_num,pwm_gpio_to_channel(Pcb::booster_pwm_pin), 0);
    pwm_set_enabled(pwm_slice_num, true);

    pwm_value = 0;
    targetVoltage = 0.0f;
}

void Booster::SetVoltage(float Voltage_V){
    targetVoltage = Voltage_V;
}

float Booster::GetVoltage(){
    return Analog::GetBoosterVoltage();
}

void Booster::Update(){
    if(time_us_32() - update_timer > update_freq_us ){
        uint32_t us = time_us_32() - update_timer ;
        update_timer = time_us_32();

        float v = GetVoltage();
        float diff = targetVoltage - v;
        float change = (diff)*100.f;
        if (change > 0){
            change =1; 
        }
        if (change < -0){
            change = -1; 
        }
        pwm_value += change;
          
        if (pwm_value > 900){
            pwm_value = 900;
        }
        if (pwm_value < 0){
            pwm_value = 0; 
        }

        if (GetVoltage() > targetVoltage + 2) // somtihg is wrong, turn of booster
            pwm_value = 0;

        
        pwm_set_chan_level(pwm_slice_num,pwm_gpio_to_channel(Pcb::booster_pwm_pin), pwm_value);
    }
}
