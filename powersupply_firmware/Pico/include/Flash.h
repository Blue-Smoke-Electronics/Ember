#ifndef FLASH_H
#define FLASH_H

#include "pico/stdlib.h"
#include "hardware/flash.h"
#include "Sprite.h"
#include "Font.h"

class Flash{
    private: 
        
        static uint8_t write_buffer [] ;         

    public:
        static Sprite bootscreen; 
        static Sprite logo_text; 
        static Sprite logo_symbol; 
        static Sprite outputOnSymbol; 
        static Sprite outputOffSymbol; 
        static Sprite batterySymbol;
        static Sprite batterySymbol_bar;
        static Sprite batteryUpArrow;
        static Sprite batteryDownArrow;
        static Sprite batteryChargingSymbol;
        static Sprite chargingSymbol; 
        static Sprite selectedMarker;
        static Sprite overheatSymbol;
        
        static Font smallFont; 
        static Font timeFont; 
        static Font bigFont; 

        static float GetBatteryCapacity(); 
        static float GetOutputVoltage(); 
        static float GetOutputCurrent(); 
        static float * GetVoltageCalibrations();
        static float * GetCurrentCalibrations();

        static void Save(float batteryCapacity ,float outputVoltage, float outputCurrent); 
        static void SaveVoltageCalibrations(float* voltageCalibrations);
        static void SaveCurrentCalibrations(float* currentCalibrations);
};

#endif
