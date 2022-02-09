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
#ifndef BATTERY_H
#define BATTERY_H
    
void battery_init();
    
void battery_liftime_update_loop();
float battery_get_quisent_power();
float battery_get_charging_power();
float battery_get_psu_quisent_power();
int battery_charger_connected();
int battery_isCharging();
float battery_get_voltage();
float battery_get_total_power_draw();
float battery_get_max_capasity();
int battery_get_lifetime_s();
int battery_get_procentage();

    
    
    
#endif