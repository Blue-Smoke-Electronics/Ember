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
#ifndef LED_H
#define LED_H

#include "project.h"



void Led_init();

void led_on_off_on();
void led_on_off_off();

void led_mcu_on();
void led_mcu_off();

void led_sw_voltage_on();
void led_sw_voltage_off();

void led_sw_current_on();
void led_sw_current_off();



#endif 