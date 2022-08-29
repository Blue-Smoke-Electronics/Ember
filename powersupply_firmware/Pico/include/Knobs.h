#ifndef KNOBS_H
#define KNOBS_H
#include "pico/stdlib.h"

class Knobs{
    private:
        

        static uint32_t update_timer;
        static const uint32_t update_freq_us = 100*1000; 

        static volatile bool on_off_switch_is_pushed;
        static int on_off_switch_skipp_first; 
        static volatile bool power_switch_is_pushed;  
        static volatile int voltage_encoder_cnt; 
        static volatile int current_encoder_cnt; 


        static void IRS(uint gpio, uint32_t events); 
        
    public:
        static void Init();
        static void Update();  

};
#endif