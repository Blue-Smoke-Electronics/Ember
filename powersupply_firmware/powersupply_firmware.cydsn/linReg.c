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
#include "linReg.h"
#include "uart.h"

void LinReg_init(){
    PWM_VOLTAGE_LIMIT_Start();
    PWM_VOLTAGE_LIMIT_WriteCompare(1000);
    
    PWM_CURRENT_LIMIT_Start();
    PWM_CURRENT_LIMIT_WriteCompare(1000);
}

void linReg_voltage_limit_set(uint16_t limit_mV){
    PWM_VOLTAGE_LIMIT_WriteCompare(((limit_mV*7272)/1000)/4);
}
void linReg_current_limit_set(uint16_t limit_mA){
    //PWM_CURRENT_LIMIT_WriteCompare(((limit_mA*7272)/200)/5);
    
    float mV = (limit_mA+54)*1.34;
    
    PWM_CURRENT_LIMIT_WriteCompare((uint16_t)(7272*((mV)/1000)));
}