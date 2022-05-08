#include "Onoff.h"
#include "pico/stdlib.h"
#include "Pcb.h"
#include "Flash.h"
#include "Battery.h"
#include "PSU.h"

void Onoff::Init(){
    // keap device powerd on when powerswitch releases 
    gpio_init(Pcb::on_off_latch_pin);
    gpio_set_dir(Pcb::on_off_latch_pin, GPIO_OUT);
    gpio_put(Pcb::on_off_latch_pin,true);


}


void Onoff::Turn_off_device(){
    Flash::Save(Flash::batteryCapacity,Battery::GetCapasityLeft());
    Flash::Save(Flash::outputVoltage,PSU::getTargetVoltage());
    Flash::Save(Flash::outputCurrent,PSU::getTargetCurrent());
    gpio_put(Pcb::on_off_latch_pin,false);
}