#ifndef ANALOG_H
#define ANALOG_H

class Analog{
    private:
    public:
        static void Init();
        static float GetTemp();
        static float GetOutputCurrent();
        static float GetOutputVoltage();
        static float GetBoosterVoltage();
        static float GetBatteryVoltage();
};

#endif