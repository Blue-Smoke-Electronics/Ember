#include "Onoff.h"
#include "pico/stdlib.h"
#include "Pcb.h"
#include "Flash.h"
#include "Battery.h"
#include "PSU.h"
#include "hardware/watchdog.h"

bool Onoff::watchdogenabled = false; 

void Onoff::Init(){
    // keap device powerd on when powerswitch releases 
    gpio_init(Pcb::on_off_latch_pin);
    gpio_set_dir(Pcb::on_off_latch_pin, GPIO_OUT);
    gpio_put(Pcb::on_off_latch_pin,true);

    //watchdog_enable(10000,true); // give time for device to start up 

}


void Onoff::Turn_off_device(){
    Flash::Save(Flash::batteryCapacity,Battery::GetCapasityLeft());
    Flash::Save(Flash::outputVoltage,PSU::getTargetVoltage());
    Flash::Save(Flash::outputCurrent,PSU::getTargetCurrent());
    gpio_put(Pcb::on_off_latch_pin,false);
}

void Onoff::KeepAlive(){
    // can not be starte in init
    if (!watchdogenabled){
        watchdog_enable(1000,true);
        watchdogenabled = true; 
    }
    
    watchdog_update();
}