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

#include "led.h"


void Led_init(){
    
}
void on_off_led_set(uint8_t prosentage_on){
    led_on_off_off();
    led_mcu_off();
    led_sw_voltage_off();
    led_sw_current_off();
}

void led_on_off_on(){
    OUTPUT_ENABLE_LED_Write(1);
}
void led_on_off_off(){
    OUTPUT_ENABLE_LED_Write(0);
}

void led_mcu_on(){
    MCU_LED_Write(1);
}
void led_mcu_off(){
    MCU_LED_Write(0);
}

void led_sw_voltage_on(){
    SW_VOLTAGE_LED_Write(1);
}
void led_sw_voltage_off(){
    SW_VOLTAGE_LED_Write(0);
}

void led_sw_current_on(){
    SW_CURRENT_LED_Write(1);
}
void led_sw_current_off(){
    SW_CURRENT_LED_Write(0);
}