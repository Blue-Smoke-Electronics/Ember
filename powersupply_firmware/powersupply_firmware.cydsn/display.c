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

#include "display.h"
#include "uart.h"
#include <stdio.h>
#include <project.h>


// adafruint display https://www.adafruit.com/product/2050
    // driver hx8357D  https://cdn-shop.adafruit.com/datasheets/HX8357-D_DS_April2012.pdf


//4DLCD-35480320-[RTP/CTP]-[CLB] datasheet https://www.tme.eu/Document/10635b7c24528589cc9152e9768eee8f/4DLCD-35480320.pdf 
        // driver HX8357D https://cdn-shop.adafruit.com/datasheets/HX8357-D_DS_April2012.pdf

void write_reg(uint16_t data){
    GraphicLCDIntf_Write16(DISPLAY_COMAND,data);
}
void write_dat(uint16_t data){
    GraphicLCDIntf_Write16(DISPLAY_DATA,data);
}

void Display_init(){
    
    
    
    /*
   
    // hardware reset
   nrst_Write(1);
    CyDelay(15);
    nrst_Write(0);
    CyDelay(120);
    nrst_Write(1);
    CyDelay(120);
    
    // start up display module 
    GraphicLCDIntf_Start();
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0x29);
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0x11);
    CyDelay(5);
    
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0x13);
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0x39);
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0x29);
    //GraphicLCDIntf_Write16(DISPLAY_COMAND,0x28);
    
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0x0A);
    uint16_t d1 = GraphicLCDIntf_Read16(DISPLAY_DATA);
    uint16_t d2 = GraphicLCDIntf_Read16(DISPLAY_DATA);
    uint16_t d3 = GraphicLCDIntf_Read16(DISPLAY_DATA);
    uint16_t d4 = GraphicLCDIntf_Read16(DISPLAY_DATA);
    printf("d1: %d , d2: %d , d3: %d , d4: %d\r\n",d1,d2,d3,d4);
    
    write_reg(0x1111);
    CyDelay(120);
    write_reg(0x2929);
    
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0x2c);
    while (1){
      
        GraphicLCDIntf_Write16(DISPLAY_DATA,0xFFFF);
        GraphicLCDIntf_Write16(DISPLAY_DATA,0x0000);
        //CyDelay(500);
        
        
    }
  */

    nrst_Write(0);
    CyDelay(5);
    nrst_Write(1);
    CyDelay(120);
    
    // start paralell comunication module
    GraphicLCDIntf_Start();
    

    //Software reset (01h) 
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0x01);
    
    // wait for reset to finish
    CyDelay(10);
    

    
    //SETEXTC: enable extension command (B9h) 
    // send (FFh,83h,57h) to enable 
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0xb9);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0xFF);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x83);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x57);
    
    // wait for 300 ms, do not know why
    ///CyDelay(300);
    
    //SETRGB: set RGB interface (B3h) 
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0xb3);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x80); //0x80 enables SDO Pin
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x00);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x06);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x06);
    

    
    //SETCOM: set VCOM voltage related register (B6h) 
    //GraphicLCDIntf_Write16(DISPLAY_COMAND,0xb6);
    //GraphicLCDIntf_Write16(DISPLAY_DATA,0x0A);// -1.52V
    //GraphicLCDIntf_Write16(DISPLAY_DATA,0xA2);
    

    
    //SETOSC: set internal oscillator (B0h) 
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0xB0);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x68);// Normal mode 70Hz, Idle mode 55 Hz
    

    
    //SETPanel: set panel characteristic (CCh) 
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0xcc);
    GraphicLCDIntf_Write16(DISPLAY_DATA,(1<<3)|(1<<2)|(1<<0));// RGB -> BGR, Gate direction swapped G480 -> G1 
    
    

    
    //SETPOWER: set power control (B1h) 
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0xb1);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x00);// Not deep standby
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x15);// BT
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x1C);// VSPR
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x1C);// VSNR
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x83);// AP
    GraphicLCDIntf_Write16(DISPLAY_DATA,0xAA);// FS
    
    //SETSTBA (C0h) 
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0xC0);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x50);// OPON normal
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x50);// OPON idle
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x01);// STBA
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x3C);// STBA
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x1E);// STBA
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x08);// GEN
    
    //SETCYC: set display cycle register (B4h) 
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0xB4);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x02);// NW 0x02
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x40);// RTN
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x00);// DIV
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x2A);// DUM
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x2A);// DUM
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x0D);// GDON
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x78);// GDOFF
    
    //SETGamma: set gamma curve (E0h)
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0xe0);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x02);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x0A);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x11);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x1d);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x23);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x35);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x41);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x4b);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x4b);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x42);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x3A);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x27);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x1B);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x08);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x09);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x03);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x02);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x0A);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x11);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x1d);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x23);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x35);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x41);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x4b);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x4b);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x42);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x3A);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x27);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x1B);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x08);  
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x09);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x03);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x00);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x01);
    
    
    // Interface pixel format (3Ah)
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0x3A);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x55);// 0x77 = 24 bit 0x66 = 18 bit, 0x55 = 16 bit
    
    //Memory access control (36h) 
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0x36);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0b00000000);
  
    //Tearing effect line on (35h) 
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0x35);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x00);// TW off
    
    //Set tear scan line (44h) 
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0x44);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x00);
    GraphicLCDIntf_Write16(DISPLAY_DATA,0x02);
    
    //Sleep out (11h) 
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0x11);
    
    //wait for wakeup
    //CyDelay(150);
    
    //Display on (29h) 
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0x29);
    
}


