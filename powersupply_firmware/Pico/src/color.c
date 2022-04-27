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
#include "color.h"

bool colors_is_equal(color_rgb c1,color_rgb c2){
    if(c1.blue == c2.blue && c1.green == c2.green && c1.red == c2.red){
        return true;
    }
    else{
        return false;
    }

}
/* [] END OF FILE */
