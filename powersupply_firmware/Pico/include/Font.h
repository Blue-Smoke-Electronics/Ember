#ifndef FONT_H
#define FONT_H

#include "pico/stdlib.h"


class Font{
    private:
        int char_cnt; 
        char start_char; 
        uint8_t * flash_address; 
    public:
        
        int char_widht; 
        int char_height; 
        int char_size; 
        
        Font();
        Font(uint8_t * flash_address); 
        uint8_t * Get_char(char c); 


};


#endif