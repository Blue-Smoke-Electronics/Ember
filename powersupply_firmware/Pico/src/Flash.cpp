#include "Flash.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "Pcb.h"
#include "hardware/sync.h"

uint8_t * Flash::end_of_program_memory = get_next_avilable_address((uint8_t *)XIP_BASE+Pcb::flash_size_bytes/2 ,0); 

Sprite Flash::bootscreen = Sprite(160,128,end_of_program_memory); 
Sprite Flash::logo = Sprite(30,24,get_next_avilable_address(bootscreen)); 

Font Flash::bigFont =  Font(24,38, get_next_avilable_address(logo));
Font Flash::smalFont =  Font(12,19, get_next_avilable_address(bigFont)); 

Sprite Flash::outputOnSymbol = Sprite(20,20, get_next_avilable_address(smalFont));
Sprite Flash::outputOffSymbol = Sprite(20,20, get_next_avilable_address(outputOnSymbol));

float * Flash::batteryCapacity = (float*)get_next_avilable_address(outputOffSymbol);
float * Flash::outputVoltage = (float*)get_next_avilable_address(batteryCapacity);
float * Flash::outputCurrent = (float*)get_next_avilable_address(outputVoltage);

Sprite Flash::batterySymbolLow = Sprite(26,50, get_next_avilable_address(outputCurrent));
Sprite Flash::batterySymbolMedLow = Sprite(26,50, get_next_avilable_address(batterySymbolLow)); 
Sprite Flash::batterySymbolMedHigh = Sprite(26,50, get_next_avilable_address(batterySymbolMedLow));
Sprite Flash::batterySymbolHigh = Sprite(26,50, get_next_avilable_address(batterySymbolMedHigh));
Sprite Flash::batterySymbolEmpty = Sprite(26,50, get_next_avilable_address(batterySymbolHigh));
Sprite Flash::batteryChargingSymbol = Sprite(19,29, get_next_avilable_address(batterySymbolEmpty));

Sprite Flash::selectedMarker = Sprite(12,2, get_next_avilable_address(batteryChargingSymbol));

uint8_t *Flash:: write_address; 
int Flash::cnt_since_program;
int Flash::cnt_since_erase;
uint8_t Flash::write_buffer [FLASH_PAGE_SIZE];

void Flash::start_data_stream(const uint8_t * address){
    write_address = (uint8_t*) address; 
    cnt_since_program = 0; 
    cnt_since_erase = 0; 
}

void Flash::stop_data_stream(){
    uint32_t ints = save_and_disable_interrupts(); // todo disable dual core
    if(cnt_since_erase == 0){
        flash_range_erase((uint32_t)(write_address-XIP_BASE), FLASH_SECTOR_SIZE);
    }
    flash_range_program((uint32_t)(write_address-XIP_BASE), write_buffer, FLASH_PAGE_SIZE);
    printf("Flash load compleated!\r\n" );
    restore_interrupts (ints);
}

void Flash::stream_byte(uint8_t data){
    write_buffer[cnt_since_program] = data;    

    uint32_t ints = save_and_disable_interrupts();// todo disable dual core
    if(cnt_since_erase == 0){
        //printf("erassing: %d \r\n",this->write_address-XIP_BASE );
        flash_range_erase((uint32_t)(write_address-XIP_BASE), FLASH_SECTOR_SIZE);
    }
    if(cnt_since_program == FLASH_PAGE_SIZE-1){
        flash_range_program((uint32_t)(write_address-XIP_BASE), write_buffer, FLASH_PAGE_SIZE);
        //printf("programming: %d \r\n",this->write_address-XIP_BASE );
        write_address += FLASH_PAGE_SIZE;
    }
    restore_interrupts (ints);
    
    cnt_since_program++; 
    if (cnt_since_program == FLASH_PAGE_SIZE){
        cnt_since_program =0; 
    }
    cnt_since_erase++; 
    if (cnt_since_erase == FLASH_SECTOR_SIZE){
        cnt_since_erase =0; 
    }
}

void Flash::Save(float * address, float data){   
    uint32_t  a = ((uint32_t)address-(uint32_t)XIP_BASE);

    write_buffer[0] = ((uint8_t*)(&data))[0]; 
    write_buffer[1] = ((uint8_t*)(&data))[1]; 
    write_buffer[2] = ((uint8_t*)(&data))[2]; 
    write_buffer[3] = ((uint8_t*)(&data))[3]; 
    
    sleep_ms(100); // magicaly removes freeses

    uint32_t ints = save_and_disable_interrupts();// todo disable dual core
    flash_range_erase(a, FLASH_SECTOR_SIZE);
    flash_range_program(a, write_buffer, FLASH_PAGE_SIZE);
    restore_interrupts (ints);
}

void Flash::Load(Sprite sprite){
    start_data_stream(sprite.flash_address);
    for (int i=0; i< sprite.size;i++){
        uint8_t r =getchar();
        printf("%d\n",i);
        stream_byte(r);
    }
    stop_data_stream();
}

void Flash::Load(Font font){
        start_data_stream(font.flash_address);
    for (int i=0; i< font.size;i++){
        uint8_t r =getchar();
        printf("%d\n",i);
        stream_byte(r);
    }
    stop_data_stream();
}

uint8_t * Flash::get_next_avilable_address(Font font){
    return get_next_avilable_address(font.flash_address,font.size);
}

uint8_t * Flash::get_next_avilable_address(Sprite sprite){
    return get_next_avilable_address(sprite.flash_address,sprite.size);
} 

uint8_t * Flash::get_next_avilable_address(uint8_t * address, int size){
    return address+size+FLASH_SECTOR_SIZE-size%FLASH_SECTOR_SIZE; 
}

uint8_t * Flash::get_next_avilable_address(float * address){
    return get_next_avilable_address((uint8_t * )address,1); 
}

