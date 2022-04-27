/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

/* [] END OF FILE */
#ifndef DISPLAY_H
#define DISPLAY_H

#include "stdio.h"
#include "pico/stdlib.h"
#include "color.h"
#include "Sprite.h"
#include "hardware/dma.h"
#include "Font.h"

#include <string.h>

#define DISPLAY_COMAND 0
#define DISPLAY_DATA 1


class Display{
  private: 
    void Send_packet(uint8_t command, uint8_t * data);
    void WriteComm(uint8_t data);
    void WriteData(uint8_t data);
    void draw_image(uint16_t x,uint16_t y,uint16_t width, uint16_t heigth, const uint8_t * img); 
    int dma_channal; 
    dma_channel_config dma_channal_config; 

  public:     
  
    int width; 
    int height;
    
    Display();
    Display(int width, int height);
    void Draw_pixel(int xpos,int ypos,uint8_t red, uint8_t green, uint8_t blue);
    void Draw_sprite(int xpos, int ypos, Sprite sprite);
    void Draw_char(int xpos, int ypos, Font font, char c); 
    void Draw_string(int xpos, int ypos, Font font, char * s);
    void Clear_square(int x,int y,int width, int heigth);
    void Clear_all();
    
  
    

};




#endif