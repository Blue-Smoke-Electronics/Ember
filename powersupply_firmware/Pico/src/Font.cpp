#include "Font.h"

Font::Font(){
    
}
Font::Font(uint8_t * flash_address ){
    this->flash_address = flash_address; 
}


uint8_t * Font::Get_char(char c){
    
}