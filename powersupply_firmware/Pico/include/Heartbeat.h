#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include "pico/stdlib.h"

class Heartbeat{
    private:
        const int update_freq_us = 1*1000*1000;

        //static void Heartbeat_task(void* param);
        uint32_t update_timer; 
        bool led_value; 

    public: 
        Heartbeat();
        void Update(); 
};

#endif
