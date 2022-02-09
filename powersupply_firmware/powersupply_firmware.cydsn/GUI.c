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

#include "GUI.h"
#include "display.h"
#include <stdio.h>
#include "ArialNumFontPlus.h"
#include "BigFont.h"
#include "Grotesk32x64.h"
#include "Grotesk16x32.h"
#include "project.h"
#include "color.h"


#include <stdio.h>


typedef struct {
    char c;
    int x;
    int y;
    struct color_rgb color;
} char_and_pos; 

#define CHAR_DRAWING_MEM_SIZE 200
char_and_pos char_drawing_memory [CHAR_DRAWING_MEM_SIZE];
int char_drawing_memory_end = 0; 

 void GUI_draw_char(char c,int xpos, int ypos,color_rgb color,const uint8 font[]){
    // http://www.rinkydinkelectronics.com/h_utft_fonts_101.php
   
    // load font info
    int font_width = font[0];
    int font_height = font[1];
    int font_start_char =font[2]; 
    int font_char_count = font[3]; 
       
    // check char for validity
    if (c < font_start_char || c > font_start_char+font_char_count) {
        printf ("GUI_draw_char, char not in font: %d\r\n", (int)c);
        return;
    }
    
    // Convert the character to an index
    c -= font_start_char;
        

    
    // check if char is in memory
    int mempos =-1; 
    for (int i = 0; i < char_drawing_memory_end; i++){
        if( char_drawing_memory[i].x == xpos && char_drawing_memory[i].y == ypos){
            mempos = i; 
            break; 
        }
    }
    
    // if char is in memory, check if it is updated
    if (mempos !=-1){
        if(char_drawing_memory[mempos].c == c && colors_is_equal(char_drawing_memory[mempos].color, color) ){
            return; 
        }
    }
    // othervise add to memory
    else {
        char_drawing_memory_end++; 
        char_drawing_memory[char_drawing_memory_end].c = c;
        char_drawing_memory[char_drawing_memory_end].x = xpos;
        char_drawing_memory[char_drawing_memory_end].y = ypos;
    }
    
    // Draw pixels
    for (uint8 j=0; j<font_height; j++) {
        for (uint8 i=0; i<font_width; i++) { 
            // do not update screen if not nessesary 
            bool new_pixel = font[c*font_height*font_width/8 + j*font_width/8+i/8+4] & (1<<(7-i%8));
            bool old_pixel = font[char_drawing_memory[mempos].c*font_height*font_width/8 + j*font_width/8+i/8+4] & (1<<(7-i%8));
            if ( new_pixel == old_pixel && colors_is_equal(char_drawing_memory[mempos].color,color) && mempos != -1){
                continue;   
            }
            
            
            if (new_pixel) {
                Display_draw_pixel(xpos+i, ypos+j, color);
            }

            else{
                Display_draw_pixel(xpos+i, ypos+j, COLORRGB_WHITE);
            }
        }
        //printf("\r\n");
    }
    
    // update memory
    char_drawing_memory[mempos].c = c; 
    char_drawing_memory[mempos].color = color;
}
void GUI_draw_string(char str [],int xpos, int ypos,color_rgb color,const uint8 font[]){
    //TODO: update only character that has changed 
    int font_width = font[0];
    for( uint i =0; i < strlen(str);i++){
       GUI_draw_char(str[i],xpos+i*font_width,ypos,color,font);   
    }
}


void GUI_Draw_voltage_level(int volt_mV){
    char str[6];
    sprintf(str, "%02d.%02d V", volt_mV/1000,volt_mV%1000/10);
    // remove leading zero
    if (str[0] == '0'){
        str[0] = ' '; 
    }
    GUI_draw_string(str,220,70,COLORRGB_BLACK,Grotesk32x64);
}
void GUI_Draw_voltage_limit(int volt_mV,bool is_selected,int encodergain){
    int posx = 250;
    int posy = 28;
    
    char str[12];
    sprintf(str, "Vset: %02d.%02dV", volt_mV/1000,volt_mV%1000/10);
    // remove leading zero
    if (str[6] == '0'){
        str[6] = ' '; 
    }
    
    Display_fill_square(posx+16*6,posy+32,16*5,4,COLORRGB_WHITE); // clearing encoder pos marker
    if (is_selected){
        GUI_draw_string(str,posx,posy,COLORRGB_RED,Grotesk16x32);   
        if (encodergain < 10){
            //Display_fill_square(posx+16*10,posy+32,16,4,COLORRGB_RED);     
        }
        else if (encodergain < 100){
            Display_fill_square(posx+16*10,posy+32,16,4,COLORRGB_RED);   
        }
        else if (encodergain < 1000){
            Display_fill_square(posx+16*9,posy+32,16,4,COLORRGB_RED);    
        }
        else if (encodergain < 10000){
            Display_fill_square(posx+16*7,posy+32,16,4,COLORRGB_RED);     
        }

    }
    else {
        GUI_draw_string(str,posx,posy,COLORRGB_BLACK,Grotesk16x32);
        
    }
    

    
}

void GUI_Draw_current_level(int amp_mA){
    char str[6];
    sprintf(str, "%03d mA",amp_mA%1000);
    // remove leading zero
    if (str[0] == '0'){
        str[0] = ' '; 
    }
    GUI_draw_string(str,246,230,COLORRGB_BLACK,Grotesk32x64);
}

