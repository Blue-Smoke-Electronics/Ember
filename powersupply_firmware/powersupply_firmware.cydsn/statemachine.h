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

#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "project.h"
    
    
//void statemachine_init();
void handle_encoder_left();
void handle_encoder_right();
void handle_encoder_click();
void handle_sw_voltage_click();
void handle_sw_current_click();
void handle_sw_on_off_click();
//void handle_set_voltage_comand(int voltage);
//void handle_set_current_comand(int current);

CY_ISR(timer_timout_interrupt_handler);
void timeout_timer_Reset();
void statemachine_loop();
    
#endif

/* [] END OF FILE */
