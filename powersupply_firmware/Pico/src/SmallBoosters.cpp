#include "SmallBoosters.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include "Pcb.h"    

void SmallBoosters::Init(){
//    TaskHandle_t heartbeat_Task = NULL;
//    uint32_t s1 = xTaskCreate(Hartbeat_task, "Heartbeat", 1024, NULL, 0, &heartbeat_Task);
    gpio_init(Pcb::smallboosters_enable_pin);
    gpio_set_dir(Pcb::smallboosters_enable_pin, GPIO_OUT);
    gpio_put(Pcb::smallboosters_enable_pin, true);

}

void SmallBoosters::Disable(){


    gpio_put(Pcb::smallboosters_enable_pin, false);
    
}
void SmallBoosters::Enable(){


    gpio_put(Pcb::smallboosters_enable_pin, true);
    
}
