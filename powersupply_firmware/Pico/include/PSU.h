#ifndef PSU_H
#define PSU_H

#include "pico/stdlib.h"

class PSU{
    private:
    static const int update_freq_us = 10*1000;
    static uint32_t update_timer;
    static float targetVoltage; 
    static float targetCurrent; 
    static bool enabled; 

    public: 
    static void Init();
    static void Update();

    static float getVoltage();
    static float getCurrent();
    static float getPower();
    static float getTargetVoltage();
    static float getTargetCurrent(); 
    static void SetVoltage(float voltage_V);
    static void SetCurrent(float current_mA); 

    static void Enable();
    static void Disable(); 
};


#endif