#ifndef ONOFF_H
#define ONOFF_H

#include "pico/stdlib.h"

class Onoff{
    private: 
        void turn_off_device();
        static void IRS(uint gpio, uint32_t event);
        static uint32_t debounce_timestamp; 

    public: 
        Onoff(); 

};


#endif