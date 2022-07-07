#ifndef ONOFF_H
#define ONOFF_H

#include "pico/stdlib.h"

class Onoff{
    private: 
        static bool watchdogenabled; 

    public: 
        static void Init(); 
        static void Turn_off_device();
        //static void KeepAlive(); 

};


#endif