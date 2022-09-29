#include "Font.h"
#include "pico/stdlib.h"

#include <map> 

Font::Font(){
    
}
/*Font::Font(int width, int height, const uint8_t * flash_address ){
    this->flash_address = flash_address; 
    this->char_widht = width; 
    this->char_height = height; 
    this->char_size = this->char_widht*this->char_height*3; 
    this->size = this->char_size*this->char_cnt; 

}*/

Font::Font(const char * includedchars, const uint8_t* setup, const uint8_t * data){
    //this->flash_address = flash_address; 
    this->char_widht = setup[0]; 
    this->char_height = setup[1]; 
    this->char_cnt = setup[2];
    this->char_size = this->char_widht*this->char_height*3; 
    this->data_size = this->char_size*char_cnt; 
    this->includedchars = includedchars;
    this->data = data; 
    
}


const uint8_t * Font::Get_char_address(char c){
    for(int i =0; i < this->char_size; i++){
        if (c == includedchars[i]){
            return &this->data[i*this->char_size];
        }
    }

    //printf("char %c is not included in font",c);

    return &this->data[0];
}