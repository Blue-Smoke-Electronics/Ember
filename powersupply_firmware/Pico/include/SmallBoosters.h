#ifndef SMALLBOOSTERS_H
#define SMALLBOOSTERS_H

#include "pico/stdlib.h"

class SmallBoosters{

    public: 
        static void Init();
        static void Disable(); 
        static void Enable();
};

#endif