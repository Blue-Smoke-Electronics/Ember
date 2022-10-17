#include "Onoff.h"
#include "pico/stdlib.h"
#include "Pcb.h"
#include "Flash.h"
#include "Battery.h"
#include "PSU.h"
#include "hardware/watchdog.h"
#include "GUI.h"

bool Onoff::IsOn = false;

bool Onoff::skip_next_disable = false;

void Onoff::Init(){
    // keep device power on when powerswitch releases
    gpio_init(Pcb::on_off_latch_pin);
    gpio_set_dir(Pcb::on_off_latch_pin, GPIO_OUT);
    gpio_put(Pcb::on_off_latch_pin, false); // if not expicitly turned on, remain off when disconnected

    if (gpio_get(Pcb::usb_connected_pin)) // pluging in usb turns on device
        IsOn = false;
    else // pressing power on button turns on device
       Turn_on_device();
}

void Onoff::Turn_off_device(){
    //printf("starting shutdown\r\n");
    IsOn = false;
    Flash::Save(Battery::GetCapasityLeft(), PSU::getTargetVoltage(), PSU::getTargetCurrent());

    //printf("shutdown compleated \r\n\n");
    gpio_put(Pcb::on_off_latch_pin, false); // turn device off, if usb is not connected
    PSU::Disable();
}

void Onoff::Turn_on_device(){
    IsOn = true;
    gpio_put(Pcb::on_off_latch_pin, true);
}   
