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

        static uint8_t * get_next_avilable_address(Font font); 
        static uint8_t * get_next_avilable_address(Sprite sprite); 
        static uint8_t * get_next_avilable_address(uint8_t * address, int size);
        static uint8_t * get_next_avilable_address(float * address);
        
    public:
        

        static Sprite bootscreen; 
        static Sprite logo; 
        static Font smalFont; 
        static Font bigFont; 

        static Sprite outputOnSymbol; 
        static Sprite outputOffSymbol; 

        static float *  batteryCapacity; 
        static float * outputVoltage; 
        static float * outputCurrent; 

        static void Load(Sprite sprite);
        static void Load(Font font);
        static void Save(float * address, float data); 

    


};




#endif