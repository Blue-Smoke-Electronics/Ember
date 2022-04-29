#include "Font.h"
#include "pico/stdlib.h"

Font::Font(){
    
}
Font::Font(uint8_t * flash_address ){
    this->flash_address = flash_address; 
}


uint8_t * Font::Get_char(char c){
    
   
    return (uint8_t *)0;
}