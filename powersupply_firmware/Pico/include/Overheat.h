#ifndef OVERHEAT_H
#define OVERHEAT_H

#include "pico/stdlib.h"

class Overheat{
    private:
        static const uint32_t update_freq_us = 500*1000;
        static uint32_t update_timer;
         
        static float maxTemp;
        static int32_t hysteresisTimer;

    public:
        static void Init();
        static void Update();
        static bool IsOverheated();
};

#endif