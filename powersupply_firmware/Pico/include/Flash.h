#ifndef FLASH_H
#define FLASH_H

#include "pico/stdlib.h"
#include "hardware/flash.h"
#include "Sprite.h"
#include "Font.h"


class Flash{
    private:
        static uint8_t * end_of_program_memory; 
        
        const int  flash_size = PICO_FLASH_SIZE_BYTES;
        uint8_t * write_address; 
        int cnt_since_program;
        int cnt_since_erase;
        uint8_t write_buffer [FLASH_PAGE_SIZE] ; 
        
    public:
        void start_data_stream(const uint8_t * address);
        void stop_data_stream();
        void stream_byte(uint8_t data);

        static Sprite logo; 
        static Font smalFont; 
        static Font bigFont; 
        

    


};




#endif