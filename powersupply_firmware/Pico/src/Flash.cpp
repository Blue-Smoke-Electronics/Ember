#include "Flash.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "Pcb.h"
#include "hardware/sync.h"

#include "Font.h"

#include "smallFont.h"
#include "smallFontRed.h"
#include "bigFont.h"

uint8_t Flash::write_buffer[FLASH_PAGE_SIZE/sizeof(float)];   

void Flash::Save(float batteryCapacity ,float outputVoltage, float outputCurrent ){   
    write_buffer[0] = ((uint8_t*)(&batteryCapacity))[0];
    write_buffer[1] = ((uint8_t*)(&batteryCapacity))[1];
    write_buffer[2] = ((uint8_t*)(&batteryCapacity))[2];
    write_buffer[3] = ((uint8_t*)(&batteryCapacity))[3];
    write_buffer[4] = ((uint8_t*)(&outputVoltage))[0];
    write_buffer[5] = ((uint8_t*)(&outputVoltage))[1];
    write_buffer[6] = ((uint8_t*)(&outputVoltage))[2];
    write_buffer[7] = ((uint8_t*)(&outputVoltage))[3];
    write_buffer[8] = ((uint8_t*)(&outputCurrent))[0];
    write_buffer[9] = ((uint8_t*)(&outputCurrent))[1];
    write_buffer[10] = ((uint8_t*)(&outputCurrent))[2];
    write_buffer[11] = ((uint8_t*)(&outputCurrent))[3];   
    sleep_ms(100); // magicaly removes freeses

    // writing to end of flash memmory
    uint32_t ints = save_and_disable_interrupts();// todo disable dual core
    flash_range_erase(Pcb::flash_size_bytes-FLASH_SECTOR_SIZE, FLASH_SECTOR_SIZE);
    flash_range_program(Pcb::flash_size_bytes-FLASH_SECTOR_SIZE, write_buffer, FLASH_PAGE_SIZE);
    restore_interrupts (ints);
}
float Flash::GetBatteryCapacity(){
    return ((float*)(XIP_BASE+Pcb::flash_size_bytes-FLASH_SECTOR_SIZE))[0];
}
float Flash::GetOutputVoltage(){
    return ((float*)(XIP_BASE+Pcb::flash_size_bytes-FLASH_SECTOR_SIZE))[1];
} 
float Flash::GetOutputCurrent(){
    return ((float*)(XIP_BASE+Pcb::flash_size_bytes-FLASH_SECTOR_SIZE))[2];
} 


Font Flash::smallFont =  Font(smallFont_CHARS,smallFont_SETUP,smallFont_DATA);
Font Flash::smallFontRed =  Font(smallFontRed_CHARS,smallFontRed_SETUP,smallFontRed_DATA);
Font Flash::bigFont =  Font(bigFont_CHARS,bigFont_SETUP,bigFont_DATA);
