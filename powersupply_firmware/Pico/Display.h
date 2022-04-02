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
#include "color.h"

#define DISPLAY_COMAND 0
#define DISPLAY_DATA 1

/*class Color{
  private:
  
  public: 
    int red;
    int green; 
    int blue;
};*/

class Display{
  private: 
  void Send_packet(uint8_t command, uint8_t * data);
  void GraphicLCDIntf_Write16(uint8_t dc,uint8_t data);
  void WriteComm(uint8_t data);
  void WriteData(uint8_t data);
  void draw_image(uint16_t x,uint16_t y,uint16_t width, uint16_t heigth, const uint8_t * img); 
  public: 
    Display();

    void Display_draw_pixel(uint16_t x,uint16_t y,color_rgb color);
    void Display_fill_square(uint16_t x,uint16_t y,uint16_t width, uint16_t heigth, color_rgb color);
    void Display_fill_screen(color_rgb color);

    void Display_draw_smily(color_rgb color);
    
  
    

};




#endif