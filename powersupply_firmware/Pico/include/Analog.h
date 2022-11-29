#ifndef ANALOG_H
#define ANALOG_H

class Analog{
    private:
        static float zeroCurrentReading; 
        static float zeroVoltageReading;
    public:
        static void Init();
        static float GetTemp();
        static float GetOutputCurrent();
        static float GetOutputVoltage();
        static float GetBoosterVoltage();
        static float GetBatteryVoltage();
};

#endif