void GUI_Draw_current_limit(int amp_mA,bool is_selected,int encodergain){
    int posx = 250;
    int posy = 190;
    
    char str[14];
    sprintf(str, "Iset: %03dmA",amp_mA%1000);
    // remove leading zero
    if (str[5] == '0'){
        //str[5] = ' '; 
    }
    Display_fill_square(posx+16*6,posy+32,16*5,4,COLORRGB_WHITE); // clearing encoder pos marker
    if (is_selected){
        GUI_draw_string(str,posx,posy,COLORRGB_RED,Grotesk16x32); 
        if (encodergain < 10){
            Display_fill_square(posx+16*8,posy+32,16,4,COLORRGB_RED);
        }
        else if (encodergain < 100){
            Display_fill_square(posx+16*7,posy+32,16,4,COLORRGB_RED);
        }
        else if (encodergain < 1000){
            Display_fill_square(posx+16*6,posy+32,16,4,COLORRGB_RED);   
        }
        else if (encodergain < 10000){
           // Display_fill_square(posx+16*3,posy+32,16,4,COLORRGB_RED);
        }

    }
    else {
        GUI_draw_string(str,posx,posy,COLORRGB_BLACK,Grotesk16x32);
    }   
}

void GUI_Draw_power_level(int pow_mW){
    char str[6];
    sprintf(str, "%02d.%02d W", pow_mW/1000,pow_mW%1000/10);
    // remove leading zero
    if (str[0] == '0'){
        str[0] = ' '; 
    }
    GUI_draw_string(str,280,145,COLORRGB_BLACK,Grotesk16x32);
}


int last_battery_prosent = -1;
int last_battery_timeleft_s = -1; 
void GUI_Draw_battery_info(int charge_prosent, int timeleft_s, int isCharging){
    // do not spend time drawing when nothing is updated
    if(last_battery_prosent == charge_prosent && last_battery_timeleft_s ==timeleft_s){
        return;
    }
    last_battery_prosent = charge_prosent;
    last_battery_timeleft_s = timeleft_s;
    
    char str[13];
    sprintf(str, "%02d%% ", charge_prosent);
    GUI_draw_string(str,70,37,COLORRGB_BLACK,Grotesk16x32);
    sprintf(str, "%dh%02d ", timeleft_s/3600,(timeleft_s%3600)/60);
    GUI_draw_string(str,70,37+32,COLORRGB_BLACK,Grotesk16x32);
    GUI_Draw_battery_symbol(30,35,charge_prosent,isCharging);
}
 void GUI_Draw_battery_symbol(int xpos,int ypos,uint8 fill_prosentge, uint8 isCharging){
    int width = 25; 
    int height = 70;
    int border_tickness = 5; 
    // draw borders
    Display_fill_square(xpos+border_tickness/2,ypos-border_tickness*2,width-border_tickness,border_tickness,COLORRGB_BLACK);
    Display_fill_square(xpos-border_tickness,ypos-border_tickness,width+border_tickness*2,border_tickness,COLORRGB_BLACK);
    Display_fill_square(xpos-border_tickness,ypos,border_tickness,height,COLORRGB_BLACK);
    Display_fill_square(xpos+width,ypos,border_tickness,height,COLORRGB_BLACK);
    Display_fill_square(xpos-border_tickness,ypos+height,width+border_tickness*2,border_tickness,COLORRGB_BLACK);
    
    
    //Display_fill_square(xpos,ypos,width,height,COLORRGB_WHITE);
    Display_fill_square(xpos,ypos+height-(int)fill_prosentge/(100.0/height),width,(int)fill_prosentge/(100.0/height),COLORRGB_GREEN);
    Display_fill_square(xpos,ypos,width,height-(int)fill_prosentge/(100.0/height),COLORRGB_WHITE);
    
    // draw charging icon
    if(isCharging){
        for (int y =0; y < height/6; y++){
            for (int x =0; x < width/5; x++){
                Display_draw_pixel(xpos+width*2/3+x-y,ypos+height/3+y,COLORRGB_YELLOW);
            }
        }
        Display_fill_square(xpos+width/6,ypos+height/2-width/12,width*2/3,width/5,COLORRGB_YELLOW);
        for (int y =0; y < height/6; y++){
            for (int x =0; x < width/6; x++){
                Display_draw_pixel(xpos+width*1/3-width/5+x+y,ypos+height*2/3-y,COLORRGB_YELLOW);
            }
        }
    }
}

void GUI_Draw_on_off(bool is_on){
    
    char str_output[] = "Output:";
    GUI_draw_string(str_output,20,190,COLORRGB_BLACK,Grotesk16x32);

    if(is_on){
        char str_onoff[] = "ON ";
        GUI_draw_string(str_onoff,30,230,COLORRGB_GREEN,Grotesk32x64);
    }
    else{
        char str_onoff[] = "OFF";
        GUI_draw_string(str_onoff,30,230,COLORRGB_RED,Grotesk32x64);
    }
}
void GUI_draw_logo(){
    Display_fill_screen(COLORRGB_WHITE);
    GUI_draw_string("BLUE SMOKE",85,100,COLORRGB_BLUE,Grotesk32x64);
    GUI_draw_string("Electronics",145,185,COLORRGB_BLACK,Grotesk16x32);
}

/* [] END OF FILE */
