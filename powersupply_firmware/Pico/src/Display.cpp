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
#include "hardware/pwm.h"

int Display::dma_channal; 
dma_channel_config Display::dma_channal_config; 
const int Display::height = 128; 
const int Display::width = 160; 
const uint16_t Display::spiQuie_size = 600;
SpiData Display::spiQuie[Display::spiQuie_size];
uint16_t Display::spiQuie_cnt =0; 
uint16_t Display::spiQuie_read_pos =0;
uint8_t Display::static_byte; 
bool Display::display_queue_overflow = false; 
uint Display::backlight_pwm_slice_num;

void Display::WriteComm (uint8_t data) {
    Push_to_spiQueue(SpiData(true,data));
}

void Display::WriteData (uint8_t data) {
    Push_to_spiQueue(SpiData(false,data));
}

void Display::Init () {
    spi_init(spi0,40000000); //    65000000); // 62.5Mhz
    gpio_set_function(Pcb::display_MISO_pin,GPIO_FUNC_SPI);
    gpio_set_function(Pcb::display_SCL_pin,GPIO_FUNC_SPI);
    //gpio_set_function(Pcb::display_MOSI_pin,GPIO_FUNC_SPI);

    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(Pcb::display_NCS_pin);
    gpio_init(Pcb::display_DC_pin);
    gpio_init(Pcb::dispaly_RST_pin);
    gpio_set_dir(Pcb::display_NCS_pin, GPIO_OUT);
    gpio_set_dir(Pcb::display_DC_pin, GPIO_OUT);
    gpio_set_dir(Pcb::dispaly_RST_pin, GPIO_OUT);
    gpio_put(Pcb::display_NCS_pin, 1);

    // setup backlint pwm
    gpio_init(Pcb::display_BACKLIGHT_pin);
    gpio_set_dir(Pcb::display_BACKLIGHT_pin, GPIO_OUT);
    gpio_put(Pcb::display_BACKLIGHT_pin, 1);
    // do not actiwait this before backlight pinn is reconnected to somthing other than gipo4
    // gpio_set_function(Pcb::display_BACKLIGHT_pin,GPIO_FUNC_PWM);
    // backlight_pwm_slice_num = pwm_gpio_to_slice_num(Pcb::display_BACKLIGHT_pin);
    // pwm_set_clkdiv(backlight_pwm_slice_num,200.0f);
    // pwm_set_wrap(backlight_pwm_slice_num,100); // 133kHz
    // pwm_set_enabled(backlight_pwm_slice_num,true);
    // Set_backlight(100);


    // setup dma 
    dma_channal = dma_claim_unused_channel(true);
    dma_channal_config = dma_channel_get_default_config(dma_channal);
    channel_config_set_transfer_data_size(&dma_channal_config,DMA_SIZE_8); // spi uses 8 bit for each transferr 
    channel_config_set_dreq(&dma_channal_config,DREQ_SPI0_TX); // wait on spi to compleat send befor moving data

    //Hardware reset
    gpio_put(Pcb::dispaly_RST_pin, 1);
    sleep_ms(15);
    gpio_put(Pcb::dispaly_RST_pin, 0);
    sleep_ms(120);
    gpio_put(Pcb::dispaly_RST_pin, 1);
    sleep_ms(120);

    // Display init commands
    WriteComm(0x01); //  1: Software reset,
    WriteComm(0x11); //  2: Out of sleep mode,
    
    WriteComm(0xB1); //  4: Frame rate control,  Rate = fosc/(1x2+40) * (LINE+2C+2D)
    WriteData(0x01); //    
    WriteData(0x2C); //     
    WriteData(0x2D); //    

    WriteComm(0xB2); // Framerate ctrl - idle mode,
    WriteData(0x01);
    WriteData(0x2C);
    WriteData(0x2D);

    WriteComm(0xB3); // Frame Rate Control (In Partial mode/ full colors)
    WriteData(0x01);
    WriteData(0x2C);
    WriteData(0x2D);
    WriteData(0x01);
    WriteData(0x2C);
    WriteData(0x2D);

    WriteComm(0xB4);
    WriteData(0x07);//     No inversion

    WriteComm(0xC0);
    WriteData(0xA2);
    WriteData(0x02);
    WriteData(0x84);

    WriteComm(0xC1);
    WriteData(0xC5);

    WriteComm(0xC2);
    WriteData(0x0A);
    WriteData(0x00);

    WriteComm(0xC3);
    WriteData(0x8A);
    WriteData(0x2A);

    WriteComm(0xC4);
    WriteData(0x8A);
    WriteData(0xEE);

    WriteComm(0xC5);
    WriteData(0x0E);
    
    WriteComm(0x20);
    
    WriteComm(0x36);  // madctl :: scanning direction of frame memory 
    WriteData(0b10101000);  // setting display orientation and BGR color mode 


    WriteComm(0x3A);  //  3: Set color mode,
    WriteData(0x06);  // 6 =  18 bit mode - 18 bit is max with spi  | 5 = 16bit (5r 6b 5g) do not work :( 

    WriteComm(0xE0);  // setting gamma 
    WriteData(0x02);
    WriteData(0x1c);
    WriteData(0x07);
    WriteData(0x12);
    WriteData(0x37);
    WriteData(0x32);
    WriteData(0x29);
    WriteData(0x2d);
    WriteData(0x29);
    WriteData(0x25);
    WriteData(0x2B);
    WriteData(0x39);
    WriteData(0x00);
    WriteData(0x01);
    WriteData(0x03);
    WriteData(0x10);

    WriteComm(0xE1);  // setting gamma 
    WriteData(0x03);
    WriteData(0x1d);
    WriteData(0x07);
    WriteData(0x06);
    WriteData(0x2E);
    WriteData(0x2C);
    WriteData(0x29);
    WriteData(0x2D);
    WriteData(0x2E);
    WriteData(0x2E);
    WriteData(0x37);
    WriteData(0x3F);
    WriteData(0x00);
    WriteData(0x00);
    WriteData(0x02);
    WriteData(0x10);

    WriteComm(0x3A);  //  3: Set color mode,
    WriteData(0x06);  // 6 =  18 bit mode - 18 bit is max with spi  | 5 = 16bit (5r 6b 5g) do not work :( 
    WriteComm(0x13);  // Normal display on
    WriteComm(0x29);  // Main screen turn on   
}

