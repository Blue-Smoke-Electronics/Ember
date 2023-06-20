#ifndef ANALOG_H
#define ANALOG_H

#include "pico/stdlib.h"

class Analog{
    private:
        static float zeroCurrentReading; 
        static float zeroVoltageReading;
        static uint16_t fix_adc_unliniarity(uint16_t);
        static float  voltage_calibrations[];
        static float current_callibrations[];
        static float calibrate_voltage(float voltage);
        static float calibrate_current(float current);
        static float interpolate(float x, float x0, float y0, float x1, float y1);  

        


    public:
        static void Init();
        static float GetTemp();
        static float GetOutputCurrent();
        static float GetOutputVoltage();
        static float GetBoosterVoltage();
        static float GetBatteryVoltage();
        static void VoltageCalibration(int voltage, float messured_value);
        static void CurrentCalibration(int current, float messured_value);




};

#endif