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
#include "booster.h"
#include "analog.h"
#include "battery.h"

#include "eeprom.h"



int main()
{
 
    CyGlobalIntEnable; /* Enable global interrupts. */
    Debug_uart_init(); 
    battery_init();
    
    
    printf("starting main loop\r\n");
    Timer_now_Start();

    for(;;)
    {
        statemachine_loop();
        battery_liftime_update_loop();
        
        Booster_print_status();
       
    }
}