void Display::Draw_pixel (int x,int y,uint8_t red, uint8_t green, uint8_t blue) {
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

    //nop
    WriteComm(0x0);
}

void Display::Clear_square (int x,int y,int width, int heigth) {
    if (x+width >= width && y+heigth >= height && x < 0 && y <0){
        printf("Display, draw_square: invalid x or y, minx= %d miny= %d maxx= %d maxy= %d\r\n",x,y,x+width,y+heigth);
        return;
    }
    
    WriteComm(0x2a); // set colomn 
    WriteData(x>>8);
    WriteData(x);
    WriteData((x+width-1)>>8);
    WriteData(x+width-1);
    
    WriteComm(0x2b);  //set row
    WriteData(y>>8);
    WriteData(y);
    WriteData((y+heigth-1)>>8);
    WriteData( y+heigth-1);
     
    WriteComm(0x2c);  // draw pixels
    Push_to_spiQueue(SpiData(0xFF,width*heigth*3));

    WriteComm(0x0); // nop, signals that transmition is done. 
}
void Display::Draw_sprite (int x, int y, Sprite sprite) {
    if (x+sprite.width >= width && y+sprite.height >= height && x < 0 && y <0){
        printf("Display, draw_square: invalid x or y, minx= %d miny= %d maxx= %d maxy= %d\r\n",x,y,x+sprite.width,y+sprite.height);
        return;
    }
    
    WriteComm(0x2a);  // set colomn 
    WriteData(x>>8);
    WriteData(x);
    WriteData((x+sprite.width-1)>>8);
    WriteData(x+sprite.width-1);
    
    WriteComm(0x2b);  //set row
    WriteData(y>>8);
    WriteData(y);
    WriteData((y+sprite.height-1)>>8);
    WriteData( y+sprite.height-1);
    
    WriteComm(0x2c);  // draw pixels
    Push_to_spiQueue(SpiData(sprite.flash_address,sprite.size)); // send data from flash to display 
    WriteComm(0x0);  // NOP
}

