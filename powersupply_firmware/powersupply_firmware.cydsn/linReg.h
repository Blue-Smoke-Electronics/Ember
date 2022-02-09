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
#ifndef LINREG_H
#define LINREG_H

#include "project.h"



void LinReg_init();

void linReg_voltage_limit_set(uint16_t limit_mV);
void linReg_current_limit_set(uint16_t limit_mA);


#endif 