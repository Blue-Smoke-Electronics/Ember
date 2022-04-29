#ifndef GUD_H
#define GUD_H

#include "Display.h"
#include "Font.h"
#include"Sprite.h"



class GUI{
    private:
        enum class State { booting, running, menu, charging };

        const int boot_time_us = 2*1000*1000;

        Font big_font; 
        Font Smal_font;
        Sprite logo; 
        
        State state; 
        uint32_t boot_start_time; 

        bool newState; 

        uint32_t update_timer;
        const uint32_t update_freq_us = 100*1000; 

        void Show_boot_screen();

    public:
        GUI(); 
        
        void Update();

};



#endif
