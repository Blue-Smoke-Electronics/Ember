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

#include "Display.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "color.h"
#include "logo.h"

#define PIN_MISO 16
#define PIN_CS 17  
#define PIN_SCK  18
#define PIN_MOSI 19
#define PIN_DC   21 // todo: make pio module that suport 9bit spi
#define PIN_RESET 20
#define SPI_PORT spi0
#define DISPLAY_COMAND  0
#define DISPLAY_DATA  1

void Display::GraphicLCDIntf_Write16(uint8_t dc,uint8_t data){
  gpio_put(PIN_CS, 0);
  gpio_put(PIN_DC,dc);
  spi_write_blocking(SPI_PORT,&data,1); 

  gpio_put(PIN_CS, 1);
}
void Display::WriteComm(uint8_t data){
    gpio_put(PIN_CS, 0);
    gpio_put(PIN_DC,0);
    spi_write_blocking(SPI_PORT,&data,1); 

    gpio_put(PIN_CS, 1);
}
void Display::WriteData(uint8_t data){
    gpio_put(PIN_CS, 0);
    gpio_put(PIN_DC,1);
    spi_write_blocking(SPI_PORT,&data,1); 

    gpio_put(PIN_CS, 1);
}

Display::Display(){

 // #define READ_BIT 0x80

  spi_init(SPI_PORT,65000000); // 62.5Mhz
  gpio_set_function(PIN_MISO,GPIO_FUNC_SPI);
  //gpio_set_function(PIN_CS,GPIO_FUNC_SPI);
  gpio_set_function(PIN_SCK,GPIO_FUNC_SPI);
  gpio_set_function(PIN_MOSI,GPIO_FUNC_SPI);
  //gpio_set_function(PIN_CS,GPIO_FUNC_SPI);

   // Chip select is active-low, so we'll initialise it to a driven-high state
  gpio_init(PIN_CS);
  gpio_init(PIN_DC);
  gpio_init(PIN_RESET);
  gpio_set_dir(PIN_CS, GPIO_OUT);
  gpio_set_dir(PIN_DC, GPIO_OUT);
  gpio_set_dir(PIN_RESET, GPIO_OUT);
  gpio_put(PIN_CS, 1);


//*********Hardware reset*********//
    gpio_put(PIN_RESET, 1);
sleep_ms(15);
    gpio_put(PIN_RESET, 0);
sleep_ms(120);
    gpio_put(PIN_RESET, 1);
sleep_ms(120);
WriteComm(0x01);
sleep_ms(5);
//********Start Initial Sequence*******//
WriteComm(0xE0); //P-Gamma
WriteData(0x00);
WriteData(0x13);
WriteData(0x18);
WriteData(0x04);
WriteData(0x0F);
WriteData(0x06);
WriteData(0x3A);
WriteData(0x56);
WriteData(0x4D);
WriteData(0x03);
WriteData(0x0A);
WriteData(0x06);
WriteData(0x30);
WriteData(0x3E);
WriteData(0x0F);
WriteComm(0XE1); //N-Gamma
WriteData(0x00);
WriteData(0x13);
WriteData(0x18);
WriteData(0x01);
WriteData(0x11);
WriteData(0x06);
WriteData(0x38);
WriteData(0x34);
WriteData(0x4D);
WriteData(0x06);
WriteData(0x0D);
WriteData(0x0B);
WriteData(0x31);
WriteData(0x37);
WriteData(0x0F);
WriteComm(0xC0);
WriteData(0x18);
WriteData(0x16);
WriteComm(0xC1);
WriteData(0x45);
WriteComm(0xC5); //VCOM
WriteData(0x00);
WriteData(0x63);
WriteData(0x01);
WriteComm(0x36); // 
WriteData(0b00100000); // rgb, display read left to right, topp to bottom
WriteComm(0x3A); //Interface Mode Control
WriteData(0x06); // 6 =  18 bit mode - 18 bit is max with spi  | 5 = 16bit (5r 6b 5g) do not work :( 
WriteComm(0xB0); //Interface Mode Control
WriteData(0x00);
WriteComm(0xB1); //Frame rate 70HZ
WriteData(0xB0);
WriteComm(0xB4);
WriteData(0x02);
WriteComm(0xB6); //RGB/MCU Interface Control
WriteData(0x02);
//WriteData(0x22);
WriteComm(0xE9);
WriteData(0x00);
WriteComm(0xF7);
WriteData(0xA9);
WriteData(0x51);
WriteData(0x2C);
WriteData(0x82);
WriteComm(0x11);
//WriteComm(0x21); // invert display, use with isp display 
sleep_ms(120);

WriteComm(0x29);

Display_fill_screen(COLORRGB_WHITE);
//Display_draw_smily(COLORRGB_RED);

draw_image(100,0,280,320,logo);

    
}