void Display::Draw_char (int xpos, int ypos, Font font, char c) {
    Sprite char_img = Sprite(font.char_widht,font.char_height,font.Get_char_address(c));
    Draw_sprite(xpos,ypos,char_img);
}

void Display::Draw_string (int xpos, int ypos, Font font, std::string s) {
    int pos = xpos; 
    for (int i = 0; i < s.length() ; i++){
        Draw_char(pos,ypos,font,s[i]);
        pos += font.char_widht; 
    }
}

void Display::Clear_all () {
    Clear_square(0,0,width,height);
}

void Display::Update () {
    if(dma_channel_is_busy(dma_channal)  ){
        return; 
    }

    if (spiQuie_cnt ==0 ) { // nothing to draw
        return; 
    }
    
    SpiData data = spiQuie[spiQuie_read_pos];
    spiQuie_read_pos++;
    spiQuie_cnt--;
    if (spiQuie_read_pos >= spiQuie_size){
        spiQuie_read_pos=0;
    }
    
    if (data.isCommand) {
        sleep_us(10); 
    }
    
    gpio_put(Pcb::display_NCS_pin,0);

    if (data.isCommand) {
        gpio_put(Pcb::display_DC_pin,0);
    }
    else {
        gpio_put(Pcb::display_DC_pin,1);
    }

    if (data.staticInput) {
        channel_config_set_read_increment(&dma_channal_config,false); // do copy data from the same address evrey time
    }
    else {
        channel_config_set_read_increment(&dma_channal_config,true);
    }
    
    if (data.useAddress) {
        dma_channel_configure(dma_channal,&dma_channal_config,&spi_get_hw(spi0)->dr,data.data_ptr,data.size,true);
    }
    else {
        Display::static_byte = data.data;  
        dma_channel_configure(dma_channal,&dma_channal_config,&spi_get_hw(spi0)->dr,&Display::static_byte,data.size,true);
    }

    if (data.isCommand && data.data == 0x01) { // this is a software Reset command, and it requres 120 ms dalay for the display to start up again 
        sleep_ms(120); 
    }
    if (data.isCommand && data.data == 0x11) { // this is a sleep out command , and it requres 120 ms dalay for the display to start up again 
        sleep_ms(120); 
    }
} 

bool Display::Isready(){
    if (spiQuie_cnt ==0 ){
        return true; 
    }
    else {
        return false; 
    }
}

void Display::Push_to_spiQueue (SpiData spiData) {
    int writepos = spiQuie_read_pos+spiQuie_cnt;
    if(writepos >=spiQuie_size){
        writepos -= spiQuie_size; 
    }
    spiQuie[writepos] = spiData;
    spiQuie_cnt++; 
    
    if (spiQuie_cnt > spiQuie_size){
        printf("display quieu overflow\r\n");
    }
    Display::Update();
}

void Display::Set_backlight(int percent){
    if (percent > 99){
        percent =99;
    }
    if (percent<0){
        percent =0; 
    }

    //pwm_set_chan_level(backlight_pwm_slice_num,pwm_gpio_to_channel(Pcb::display_BACKLIGHT_pin),percent);
    if (percent == 0 ){
        gpio_put(Pcb::display_BACKLIGHT_pin, 0);
    }
    else{
        gpio_put(Pcb::display_BACKLIGHT_pin, 1);
    }

}

SpiData::SpiData(){
}

SpiData::SpiData(bool isCommand, uint8_t data){
    this->isCommand = isCommand;
    this->data = data;  
    this->data_ptr = &this->data;
    this->useAddress = false; 
    this->size = 1; 
    this->staticInput = false;  
}

SpiData::SpiData(const uint8_t * data,int size){
    this->isCommand = false;
    this->data = 0xFF; 
    this->data_ptr = data; 
    this->useAddress = true; 
    this->size = size; 
    this->staticInput = false; 
}

SpiData::SpiData(uint8_t  data,int size){
    this->isCommand = false;
    this->data = data;  
    this->data_ptr = &this->data;
    this->useAddress = false; 
    this->size = size; 
    this->staticInput = true;
}
