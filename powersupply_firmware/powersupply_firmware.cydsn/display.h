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

#include "project.h"
#include "color.h"

#define DISPLAY_COMAND 0
#define DISPLAY_DATA 1

void Display_init();

void Display_draw_pixel(uint16 x,uint16 y,color_rgb color);
void Display_fill_square(uint16 x,uint16 y,uint16 width, uint16 heigth, color_rgb color);
void Display_fill_screen(color_rgb color);

void Display_draw_smily(color_rgb color);

#endif