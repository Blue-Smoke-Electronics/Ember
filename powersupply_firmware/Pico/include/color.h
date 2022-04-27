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


#ifndef COLOR_H
#define COLOR_H

/* [] END OF FILE */
#include <stdbool.h>
#include "pico/stdlib.h"

#define COLORRGB_BLACK (color_rgb){0 ,0 ,0}
#define COLORRGB_WHITE (color_rgb){255 ,255 ,255}
#define COLORRGB_RED (color_rgb){255 ,0 ,0}
#define COLORRGB_GREEN (color_rgb){0,255,0}
#define COLORRGB_BLUE (color_rgb){0 ,0 ,255}
#define COLORRGB_GRAY (color_rgb){50 ,50 ,50}
#define COLORRGB_YELLOW (color_rgb){255 ,255 ,0}    

typedef struct color_rgb{
    uint8_t red ; 
    uint8_t green;
    uint8_t blue;
} color_rgb;

bool colors_is_equal(color_rgb c1,color_rgb c2);

#endif