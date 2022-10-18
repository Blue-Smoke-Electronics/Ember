#ifndef BATTERY_H
#define BATTERY_H

#include "pico/stdlib.h"

class Battery{
    private:
        
        static const float maxCapacity;
        static const uint32_t update_freq_us; 
        static uint32_t update_timer; 

        static float capasityLeft; 

        static float GetQuiescentPower();
        static float GetPsuQuiescentPower();
        static float GetChargingPower();
        static bool IsChargerConnected();
        static bool GetChargerStatus();
        static float GetTotalPowerDraw();

    public:
        static void Init();
        static void Update(); 

        static float GetCapasityLeft();
        static float GetBatteryLife_s();
        static float GetBatteryProcentage(); 
        static bool IsCharging();
        static float GetVoltage();
};

#endif
