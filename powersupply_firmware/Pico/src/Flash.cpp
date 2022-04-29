#include "Flash.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "Pcb.h"


#include "hardware/sync.h"

uint8_t * Flash::end_of_program_memory = (uint8_t *)XIP_BASE+Pcb::flash_size_bytes/2 -Pcb::flash_size_bytes% FLASH_SECTOR_SIZE; 

Sprite Flash::bootscreen = Sprite(480,320,end_of_program_memory); 
Sprite Flash::logo = Sprite(30,24,Flash::bootscreen.flash_address+Flash::bootscreen.size+FLASH_SECTOR_SIZE-Flash::bootscreen.size%FLASH_SECTOR_SIZE); 

Font Flash::smalFont =  Font( Flash::logo.flash_address+Flash::logo.size+FLASH_SECTOR_SIZE-Flash::logo.size%FLASH_SECTOR_SIZE); 

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