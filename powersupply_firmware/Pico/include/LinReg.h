#ifndef LINREG_H
#define LINREG_H

#include "pico/stdlib.h"

class LinReg{
    private:
    static uint voltage_limit_pwm_slice_num;
    static uint current_limit_pwm_slice_num; 

    public: 
    static void Init();
    static float GetVoltage();
    static float GetCurrent();
    static void SetVoltage(float voltage_V);
    static void SetCurrent(float current_mA); 
};

#endif