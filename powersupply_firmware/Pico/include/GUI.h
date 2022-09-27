#ifndef GUD_H
#define GUD_H

#include "Display.h"
#include "Font.h"
#include"Sprite.h"

class GUI{
    private:
        enum class State { booting, running, menu, charging };

        static const int boot_time_us = 200*1000;

        static Font big_font; 
        static Font Smal_font;
        static Sprite logo; 
        
        static State state; 
        static uint32_t boot_start_time; 

        static bool newState; 

        static uint32_t update_timer;
        static const uint32_t update_freq_us = 100*1000; 

        static float voltageScaler; 
        static float currentScaler; 

        void Show_boot_screen();

    public: 
        static void Init(); 
        static void Update();

        static void ChangeVoltageScaler();
        static float GetVoltageScaler();  
        static void ChangeCurrentScaler();
        static float GetCurrentScaler();

};

#endif
