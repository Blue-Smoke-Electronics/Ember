#ifndef HARTBEAT_H
#define HARTBEAT_H

#include "pico/stdlib.h"

class Hartbeat{
    private:
        const int update_freq_us = 1*1000*1000;

        //static void Hartbeat_task(void* param);
        uint32_t update_timer; 
        bool led_value; 

    public: 
        Hartbeat();
        void Update(); 
};

#endif