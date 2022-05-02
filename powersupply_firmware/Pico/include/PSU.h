#ifndef PSU_H
#define PSU_H

class PSU{
    private:

    public: 
    static void Init();
    static void Update();

    static float getVoltage();
    static float getCurrent();
    static float getPower();
    static void SetVoltage(float voltage_V);
    static void SetCurrent(float current_mA); 
};


#endif