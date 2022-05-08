#include "Knobs.h"
#include "Pcb.h"
#include "hardware/gpio.h"
#include "PSU.h"
#include "GUI.h"
#include "Onoff.h"


uint32_t Knobs::update_timer;


volatile bool Knobs::on_off_switch_is_pushed = false;
volatile int Knobs::voltage_encoder_cnt; 
volatile int Knobs::current_encoder_cnt; 



void Knobs::Init(){

        
    gpio_set_pulls(Pcb::on_off_switch_pin,false,false); 
    gpio_set_pulls(Pcb::ouput_on_off_switch_pin,false,false); 
    gpio_set_pulls(Pcb::encoder_current_A_pin,false,false);
    gpio_set_pulls(Pcb::encoder_current_B_pin,false,false);
    gpio_set_pulls(Pcb::encoder_current_switch_pin,false,false);
    gpio_set_pulls(Pcb::encoder_voltage_A_pin,false,false);
    gpio_set_pulls(Pcb::encoder_voltage_B_pin,false,false);
    gpio_set_pulls(Pcb::encoder_voltage_switch_pin,false,false);

    gpio_set_irq_enabled_with_callback(Pcb::on_off_switch_pin,GPIO_IRQ_EDGE_RISE,true,&IRS);
    gpio_set_irq_enabled_with_callback(Pcb::ouput_on_off_switch_pin,     GPIO_IRQ_EDGE_FALL, true, &IRS);
    gpio_set_irq_enabled_with_callback(Pcb::encoder_current_A_pin,      GPIO_IRQ_EDGE_FALL, true, &IRS);
    gpio_set_irq_enabled_with_callback(Pcb::encoder_current_switch_pin, GPIO_IRQ_EDGE_FALL, true, &IRS);
    gpio_set_irq_enabled_with_callback(Pcb::encoder_voltage_A_pin,      GPIO_IRQ_EDGE_FALL, true, &IRS);
    gpio_set_irq_enabled_with_callback(Pcb::encoder_voltage_switch_pin, GPIO_IRQ_EDGE_FALL, true, &IRS);


}

void Knobs::Update(){
    if(time_us_32() - update_timer > update_freq_us ){
        update_timer = time_us_32() ;

        if (on_off_switch_is_pushed){
            on_off_switch_is_pushed = false; 
            if(PSU::IsEnabled()){
                PSU::Disable();
            }
            else {
                PSU::Enable(); 
            }
        }
        PSU::ChangeVoltage(voltage_encoder_cnt*GUI::GetVoltageScaler());
        voltage_encoder_cnt =0; 
        PSU::ChangeCurrent(current_encoder_cnt*GUI::GetCurrentScaler());
        current_encoder_cnt =0; 
    }
} 
void Knobs::IRS(uint gpio, uint32_t events){
    switch (gpio){
        case Pcb::on_off_switch_pin:
                Onoff::Turn_off_device();
            break; 
        case Pcb::ouput_on_off_switch_pin:
                on_off_switch_is_pushed = true; 
            break;  
        case Pcb::encoder_voltage_switch_pin:

            break; 

        case Pcb::encoder_current_switch_pin:

            break; 
        case Pcb::encoder_voltage_A_pin: 
            if (gpio_get(Pcb::encoder_voltage_B_pin)){
                voltage_encoder_cnt++; 
            }
            else{
                voltage_encoder_cnt--; 
            }
         break;

        case Pcb::encoder_current_A_pin:
            if (gpio_get(Pcb::encoder_current_B_pin)){
                current_encoder_cnt++; 
            }
            else{
                current_encoder_cnt--; 
            }
            break;




    }

}