void Display_draw_pixel(uint16 x,uint16 y,color_rgb color){
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
    
    uint16_t data = ((color.blue&0xF8)<<8) | ((color.green&0xFC) <<3)|(color.red >> 3);
    GraphicLCDIntf_Write16(DISPLAY_DATA,data);  
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0x0);
    

}

void Display_fill_square(uint16 x,uint16 y,uint16 width, uint16 heigth, color_rgb color){
    if (x+width >= 480 && y+heigth >= 320 && x < 0 && y <0){
        printf("Display, draw_square: invalid x or y, minx= %d miny= %d maxx= %d maxy= %d\r\n",x,y,x+width,y+heigth);
        return;
    }
    
    
    // set colomn 
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0x2b);
    GraphicLCDIntf_Write16(DISPLAY_DATA,x>>8);
    GraphicLCDIntf_Write16(DISPLAY_DATA,x);
    GraphicLCDIntf_Write16(DISPLAY_DATA,(x+width-1)>>8);
    GraphicLCDIntf_Write16(DISPLAY_DATA,x+width-1);
    
    //set row
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0x2a);
    GraphicLCDIntf_Write16(DISPLAY_DATA,y>>8);
    GraphicLCDIntf_Write16(DISPLAY_DATA,y);
    GraphicLCDIntf_Write16(DISPLAY_DATA,(y+heigth-1)>>8);
    GraphicLCDIntf_Write16(DISPLAY_DATA, y+heigth-1);
    
    
    // draw pixels
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0x2c);
    for (int x =0; x < width; x++){
        for(int y =0; y < heigth;y++){
            uint16_t data = ((color.blue&0xF8)<<8) | ((color.green&0xFC) <<3)|(color.red >> 3);
            GraphicLCDIntf_Write16(DISPLAY_DATA,data);   
        }
    }

    
    GraphicLCDIntf_Write16(DISPLAY_COMAND,0x0);
    
}


void Display_fill_screen(color_rgb color){
    Display_fill_square(0,0,479,319,color);
}

void Display_draw_smily(color_rgb color){
    Display_fill_square(160,90,30,30,color);
    Display_fill_square(480-160-30,90,30,30,color);
    
    Display_fill_square(150,200,30,30,color);
    Display_fill_square(180,210,30,30,color);
    Display_fill_square(210,220,30,30,color);
    
    Display_fill_square(480-30-150,200,30,30,color);
    Display_fill_square(480-30-180,210,30,30,color);
    Display_fill_square(480-30-210,220,30,30,color);
}
