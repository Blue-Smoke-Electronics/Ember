#ifndef ONOFF_H
#define ONOFF_H

#include "pico/stdlib.h"

class Onoff{
    private:
        //static bool watchdogenabled;
        static bool skip_next_disable;

        static const uint32_t update_freq_us = 100*1000;
        static uint32_t update_timer;

    public:
        static bool IsOn;
        
        static void Init();
        static void Update();
        static void Turn_off_device();
        static void Turn_on_device();
        
};

#endif
