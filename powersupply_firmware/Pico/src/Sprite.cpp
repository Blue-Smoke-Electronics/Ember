#include "Sprite.h"
#include "Font.h"

Sprite::Sprite(){
    
}
Sprite::Sprite(const uint8_t* setup, const uint8_t * data){
    this->width = setup[0]; 
    this -> height = setup[1]; 
    this->size = width*height*3;
    this->data = data; 
}

Sprite::Sprite(Font font, char c){
    this->width = font.char_widht; 
    this -> height = font.char_height; 
    this->size = font.char_size;
    this->data = font.Get_char_address(c); 
}

const uint8_t * Sprite::Get_address(){
    return this->data;
}