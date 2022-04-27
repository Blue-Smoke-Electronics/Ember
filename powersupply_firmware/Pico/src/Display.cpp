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
//#include "logo.h"
#include "Flash.h"
#include "hardware/dma.h"

#define PIN_MISO 4
#define PIN_CS 5  
#define PIN_SCK  2
#define PIN_MOSI 3
#define PIN_DC   6// todo: make pio module that suport 9bit spi
#define PIN_RESET 7
#define SPI_PORT spi0
#define DISPLAY_COMAND  0
#define DISPLAY_DATA  1

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
    
}
Display::Display(int width, int height){

    this->width = width;
    this->height = height;
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
//WriteComm(0x22);


// setup dma 
this->dma_channal = dma_claim_unused_channel(true);
this->dma_channal_config = dma_channel_get_default_config(this->dma_channal);
channel_config_set_transfer_data_size(&this->dma_channal_config,DMA_SIZE_8); // spi uses 8 bit for each transferr 
channel_config_set_dreq(&this->dma_channal_config,DREQ_SPI0_TX); // wait on spi to compleat send befor moving data



}





void Display::Draw_pixel(int x,int y,uint8_t red, uint8_t green, uint8_t blue){
    if (x >= 480 && y >= 320 && x < 0 && y < 0){
        printf("Display, draw_pixel: invalid x or y, x= %d y= %d\r\n",x,y);
        return;
    }
    // set colomn 
    WriteComm(0x2b);
    WriteData(x>>8);
    WriteData(x);
    WriteData((x+1)>>8);
    WriteData(x+1);
    
    //set row
    WriteComm(0x2a);
    WriteData(y>>8);
    WriteData(y);
    WriteData((y+1)>>8);
    WriteData(y+1);
    
    // draw pixel
    WriteComm(0x2c);
    WriteData(blue);
    WriteData(green);
    WriteData(red );
    WriteData(0x0);
    

}
//const uint8_t data[320*480*3] = {}; 
void Display::Clear_square(int x,int y,int width, int heigth){
    if (x+width >= this->width && y+heigth >= this->height && x < 0 && y <0){
        printf("Display, draw_square: invalid x or y, minx= %d miny= %d maxx= %d maxy= %d\r\n",x,y,x+width,y+heigth);
        return;
    }
    
    
    // set colomn 
    WriteComm(0x2a);
    WriteData(x>>8);
    WriteData(x);
    WriteData((x+width-1)>>8);
    WriteData(x+width-1);
    
    //set row
    WriteComm(0x2b);
    WriteData(y>>8);
    WriteData(y);
    WriteData((y+heigth-1)>>8);
    WriteData( y+heigth-1);
    
    
    // draw pixels
    WriteComm(0x2c);

    

    // wait on dma to be ready
    while(dma_channel_is_busy(this->dma_channal)){
        
    }

    // use dma to send data to spi 
    gpio_put(PIN_CS, 0);
    gpio_put(PIN_DC,1);

    uint8_t data = 0xFF; 
    channel_config_set_read_increment(&this->dma_channal_config,false); // do copy data from the same address evrey time
    dma_channel_configure(this->dma_channal,&this->dma_channal_config,&spi_get_hw(SPI_PORT)->dr,&data,width*heigth*3,true); // sent 0xff to all pixels in square 
    channel_config_set_read_increment(&this->dma_channal_config,true); // back to normal operation

    gpio_put(PIN_CS, 1);

    WriteComm(0x0); // nop, signals that transmition is done. 
    
}
void Display::Draw_sprite(int x, int y, Sprite sprite){
    if (x+sprite.width >= this->width && y+sprite.height >= this->height && x < 0 && y <0){
        printf("Display, draw_square: invalid x or y, minx= %d miny= %d maxx= %d maxy= %d\r\n",x,y,x+sprite.width,y+sprite.height);
        return;
    }
    
    
    // set colomn 
    WriteComm(0x2a);
    WriteData(x>>8);
    WriteData(x);
    WriteData((x+sprite.width-1)>>8);
    WriteData(x+sprite.width-1);
    
    //set row
    WriteComm(0x2b);
    WriteData(y>>8);
    WriteData(y);
    WriteData((y+sprite.height-1)>>8);
    WriteData( y+sprite.height-1);
    
    
    // draw pixels
    WriteComm(0x2c);

    // wait on dma to be ready
    while(dma_channel_is_busy(this->dma_channal)){

    }

    // use dma to send data to spi 
    gpio_put(PIN_CS, 0);
    gpio_put(PIN_DC,1);

    dma_channel_configure(this->dma_channal,&this->dma_channal_config,&spi_get_hw(SPI_PORT)->dr,sprite.flash_address,sprite.size,true); // sent 0xff to all pixels in square 
    // todo find a way to send nop with dma 
    while(dma_channel_is_busy(this->dma_channal)){

    }

    WriteComm(0x0); // nop, signals that transmition is done. 
    gpio_put(PIN_CS, 1);
}

void Display::Clear_all(){
    this->Clear_square(0,0,480,320);
}
