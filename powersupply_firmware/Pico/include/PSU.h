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
        static float linregDrop;
        static float lowpassFilterMemoryVoltage;
        static float lowpassFilterMemoryCurrent; 
        static bool isCC;


    public:
        static const float maxVoltage;
        static const float maxCurrent;
        static void Init();
        static void Update();

        static float getVoltage();
        static float getCurrent();
        static float getVoltageSmooth();
        static float getCurrentSmooth();
        static float getPower();
        static float getTargetVoltage();
        static float getTargetCurrent();
        static bool IsCC();
        static void SetVoltage(float voltage_V);
        static void SetCurrent(float current_mA);
        static void ChangeVoltage(float voltage_v);
        static void ChangeCurrent(float current_mA);
        

        static void Enable();
        static void Disable();
        static bool IsEnabled();
};


#endif
