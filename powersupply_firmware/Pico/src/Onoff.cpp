#include "Onoff.h"
#include "pico/stdlib.h"
#include "Pcb.h"

Onoff::Onoff(){
    // keap device powerd on when powerswitch releases 
    gpio_init(Pcb::on_off_latch_pin);
    gpio_set_dir(Pcb::on_off_latch_pin, GPIO_OUT);
    gpio_put(Pcb::on_off_latch_pin,true);

    // eneable interrupt on power swiwithc 
    gpio_init(Pcb::on_off_switch_pin);
    gpio_set_dir(Pcb::on_off_switch_pin,GPIO_IN);
    gpio_set_pulls(Pcb::on_off_switch_pin,false,false); // disable puldown 

    gpio_set_irq_enabled_with_callback(Pcb::on_off_switch_pin,GPIO_IRQ_EDGE_RISE,true,&IRS);
    this->debounce_timestamp = time_us_32();




}

void Onoff::IRS(uint gpio, uint32_t event){
    if(gpio == Pcb::on_off_switch_pin){
       //turn_off_device();  
       if (time_us_32()-this-debounce_timestamp <  )
       gpio_put(Pcb::on_off_latch_pin,false);
    }
    
}

void Onoff::turn_off_device(){
    gpio_put(Pcb::on_off_latch_pin,false);
}