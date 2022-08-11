#ifndef ONOFF_H
#define ONOFF_H

#include "pico/stdlib.h"

class Onoff{
    private: 
        static bool watchdogenabled; 
        static bool skip_next_disable; 
        

    public: 
        static bool IsOn; 
        
        static void Init(); 
        static void Turn_off_device();
        static void Turn_on_device();
        

};


#endif