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

uint32_t Onoff::update_timer;


void Onoff::Init(){
    // keep device power on when powerswitch releases
    gpio_init(Pcb::on_off_latch_pin);
    gpio_set_dir(Pcb::on_off_latch_pin, GPIO_OUT);
    gpio_put(Pcb::on_off_latch_pin, true); 

    if (gpio_get(Pcb::usb_connected_pin)) // pluging in usb turns on device
        IsOn = false;
    else // pressing power on button turns on device
       Turn_on_device();

    update_timer =0; 
}

void Onoff::Update(){
    if(time_us_32() - update_timer > update_freq_us) {
        update_timer = time_us_32(); 

        if(!IsOn && !Battery::IsChargerConnected())
        {
            Flash::Save(Battery::GetCapasityLeft(), PSU::getTargetVoltage(), PSU::getTargetCurrent());
            printf("saving and shutting down\r\n");
            gpio_put(Pcb::on_off_latch_pin, false); // turn device off
        }
    }
}

void Onoff::Turn_off_device(){
    IsOn = false;
    PSU::Disable();
}

void Onoff::Turn_on_device(){
    IsOn = true;
    gpio_put(Pcb::on_off_latch_pin, true);
}   
