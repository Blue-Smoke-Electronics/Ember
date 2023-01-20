#include <stdio.h>
#include "pico/stdlib.h"

#include "Knobs.h"
#include "Pcb.h"
#include "hardware/gpio.h"
#include "PSU.h"
#include "GUI.h"
#include "Onoff.h"

uint32_t Knobs::update_timer;

volatile bool Knobs::on_off_switch_is_pushed = false;
int Knobs::on_off_switch_skipp_first; 

volatile bool Knobs::power_switch_is_pushed = false;
volatile int Knobs::voltage_encoder_cnt; 
volatile int Knobs::current_encoder_cnt; 

void Knobs::Init(){
    gpio_set_pulls(Pcb::on_off_switch_pin, false, false);
    gpio_set_pulls(Pcb::ouput_on_off_switch_pin, false, false);
    gpio_set_pulls(Pcb::encoder_current_A_pin, false, false);
    gpio_set_pulls(Pcb::encoder_current_B_pin, false, false);
    gpio_set_pulls(Pcb::encoder_current_switch_pin, false, false);
    gpio_set_pulls(Pcb::encoder_voltage_A_pin, false, false);
    gpio_set_pulls(Pcb::encoder_voltage_B_pin, false, false);
    gpio_set_pulls(Pcb::encoder_voltage_switch_pin, false, false);

    gpio_set_irq_enabled_with_callback(Pcb::on_off_switch_pin,          GPIO_IRQ_EDGE_FALL, true, &IRS);
    gpio_set_irq_enabled_with_callback(Pcb::ouput_on_off_switch_pin,    GPIO_IRQ_EDGE_FALL, true, &IRS);
    gpio_set_irq_enabled_with_callback(Pcb::encoder_current_A_pin,      GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &IRS);
    gpio_set_irq_enabled_with_callback(Pcb::encoder_current_switch_pin, GPIO_IRQ_EDGE_FALL, true, &IRS);
    gpio_set_irq_enabled_with_callback(Pcb::encoder_voltage_A_pin,      GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &IRS);
    gpio_set_irq_enabled_with_callback(Pcb::encoder_voltage_switch_pin, GPIO_IRQ_EDGE_FALL, true, &IRS);

    Knobs::on_off_switch_skipp_first = 5;
}

void Knobs::Update(){
    if(time_us_32() - update_timer > update_freq_us){
        update_timer = time_us_32();

        if (on_off_switch_is_pushed){
            on_off_switch_is_pushed = false;
            if(PSU::IsEnabled()){
                PSU::Disable();
            }
            else {
                if (Onoff::IsOn)
                    PSU::Enable();
            }
        }

        if (power_switch_is_pushed){
            power_switch_is_pushed = false;
            if(on_off_switch_skipp_first == 0){ // becoms 0 when the powerbutton has been released for a short period
                if(Onoff::IsOn)
                    Onoff::Turn_off_device(); 
                else
                    Onoff::Turn_on_device();
            }
        }        

        if (!gpio_get(Pcb::on_off_switch_pin)){ // not holding down power button
            on_off_switch_skipp_first--;
            if (on_off_switch_skipp_first < 0)
                on_off_switch_skipp_first = 0;
        }

        if(Onoff::IsOn){
            PSU::ChangeVoltage(voltage_encoder_cnt*GUI::GetVoltageScaler());
            PSU::ChangeCurrent(current_encoder_cnt*GUI::GetCurrentScaler());
        }

        voltage_encoder_cnt = 0;
        current_encoder_cnt = 0;
    }
}

void Knobs::IRS(uint gpio, uint32_t events){
    switch (gpio){
        case Pcb::on_off_switch_pin:
            power_switch_is_pushed = true;
            break;
        case Pcb::ouput_on_off_switch_pin:
            on_off_switch_is_pushed = true;
            break;
        case Pcb::encoder_voltage_switch_pin:
            GUI::ChangeVoltageScaler();
            break; 
        case Pcb::encoder_current_switch_pin:
            GUI::ChangeCurrentScaler();
            break;
        case Pcb::encoder_voltage_A_pin:
            if (gpio_get(Pcb::encoder_voltage_A_pin)==gpio_get(Pcb::encoder_voltage_B_pin))
                voltage_encoder_cnt--;
            else
                voltage_encoder_cnt++;
            break;
        case Pcb::encoder_current_A_pin:
            if (gpio_get(Pcb::encoder_current_A_pin)==gpio_get(Pcb::encoder_current_B_pin))
                current_encoder_cnt--;
            else
                current_encoder_cnt++;
            break;
    }
}
