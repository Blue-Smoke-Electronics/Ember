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
#ifndef BUCK_H
#define BUCK_H

#include "project.h"


void Buck_init();
uint8_t i2c_read_reg(uint8_t reg);
void i2c_write_reg(uint8_t reg,uint16_t data);
void Buck_set_voltage(uint16_t mV);
void Buck_set_current_limit(uint16_t mA);
void Buck_print_status();
CY_ISR(buck_fault_interrupt_handler);

#endif 
/* [] END OF FILE */