void Display::Display_draw_pixel(uint16_t x,uint16_t y,color_rgb color){
    if (x >= 480 && y >= 320 && x < 0 && y < 0){
        printf("Display, draw_pixel: invalid x or y, x= %d y= %d\r\n",x,y);
        return;
    }
    // set colomn 
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0x2b);
    GraphicLCDIntf_Write16(DISPLAY_DATA,x>>8);
    GraphicLCDIntf_Write16(DISPLAY_DATA,x);
    GraphicLCDIntf_Write16(DISPLAY_DATA,(x+1)>>8);
    GraphicLCDIntf_Write16(DISPLAY_DATA,x+1);
    
    //set row
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0x2a);
    GraphicLCDIntf_Write16(DISPLAY_DATA,y>>8);
    GraphicLCDIntf_Write16(DISPLAY_DATA,y);
    GraphicLCDIntf_Write16(DISPLAY_DATA,(y+1)>>8);
    GraphicLCDIntf_Write16(DISPLAY_DATA,y+1);
    
    // draw pixel
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0x2c);
    GraphicLCDIntf_Write16(DISPLAY_DATA,color.blue);
    GraphicLCDIntf_Write16(DISPLAY_DATA,color.green);
    GraphicLCDIntf_Write16(DISPLAY_DATA,color.red );
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0x0);
    

}
const uint8_t data[320*480*3] = {}; 
void Display::Display_fill_square(uint16_t x,uint16_t y,uint16_t width, uint16_t heigth, color_rgb color){
    if (x+width >= 480 && y+heigth >= 320 && x < 0 && y <0){
        printf("Display, draw_square: invalid x or y, minx= %d miny= %d maxx= %d maxy= %d\r\n",x,y,x+width,y+heigth);
        return;
    }
    
    
    // set colomn 
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0x2a);
    GraphicLCDIntf_Write16(DISPLAY_DATA,x>>8);
    GraphicLCDIntf_Write16(DISPLAY_DATA,x);
    GraphicLCDIntf_Write16(DISPLAY_DATA,(x+width-1)>>8);
    GraphicLCDIntf_Write16(DISPLAY_DATA,x+width-1);
    
    //set row
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0x2b);
    GraphicLCDIntf_Write16(DISPLAY_DATA,y>>8);
    GraphicLCDIntf_Write16(DISPLAY_DATA,y);
    GraphicLCDIntf_Write16(DISPLAY_DATA,(y+heigth-1)>>8);
    GraphicLCDIntf_Write16(DISPLAY_DATA, y+heigth-1);
    
    
    // draw pixels
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0x2c);

        gpio_put(PIN_CS, 0);
    gpio_put(PIN_DC,1);
    
    spi_write_blocking(SPI_PORT,data,width*heigth*3); 
    gpio_put(PIN_CS, 1);
    ///gpio_put(PIN_CS, 1);
    // for (int x =0; x < width; x++){
    //     for(int y =0; y < heigth;y++){
    //         uint8_t b1 = (color.red<<3) | (color.green >> 5);
    //         uint8_t b2 = (color.green<<3) | (color.blue >> 5);
    //         GraphicLCDIntf_Write16(DISPLAY_DATA,color.blue);
    //         GraphicLCDIntf_Write16(DISPLAY_DATA,color.green);
    //         GraphicLCDIntf_Write16(DISPLAY_DATA,color.red);
            
          
    //     }
    // }

    
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0x0);
    
}
void Display::draw_image(uint16_t x,uint16_t y,uint16_t width, uint16_t heigth, const uint8_t * img){
    if (x+width >= 480 && y+heigth >= 320 && x < 0 && y <0){
        printf("Display, draw_square: invalid x or y, minx= %d miny= %d maxx= %d maxy= %d\r\n",x,y,x+width,y+heigth);
        return;
    }
    
    
    // set colomn 
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0x2a);
    GraphicLCDIntf_Write16(DISPLAY_DATA,x>>8);
    GraphicLCDIntf_Write16(DISPLAY_DATA,x);
    GraphicLCDIntf_Write16(DISPLAY_DATA,(x+width-1)>>8);
    GraphicLCDIntf_Write16(DISPLAY_DATA,x+width-1);
    
    //set row
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0x2b);
    GraphicLCDIntf_Write16(DISPLAY_DATA,y>>8);
    GraphicLCDIntf_Write16(DISPLAY_DATA,y);
    GraphicLCDIntf_Write16(DISPLAY_DATA,(y+heigth-1)>>8);
    GraphicLCDIntf_Write16(DISPLAY_DATA, y+heigth-1);
    
    
    // draw pixels
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0x2c);
    

    int i =0; 
    for (int x =0; x < heigth; x++){
        i+=4;
        for(int y =0; y < width;y++){
            GraphicLCDIntf_Write16(DISPLAY_DATA,img[x*width*4+y*4]);
            GraphicLCDIntf_Write16(DISPLAY_DATA,img[x*width*4+y*4+1]);
            GraphicLCDIntf_Write16(DISPLAY_DATA,img[x*width*4+y*4+2]);
            
            i--; 
          
        }
    }
    printf("%d",i);
   
    
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0x0);
}

void Display::Display_fill_screen(color_rgb color){
    Display_fill_square(0,0,479,319,color);
}

void Display::Display_draw_smily(color_rgb color){
    Display_fill_square(160,90,30,30,color);
    Display_fill_square(480-160-30,90,30,30,color);
    
    Display_fill_square(150,200,30,30,color);
    Display_fill_square(180,210,30,30,color);
    Display_fill_square(210,220,30,30,color);
    
    Display_fill_square(480-30-150,200,30,30,color);
    Display_fill_square(480-30-180,210,30,30,color);
    Display_fill_square(480-30-210,220,30,30,color);
}