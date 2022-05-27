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
#include "Flash.h"
#include "hardware/dma.h"
#include "Pcb.h"




int Display::dma_channal; 
dma_channel_config Display::dma_channal_config; 

const int Display::height = 320; 
const int Display::width = 480; 

std::queue<SpiData> Display::spiQuie = std::queue<SpiData>();
uint8_t Display::static_byte; 

void Display::WriteComm(uint8_t data){
    Push_to_spiQueue(SpiData(true,data));
}
void Display::WriteData(uint8_t data){
    Push_to_spiQueue(SpiData(false,data));

}

void Display::Init(){
 // #define READ_BIT 0x80


  spi_init(spi0,65000000); // 62.5Mhz
  gpio_set_function(Pcb::display_MISO_pin,GPIO_FUNC_SPI);
  gpio_set_function(Pcb::display_SCL_pin,GPIO_FUNC_SPI);
  gpio_set_function(Pcb::display_MOSI_pin,GPIO_FUNC_SPI);

   // Chip select is active-low, so we'll initialise it to a driven-high state
  gpio_init(Pcb::display_NCS_pin);
  gpio_init(Pcb::display_DC_pin);
  gpio_init(Pcb::dispaly_RST_pin);
  gpio_set_dir(Pcb::display_NCS_pin, GPIO_OUT);
  gpio_set_dir(Pcb::display_DC_pin, GPIO_OUT);
  gpio_set_dir(Pcb::dispaly_RST_pin, GPIO_OUT);
  gpio_put(Pcb::display_NCS_pin, 1);


//*********Hardware reset*********//
    gpio_put(Pcb::dispaly_RST_pin, 1);
sleep_ms(15);
    gpio_put(Pcb::dispaly_RST_pin, 0);
sleep_ms(120);
    gpio_put(Pcb::dispaly_RST_pin, 1);
sleep_ms(120);



WriteComm(0x01);
sleep_ms(5);

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
WriteComm(0x21); // invert display, use with isp display 
sleep_ms(120);

WriteComm(0x29);



// setup dma 
dma_channal = dma_claim_unused_channel(true);
dma_channal_config = dma_channel_get_default_config(dma_channal);
channel_config_set_transfer_data_size(&dma_channal_config,DMA_SIZE_8); // spi uses 8 bit for each transferr 
channel_config_set_dreq(&dma_channal_config,DREQ_SPI0_TX); // wait on spi to compleat send befor moving data



}



void Display::Draw_pixel(int x,int y,uint8_t red, uint8_t green, uint8_t blue){
    if (x >= 480 && y >= 320 && x < 0 && y < 0){
        printf("Display, draw_pixel: invalid x or y, x= %d y= %d\r\n",x,y);
        return;
    }
    // set colomn 
    WriteComm(0x2b);
    WriteData(y>>8);
    WriteData(y);
    WriteData((y+1)>>8);
    WriteData(y+1);
    
    //set row
    WriteComm(0x2a);
    WriteData(x>>8);
    WriteData(x);
    WriteData((x+1)>>8);
    WriteData(x+1);
    
    // draw pixel
    WriteComm(0x2c);
    WriteData(blue);
    WriteData(green);
    WriteData(red );
    WriteData(0x0);
    

}

void Display::Clear_square(int x,int y,int width, int heigth){
    if (x+width >= width && y+heigth >= height && x < 0 && y <0){
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
    Push_to_spiQueue(SpiData(0xFF,width*heigth*3));

    WriteComm(0x0); // nop, signals that transmition is done. 
   
}
void Display::Draw_sprite(int x, int y, Sprite sprite){
    if (x+sprite.width >= width && y+sprite.height >= height && x < 0 && y <0){
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

    Push_to_spiQueue(SpiData(sprite.flash_address,sprite.size));

    WriteComm(0x0); // nop, signals that transmition is done. 
}

void Display::Draw_char(int xpos, int ypos, Font font, char c){
    
    Sprite char_img = Sprite(font.char_widht,font.char_height,font.Get_char_address(c));
    Draw_sprite(xpos,ypos,char_img);
}
void Display::Draw_string(int xpos, int ypos, Font font, std::string s){
    int pos = xpos; 
    for (int i = 0; i < s.length() ; i++){
        Draw_char(pos,ypos,font,s[i]);
        pos += font.char_widht; 
    }
}

void Display::Clear_all(){
    Clear_square(0,0,480,320);
}


void Display::Update(){
    if(dma_channel_is_busy(dma_channal)  ){
        //printf("dma is busy\r\n");
        return; 
    }
    
    if( spiQuie.empty() ){
        return; 
    }
    
    
    SpiData data = spiQuie.front();
    spiQuie.pop();
    
    
  
    gpio_put(Pcb::display_NCS_pin,0);

    if(data.isComand){
        gpio_put(Pcb::display_DC_pin,0);
    }
    else{
        gpio_put(Pcb::display_DC_pin,1);
    }

    if(data.staticInput){
        channel_config_set_read_increment(&dma_channal_config,false); // do copy data from the same address evrey time
    }
    else{
        channel_config_set_read_increment(&dma_channal_config,true);
    }

    
    if(data.useAddress){
        dma_channel_configure(dma_channal,&dma_channal_config,&spi_get_hw(spi0)->dr,data.data_ptr,data.size,true);
        //spi_write_blocking(spi0,data.data_ptr,data.size); 
 
    }
    else{
        Display::static_byte = data.data;  
        dma_channel_configure(dma_channal,&dma_channal_config,&spi_get_hw(spi0)->dr,&Display::static_byte,data.size,true);
        //spi_write_blocking(spi0,&data.data,data.size); 
    }
    

} 


void Display::Push_to_spiQueue(SpiData spiData){
    if(spiQuie.size() < 1000){
        spiQuie.push(spiData);
    }
    else{
        printf("display quieu overflow\r\n");
    }
    
}


SpiData::SpiData(bool isComand, uint8_t data){
    this->isComand = isComand; 
    this->data = data;  
    this->data_ptr = &this->data;
    this->useAddress = false; 
    this->size = 1; 
    this->staticInput = false;  
}
SpiData::SpiData(uint8_t * data,int size){
    this->isComand = false; 
    this->data = 0xFF; 
    this->data_ptr = data; 
    this->useAddress = true; 
    this->size = size; 
    this->staticInput = false; 
}
SpiData::SpiData(uint8_t  data,int size){
    this->isComand = false; 
    this->data = data;  
    this->data_ptr = &this->data;
    this->useAddress = false; 
    this->size = size; 
    this->staticInput = true;
}