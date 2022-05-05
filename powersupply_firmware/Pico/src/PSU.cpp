#include "PSU.h"
#include "LinReg.h"
#include "Booster.h"

uint32_t PSU::update_timer =0; 
float PSU::targetVoltage =0; 
float PSU::targetCurrent =0; 
bool PSU::enabled = false;

void PSU::Init(){
    LinReg::Init(); 
    Booster::Init(); 

    enabled = false; 
    //todo: load from flash
    SetVoltage(2.8f);
    SetCurrent(110.0f);



}

void PSU::Update(){
    Booster::Update(); 
    LinReg::Update(); 

    if(time_us_32() - update_timer > update_freq_us ){
        update_timer = time_us_32() ;
        
        if(enabled){
        LinReg::SetVoltage(targetVoltage);
        LinReg::SetCurrent(targetCurrent); 
        Booster::SetVoltage(LinReg::GetVoltage()+3);
        }
        else{
            LinReg::SetVoltage(0);
            LinReg::SetCurrent(0); 
            Booster::SetVoltage(0);
        }
        
    }
}

float PSU::getVoltage(){
    return LinReg::GetVoltage();  
}
float PSU::getCurrent(){
    return LinReg::GetCurrent(); 
}
float PSU::getPower(){
    return (getCurrent()/1000)*getVoltage();
}
float PSU::getTargetVoltage(){
    return targetVoltage;
}
float PSU::getTargetCurrent(){
    return targetCurrent;
}

void PSU::SetVoltage(float voltage_V){
    targetVoltage = voltage_V; 
}
void PSU::SetCurrent(float current_mA){
    targetCurrent = current_mA; 
} 

void PSU::Enable(){
    enabled = true; 

}
 void PSU::Disable(){
    enabled = false; 
 }