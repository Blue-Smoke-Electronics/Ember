#ifndef FONT_H
#define FONT_H

#include "pico/stdlib.h"

#include <map>


class Font{
    private:

        const char *includedchars;
        const uint8_t *data; 

    
    public:
        
        int char_widht; 
        int char_height; 
        int char_size;
        int char_cnt; 
        int data_size;
        //const uint8_t * flash_address; 
        
        Font();
        //Font(int width, int height, const uint8_t * flash_address); 
        Font(const char * includedchars, const uint8_t* setup, const uint8_t * data); 
        const uint8_t * Get_char_address(char c); 
};


#endif