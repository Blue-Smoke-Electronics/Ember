#ifndef SPRITE_H
#define SPRITE_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "Font.h"

class Sprite{
    private: 
        const uint8_t *data; 


    public: 
        
        int width; 
        int height; 
        int size; 
        

        Sprite();
        Sprite(const uint8_t* setup, const uint8_t * data); 
        Sprite(Font font, char c);
        const uint8_t * Get_address(); 

};




#endif