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

#ifndef PSU_H
#define PSU_H

#define psu_status_OFF 0
#define psu_status_ON 1
#define psu_status_error 2

    
void psu_init();

void psu_regulator_loop();

void psu_start();
void psu_stop();
int psu_get_status();

void psu_set_voltage(int mV);
void psu_set_current(int mA);

int psu_get_voltage();
int psu_get_current();
int psu_get_power();


#endif