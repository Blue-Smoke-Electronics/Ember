#ifndef FLASH_H
#define FLASH_H

#include "pico/stdlib.h"
#include "hardware/flash.h"



class Flash{
    private:
        const int  flash_size = PICO_FLASH_SIZE_BYTES;
        uint8_t * write_address; 
        int cnt_since_program;
        int cnt_since_erase;
        uint8_t write_buffer [FLASH_PAGE_SIZE] ; 
    public:
        void start_data_stream(const uint8_t * address);
        void stop_data_stream();
        void stream_byte(uint8_t data);
        
        const int logo_width = 480;
        const int logo_height = 320;
        const int logo_size = logo_height*logo_width*3; 
 
        const uint8_t * logo_address = (uint8_t *)XIP_BASE+flash_size -logo_size-logo_size%FLASH_SECTOR_SIZE; 
    


};




#endif