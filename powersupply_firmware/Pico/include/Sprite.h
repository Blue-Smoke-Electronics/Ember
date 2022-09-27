#ifndef SPRITE_H
#define SPRITE_H

#include <stdio.h>
#include "pico/stdlib.h"

class Sprite{
    private: 



    public: 
        
        int width; 
        int height; 
        int size; 
        const uint8_t * flash_address;

        Sprite();
        Sprite(int width, int height,const uint8_t * flash_address); 
 

};




#endif