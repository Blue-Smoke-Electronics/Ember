#ifndef POWERSAVER_H
#define POWERSAVER_H

#include "pico/stdlib.h"


class Powersaver{
   private:
    static const int power_off_time  = 15*60; // secounds 
    static const int display_off_time  = 5*60; // secounds 
    static float idle_timer;
    static uint32_t update_timer; 
     

   public: 
    static void Init();
    static void Reset_idle_timer();
    static void Update();
};

#endif