#include "Flash.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "Pcb.h"



#include "hardware/sync.h"

uint8_t * Flash::end_of_program_memory = (uint8_t *)XIP_BASE+Pcb::flash_size_bytes/2 -Pcb::flash_size_bytes/2% FLASH_SECTOR_SIZE; 

Sprite Flash::logo = Sprite(320,480,end_of_program_memory); 
Font Flash::smalFont =  Font( Flash::logo.flash_address+Flash::logo.size+FLASH_SECTOR_SIZE-Flash::logo.size%FLASH_SECTOR_SIZE); 



void Flash::start_data_stream(const uint8_t * address){
    this->write_address = (uint8_t*) address; 
    this->cnt_since_program = 0; 
    this->cnt_since_erase = 0; 
}

void Flash::stop_data_stream(){
    uint32_t ints = save_and_disable_interrupts(); // todo disable dual core
    if(this->cnt_since_erase == 0){
        printf("erassing: %d \r\n",this->write_address-XIP_BASE );
        flash_range_erase((uint32_t)(this->write_address-XIP_BASE), FLASH_SECTOR_SIZE);
    }
    
    flash_range_program((uint32_t)(this->write_address-XIP_BASE), this->write_buffer, FLASH_PAGE_SIZE);
    printf("programming: %d \r\n",this->write_address-XIP_BASE );

    restore_interrupts (ints);
}


void Flash::stream_byte(uint8_t data){
    this->write_buffer[cnt_since_program] = data;    

    uint32_t ints = save_and_disable_interrupts();// todo disable dual core
    if(this->cnt_since_erase == 0){
        //printf("erassing: %d \r\n",this->write_address-XIP_BASE );
        flash_range_erase((uint32_t)(this->write_address-XIP_BASE), FLASH_SECTOR_SIZE);
    
    }
    if(this->cnt_since_program == FLASH_PAGE_SIZE-1){
        flash_range_program((uint32_t)(this->write_address-XIP_BASE), this->write_buffer, FLASH_PAGE_SIZE);
        //printf("programming: %d \r\n",this->write_address-XIP_BASE );
        this->write_address += FLASH_PAGE_SIZE;
    }

    restore_interrupts (ints);
    
    this->cnt_since_program++; 
    if (this->cnt_since_program == FLASH_PAGE_SIZE){
        this->cnt_since_program =0; 
    }
    this->cnt_since_erase++; 
    if (this->cnt_since_erase == FLASH_SECTOR_SIZE){
        this->cnt_since_erase =0; 
    }
}