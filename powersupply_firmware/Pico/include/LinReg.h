#ifndef LINREG_H
#define LINREG_H

#include "pico/stdlib.h"

class LinReg{
    private:
    static uint voltage_limit_pwm_slice_num;
    static uint current_limit_pwm_slice_num; 

    static float targetVoltage;
    static float targetCurrent; 

    static float voltageCorrection; 
    static float currentCorrection; 

    static const uint32_t update_freq_us = 1000*10; 
    static uint32_t update_timer; 

    public: 
    static void Init();
    static void Update();
    static float GetVoltage();
    static float GetCurrent();
    static void SetVoltage(float voltage_V);
    static void SetCurrent(float current_mA); 
};

#endif