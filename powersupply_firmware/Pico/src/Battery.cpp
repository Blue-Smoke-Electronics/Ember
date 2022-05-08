#include "Battery.h"
#include "Analog.h"
#include "Flash.h"
#include "PSU.h"
#include "Pcb.h"

const float Battery::maxCapacity =3.7*4000*3600 ; //V*mA*s/h

const uint32_t Battery::update_freq_us = 500*1000; 
uint32_t Battery::update_timer;

float Battery::capasityLeft =0; 

void Battery::Init(){
    Analog::Init(); 
    gpio_init(Pcb::usb_connected_pin);
    gpio_set_dir(Pcb::usb_connected_pin,GPIO_IN);

    capasityLeft = *Flash::batteryCapacity; 
    
}
void Battery::Update(){
    if(time_us_32() - update_timer > update_freq_us )
    {   
        uint32_t delta_t = time_us_32() - update_timer; 
        update_timer = time_us_32();  

        capasityLeft -= delta_t/1000000.0f *  GetTotalPowerDraw();
        
        if(capasityLeft > maxCapacity){
            capasityLeft = maxCapacity; 
        }
        if(capasityLeft < 0){
            capasityLeft =0; 
        }
        if (GetVoltage() > 4095){ // adc caped at 4096
            capasityLeft = maxCapacity;
        }
        if(GetVoltage() > 3920){
            capasityLeft = maxCapacity*0.8f > capasityLeft ? maxCapacity*0.8f :  capasityLeft; 
        }
        if(GetVoltage() > 3770){
            capasityLeft = maxCapacity*0.6f > capasityLeft ? maxCapacity*0.6f :  capasityLeft; 
        }
        if(GetVoltage() > 3700){
            capasityLeft = maxCapacity*0.4f > capasityLeft ? maxCapacity*0.4f :  capasityLeft; 
        }
        if(GetVoltage() > 3630){
            capasityLeft = maxCapacity*0.2f > capasityLeft ? maxCapacity*0.2f :  capasityLeft; 
        }
        
        if (GetVoltage() < 3300){
            capasityLeft = 0;
        }

    }
}  

float Battery::GetQuisentPower(){
    //unit is mW
    return 150*GetVoltage()/1000.0f; // guestimated values. Todo: mesure values
}
float Battery::GetPsuQuisentPower(){
    //unit is mW
    return 10*PSU::getVoltage()/1000.0f; // 
}
float Battery::GetChargingPower(){
    //unit is mW
    return 5*380; 
}
bool Battery::IsChargerConnected(){
    return gpio_get(Pcb::usb_connected_pin); 
}
bool Battery::IsCharging(){
    return IsChargerConnected();
}
float Battery::GetTotalPowerDraw(){
    return 0.0f; 
}
float Battery::GetVoltage(){
    return 0.0f; 
}
float Battery::GetBatteryLife_s(){
    return 0.0f; 
}
float Battery::GetBatteryProcentage(){
    return 0.0f; 
}

float Battery::GetCapasityLeft(){
    return capasityLeft; 
}