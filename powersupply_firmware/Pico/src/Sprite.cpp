#include "Sprite.h"

Sprite::Sprite(){
    
}
Sprite::Sprite(int width, int height,const uint8_t * flash_address){
    this->width = width; 
    this -> height = height; 
    this->size = width*height*3;
    this->flash_address = flash_address; 
}

