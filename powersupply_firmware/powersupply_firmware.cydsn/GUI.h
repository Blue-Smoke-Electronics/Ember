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

#ifndef GUI_H
#define GUI_H
    

#include "color.h"
#include "project.h"
#include <stdbool.h> 
   

    void GUI_draw_char(char c,int xpos, int ypos,color_rgb color,const uint8 font[]);
    void GUI_draw_string(char str [],int xpos, int ypos,color_rgb color,const uint8 font[]);
    
    void GUI_Draw_voltage_level(int volt_mV);
    void GUI_Draw_voltage_limit(int volt_mV,bool is_selected,int encodergain );
    
    void GUI_Draw_current_level(int amp_mA);
    void GUI_Draw_current_limit(int amp_mA,bool is_selected,int encodergain);
    
    void GUI_Draw_power_level(int pow_mW);
    
    void GUI_Draw_battery_info(int charge_prosent, int timeleft_s, int isChargning);
    void GUI_Draw_battery_symbol(int xpos,int ypos,uint8 fill_prosentge,uint8 isCharging);
    
    void GUI_Draw_on_off(bool is_on);
    void GUI_draw_logo();
   
    
    
#endif



/* [] END OF FILE */
