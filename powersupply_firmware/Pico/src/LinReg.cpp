#include "LinReg.h"
#include "Analog.h"
#include "Pcb.h"
#include "hardware/pwm.h"

uint LinReg::voltage_limit_pwm_slice_num;
uint LinReg::current_limit_pwm_slice_num;

void LinReg::Init(){
    // enable voltage pwm
    gpio_set_function(Pcb::voltage_limit_pwm_pin,GPIO_FUNC_PWM);
    voltage_limit_pwm_slice_num = pwm_gpio_to_slice_num(Pcb::voltage_limit_pwm_pin);
    pwm_set_wrap(voltage_limit_pwm_slice_num,10000); // 13.3kHz
    pwm_set_enabled(voltage_limit_pwm_slice_num,true);

    // enabel current pwm
    gpio_set_function(Pcb::current_limit_pwm_pin,GPIO_FUNC_PWM);
    current_limit_pwm_slice_num = pwm_gpio_to_slice_num(Pcb::current_limit_pwm_pin);
    pwm_set_wrap(current_limit_pwm_slice_num,10000); // 13.3kHz
    pwm_set_enabled(current_limit_pwm_slice_num,true);

}

float LinReg::GetVoltage(){
    return Analog::GetOutputVoltage();
}

float LinReg::GetCurrent(){
    return Analog::GetOutputCurrent();
}

void LinReg::SetVoltage(float voltage_V){
    pwm_set_chan_level(voltage_limit_pwm_slice_num,pwm_gpio_to_channel(Pcb::voltage_limit_pwm_pin),(voltage_V/4.7037)*(10000/3.3f));
}


void LinReg::SetCurrent(float current_mA){
   pwm_set_chan_level(current_limit_pwm_slice_num,pwm_gpio_to_channel(Pcb::current_limit_pwm_pin),(current_mA*2.5f)*(10/3.3f)); 
}


