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

#include "statemachine.h"
#include "GUI.h"
#include <stdio.h>
#include "uart.h"
#include "display.h"
#include "color.h"
#include "GUI.h"
#include "BigFont.h"
#include "ArialNumFontPlus.h"
#include "knobs.h"
#include "led.h"
#include "linReg.h"
#include "booster.h"
#include "analog.h"
#include "psu.h"
#include "eeprom.h"
#include "battery.h"


typedef enum {
    booting,
    idle,
    set_voltage,
    set_current,
    keypad,
    
} statemachine_state_t;



statemachine_state_t statemachine_state = booting; 
int max_voltage_input;
int max_voltage_value; 
int max_current_input;
int max_current_value;

int encoder_gain_current; 
int encoder_gain_voltage;


uint32 boot_time_start = 0; 
uint32 idle_loop_start =0; 
uint32 last_voltage_current_sens_update =0; 
uint32 last_battery_level_update =0;
uint32 last_psu_update =0;
uint32 voltage_setting_timout =0; 
uint32 current_setting_timout =0;
uint32 last_uart_update =0;

int setup =1;
void statemachine_loop(){

    if(statemachine_state == booting){
        if (setup){
            setup =0;
            
            
            boot_time_start = Timer_now_ReadCounter(); 
            
            Led_init();
            led_mcu_on();
            
            Display_init();
            GUI_draw_logo();
            printf("boot time GUI: %lu\n\r",boot_time_start-Timer_now_ReadCounter()); 
            
            Usb_uart_init();
            printf("boot time uart: %lu\n\r",boot_time_start-Timer_now_ReadCounter()); 
            
            
            
            analog_init();
            printf("boot time knobs and leds: %lu\n\r",boot_time_start-Timer_now_ReadCounter());            
            
            psu_init();
            
            eeprom_init();
            
            max_voltage_value = eeprom_read_max_voltage_value();
            max_current_value = eeprom_read_max_current_value();
            encoder_gain_current = eeprom_read_encoder_gain_current();
            encoder_gain_voltage = eeprom_read_encoder_gain_voltage();
            if (encoder_gain_current == 0){ // handle first run when eprom has no stored values
                encoder_gain_current=1;
            }
            if (encoder_gain_voltage == 0){ // handle first run when eprom has no stored values
                encoder_gain_voltage=1;
            }
            
            
    
        }
        
        // show logo atleast for 1.5 secounds
        if (boot_time_start-Timer_now_ReadCounter() >= 1500 ){
            setup =1; 
            statemachine_state = idle; 
        }
    }
                
    if (statemachine_state == idle){
        if (setup){
            setup =0;
            Display_fill_screen(COLORRGB_WHITE);
            GUI_Draw_voltage_limit(max_voltage_value,0,0);
            GUI_Draw_current_limit(max_current_value,0,0);
            psu_set_voltage(max_voltage_value);
            psu_set_current(max_current_value);
            
            Knobs_init();
        }
        
        led_sw_voltage_off();
        led_sw_current_off();
        
        if(last_uart_update - Timer_now_ReadCounter() >= 100){
            last_uart_update = Timer_now_ReadCounter();
            Usb_uartprocess_resived_data();
        }

        if(knobs_get_voltage_switch_presses()){
            statemachine_state = set_voltage;
            setup =1; 
        }
        
        if(knobs_get_current_switch_presses()){
            statemachine_state = set_current;
            setup =1; 
        }
            
    }    
    
    if(statemachine_state == set_voltage){
        if (setup){
            setup =0;
            led_sw_voltage_on();
            voltage_setting_timout = Timer_now_ReadCounter(); 
            max_voltage_input = max_voltage_value; 
            GUI_Draw_voltage_limit(max_voltage_value,true,encoder_gain_voltage);
            
             // clear encoder presses that happend outside this mode 
            knobs_get_encoder_switch_presses();
            knobs_get_encoder_steps();
        }
        if (knobs_get_encoder_switch_presses()){
            voltage_setting_timout = Timer_now_ReadCounter(); 
            encoder_gain_voltage/=10;
            if (encoder_gain_voltage < 10){
                encoder_gain_voltage = 1000; 
            }
            GUI_Draw_voltage_limit(max_voltage_input,true,encoder_gain_voltage);
            eeprom_write_encoder_gain_voltage(encoder_gain_voltage);
        }
        
        int steps = knobs_get_encoder_steps(); // handle multiple steps  
        if(steps != 0 ){
            voltage_setting_timout = Timer_now_ReadCounter();
            max_voltage_input = max_voltage_input + encoder_gain_voltage*steps;
            if (max_voltage_input > 12000){
                max_voltage_input = 12000; 
            }
            if (max_voltage_input < 0){
                max_voltage_input = 0; 
            }
            GUI_Draw_voltage_limit(max_voltage_input,true,encoder_gain_voltage);
            
         
        }
        
        if(knobs_get_voltage_switch_presses()){
            statemachine_state = idle;
            max_voltage_value = max_voltage_input; 
            psu_set_voltage(max_voltage_value);
            GUI_Draw_voltage_limit(max_voltage_value,0,0);
            eeprom_write_max_voltage_value(max_voltage_value);
            
        }
        
        if( voltage_setting_timout - Timer_now_ReadCounter() >=5000){
            statemachine_state = idle;
            GUI_Draw_voltage_limit(max_voltage_value,0,0);
        }
        
        
    }
    
    if( statemachine_state == set_current){
        if (setup){
            setup =0;
            led_sw_current_on();
            current_setting_timout = Timer_now_ReadCounter();
            max_current_input = max_current_value; 
            GUI_Draw_current_limit(max_current_value,1,encoder_gain_current);
            
            // clear encoder presses that happend outside this mode 
            knobs_get_encoder_switch_presses();
            knobs_get_encoder_steps();
        }
        if (knobs_get_encoder_switch_presses()){
            current_setting_timout = Timer_now_ReadCounter();
            encoder_gain_current/=10;
            if (encoder_gain_current < 1){
                encoder_gain_current = 100; 
            }
            GUI_Draw_current_limit(max_current_input,true,encoder_gain_current);
            eeprom_write_encoder_gain_current(encoder_gain_current);
        }
        
        int steps = knobs_get_encoder_steps(); // handle multiple steps  
        if(steps != 0 ){
            current_setting_timout = Timer_now_ReadCounter();
            max_current_input = max_current_input + encoder_gain_current*steps;
            if (max_current_input > 500){
                max_current_input = 500; 
            }
            if (max_current_input < 0){
                max_current_input = 0; 
            }
            GUI_Draw_current_limit(max_current_input,true,encoder_gain_current);
            
            
        }
        
        if(knobs_get_current_switch_presses()){
            statemachine_state = idle;
            max_current_value = max_current_input; 
            psu_set_current(max_current_value);
            GUI_Draw_current_limit(max_current_value,0,0);
            eeprom_write_max_current_value(max_current_value);
            
        }
        
        if( current_setting_timout - Timer_now_ReadCounter() >=5000){
            statemachine_state = idle;
            GUI_Draw_current_limit(max_current_value,0,0);
            
        } 
    }
    
    if (statemachine_state == idle || statemachine_state == set_current || statemachine_state == set_voltage){
        idle_loop_start=Timer_now_ReadCounter(); 
        if(last_voltage_current_sens_update - Timer_now_ReadCounter() >= 100){ 
            last_voltage_current_sens_update = Timer_now_ReadCounter();
            GUI_Draw_voltage_level(psu_get_voltage());
            GUI_Draw_current_level(psu_get_current());
            GUI_Draw_power_level(psu_get_power());
            
        }
        if(last_battery_level_update-Timer_now_ReadCounter() >= 1000){
            last_battery_level_update = Timer_now_ReadCounter();
            GUI_Draw_battery_info(battery_get_procentage(),battery_get_lifetime_s(), battery_isCharging()); // 49020 sec is 13h33 min 
        }
        
        if (last_psu_update-Timer_now_ReadCounter() >= 100){
            last_psu_update = Timer_now_ReadCounter(); 
            
            psu_regulator_loop(); 
            
            if ( psu_get_status() == psu_status_ON){
                GUI_Draw_on_off(1); 
                led_on_off_on();
                if (knobs_get_on_off_switch_presses()){
                    psu_stop();       
                }
                
            }else{
                GUI_Draw_on_off(0);
                led_on_off_off();
                if (knobs_get_on_off_switch_presses()){
                    psu_start();       
                }
            }
        } 
        
        //printf("idle loop time: %lu\n\r",idle_loop_start-Timer_now_ReadCounter());
    }
}
