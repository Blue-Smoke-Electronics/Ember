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
#ifndef BOOSTER_H
#define BOOSTER_H

#include "project.h"


void Booster_init();
uint8_t i2c_read_reg(uint8_t reg);
void i2c_write_reg(uint8_t reg,uint8_t data);
void Booster_set_voltage(uint16_t mV);
void Booster_set_current_limit(uint16_t mA);
void Booster_print_status();
CY_ISR(booster_fault_interrupt_handler);

#endif 
/* [] END OF FILE */
