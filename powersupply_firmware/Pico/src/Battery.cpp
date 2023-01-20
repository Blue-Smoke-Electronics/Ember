#include "Battery.h"
#include "Analog.h"
#include "Flash.h"
#include "PSU.h"
#include "Pcb.h"
#include "Booster.h"
#include "LinReg.h"
#include "Onoff.h"

const float Battery::maxCapacity = 3.65 * 1500 * 3600; //V*mA*s/h == mJ // 3.65 is avrage voltage lever during messurements

const uint32_t Battery::update_freq_us = 2 * 1000 * 1000;
uint32_t Battery::update_timer;

float Battery::capasityLeft = 0;

void Battery::Init(){
    Analog::Init();
    gpio_init(Pcb::usb_connected_pin);
    gpio_set_dir(Pcb::usb_connected_pin,GPIO_IN);

    gpio_init(Pcb::battery_charger_status_pin);
    gpio_set_dir(Pcb::battery_charger_status_pin,GPIO_IN);

    capasityLeft = Flash::GetBatteryCapacity();
}

void Battery::Update(){
    if(time_us_32() - update_timer > update_freq_us)
    {   
        uint32_t delta_t = time_us_32() - update_timer;
        update_timer = time_us_32();

        capasityLeft -= delta_t/1000000.0f *  GetTotalPowerDraw();
        
        if(capasityLeft > maxCapacity)
            capasityLeft = maxCapacity;

        if(capasityLeft < 0)
            capasityLeft =0; 

       /* if (GetVoltage() > 4.095f) // adc caped at 4096
            capasityLeft = maxCapacity*0.8f > capasityLeft ? maxCapacity*0.8f :  capasityLeft;
            //capasityLeft = maxCapacity;

        if(GetVoltage() > 3.92f)
            //capasityLeft = maxCapacity*0.8f > capasityLeft ? maxCapacity*0.8f :  capasityLeft;

        if(GetVoltage() > 3.77f)
            //capasityLeft = maxCapacity*0.6f > capasityLeft ? maxCapacity*0.6f :  capasityLeft;

        if(GetVoltage() > 3.7f)
            //capasityLeft = maxCapacity*0.4f > capasityLeft ? maxCapacity*0.4f :  capasityLeft;

        if(GetVoltage() > 3.63f)
            //capasityLeft = maxCapacity*0.2f > capasityLeft ? maxCapacity*0.2f :  capasityLeft;
        */
        
        if (GetVoltage() < 3.0f){
            capasityLeft = 0;
            PSU::Disable();
        }

        if (GetVoltage() < 2.8f){
            capasityLeft = 0;
            Onoff::Turn_off_device();
        }
    }
}  

float Battery::GetQuiescentPower(){
    //unit is mW
    return 300; // measured values, avrage over different battery voltages
}

float Battery::GetPsuQuiescentPower(){
    //unit is mW
    return 10 * PSU::getVoltage(); // mostly do to minimum current draw circuit
}

float Battery::GetChargingPower(){
    //unit is mW
    return 5 * 450;
}

bool Battery::IsChargerConnected(){
    return gpio_get(Pcb::usb_connected_pin);
}

bool Battery::IsCharging(){
    return GetTotalPowerDraw() <= 0;
}

bool Battery::GetChargerStatus(){
    return !gpio_get(Pcb::usb_connected_pin);
}

float Battery::GetTotalPowerDraw(){
    float totalpower = 0;
    
    totalpower += GetQuiescentPower();
    if(PSU::IsEnabled()){
        float linRegLosses = (Booster::GetVoltage() - LinReg::GetVoltage()) * LinReg::GetCurrent();
        float boosterLosses = PSU::getPower()* 1000 * 0.4; 
        totalpower += GetPsuQuiescentPower() + PSU::getPower() * 1000 + linRegLosses + boosterLosses;
    }

    if(IsChargerConnected())
        totalpower -= GetChargingPower();

    return totalpower; // unit is mW // negativ if device is charging
}

float Battery::GetVoltage(){
    return Analog::GetBatteryVoltage();
}

float Battery::GetBatteryLife_s(){
    int lifetime=0;
    float powerdraw = GetTotalPowerDraw();
    if(powerdraw < 0)
       lifetime= (maxCapacity - capasityLeft) / (-powerdraw);
    else
        lifetime = capasityLeft/powerdraw; 

    if (lifetime > 172800) // cap at 48 houers
        lifetime = 172800;

    if (lifetime < 0)
        lifetime =0;

    return lifetime; 
}

float Battery::GetBatteryProcentage(){
    return (capasityLeft / maxCapacity) * 100.0f;
}

float Battery::GetCapasityLeft(){
    return capasityLeft;
}
