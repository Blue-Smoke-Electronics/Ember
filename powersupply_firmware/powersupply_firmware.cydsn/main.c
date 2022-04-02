#include <project.h>
#include <stdio.h>
#include "uart.h"
#include "display.h"
#include "color.h"
#include "GUI.h"
#include "BigFont.h"
#include "ArialNumFontPlus.h"
#include "knobs.h"
#include "statemachine.h"
#include "led.h"
#include "linReg.h"
#include "buck.h"
#include "analog.h"
#include "battery.h"

#include "eeprom.h"



int main()
{
    POWER_LATCH_Write(1); // makes sure power stays on
    CyGlobalIntEnable; /* Enable global interrupts. */
    Debug_uart_init(); 
    battery_init();
    
    
    printf("starting main loop\r\n");
    Timer_now_Start();
    
    
    OUTPUT_ENABLE_LED_Write(0);
    int onoff_slowrelease=0;
    for(;;)
    {
        statemachine_loop();
        battery_liftime_update_loop();
        
        //Buck_print_status();
        
        if(!POWER_SWITCH_Read() ){
            onoff_slowrelease = 1; 
        }
        if(POWER_SWITCH_Read() && onoff_slowrelease){
            POWER_LATCH_Write(0);
        }
        
        printf("\n\n\n\n\n\n\n\n");
        printf("battery1:        %dmV\r\n",analog_get_battery_voltage1());
        printf("battery2:        %dmV\r\n",analog_get_battery_voltage2());
        printf("battery3:        %dmV\r\n",analog_get_battery_voltage3());
        printf("battery4:        %dmV\r\n",analog_get_battery_voltage4());
        printf("battery5:        %dmV\r\n",analog_get_battery_voltage5());
        printf("battery6:        %dmV\r\n",analog_get_battery_voltage6());
        printf("current:         %dmA\r\n",analog_get_current());
        printf("battery current: %dmA\r\n",analog_get_battery_current());
        printf("USB current:     %dmA\r\n",analog_get_usb_current());
        
        
        CyDelay(2000);
    }
}