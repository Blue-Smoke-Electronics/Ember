#ifndef FONT_H
#define FONT_H

#include "pico/stdlib.h"


class Font{
    private:
        const int char_cnt = 126-32; 
        const char start_char  =32; 
        
         
    public:
        
        int char_widht; 
        int char_height; 
        int char_size; 
        int size;
        uint8_t * flash_address; 
        
        Font();
        Font(int width, int height, uint8_t * flash_address); 
        uint8_t * Get_char_address(char c); 


};


#endif