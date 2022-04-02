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

#ifndef ANALOG_H
#define ANALOG_H

void analog_init();
int analog_get_voltage();
int analog_get_current();
int analog_get_battery_current();
int analog_get_usb_current();
int analog_get_battery_voltage1();
int analog_get_battery_voltage2();
int analog_get_battery_voltage3();
int analog_get_battery_voltage4();
int analog_get_battery_voltage5();
int analog_get_battery_voltage6();

#endif