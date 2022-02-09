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

#ifndef KNOBS_H
#define KNOBS_H
   
#include <project.h>

void Knobs_init();
    
CY_ISR(sw_encoder_interrupt_handler);
CY_ISR(encoder_interrupt_handler);
CY_ISR(sw_current_interrupt_handler);
CY_ISR(sw_voltage_interrupt_handler);
CY_ISR(sw_on_off_interrupt_handler);
    

int knobs_get_on_off_switch_presses();
int knobs_get_voltage_switch_presses();
int knobs_get_current_switch_presses();
int knobs_get_encoder_switch_presses();
int knobs_get_encoder_steps();
#endif