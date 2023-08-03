#ifndef BOOSTER_H
#define BOOSTER_H

#include "pico/stdlib.h"

class Booster{
    private:
        static const uint32_t update_freq_us = 500; 
        static const int swiching_freq_ticks = 1000; // 1000 = 133kHz

        static float targetVoltage; 
        
        static uint32_t update_timer; 
        static float pwm_value; 
        static uint pwm_slice_num;
    public: 
        static void Init();
        static void SetVoltage(float Voltage_V);
        static void Update();
        static float GetVoltage();
};

#endif
