#ifndef FLASH_H
#define FLASH_H

#include "pico/stdlib.h"
#include "hardware/flash.h"
#include "Sprite.h"
#include "Font.h"

class Flash{
    private:
        static uint8_t * end_of_program_memory; 
        
        static const int  flash_size = PICO_FLASH_SIZE_BYTES;
        static uint8_t * write_address; 
        static int cnt_since_program;
        static int cnt_since_erase;
        static uint8_t write_buffer [FLASH_PAGE_SIZE] ;         
        
        static void start_data_stream(const uint8_t * address);
        static void stop_data_stream();
        static void stream_byte(uint8_t data);
        
    public:
        

        static Sprite bootscreen; 
        static Sprite logo; 
        static Font smalFont; 
        static Font bigFont; 
        
        static void Load(Sprite sprite);
        static void Load(Font font);

    


};




#endif