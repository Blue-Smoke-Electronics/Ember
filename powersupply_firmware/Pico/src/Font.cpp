#include "Font.h"
#include "pico/stdlib.h"

Font::Font(){
    
}
Font::Font(int width, int height, uint8_t * flash_address ){
    this->flash_address = flash_address; 
    this->char_widht = width; 
    this->char_height = height; 
    this->char_size = this->char_widht*this->char_height*3; 
    this->size = this->char_size*this->char_cnt; 

}


uint8_t * Font::Get_char_address(char c){

    return (uint8_t *)(flash_address + (c -start_char )*char_size);
}