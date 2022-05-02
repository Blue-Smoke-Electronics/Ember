#include "PSU.h"
#include "LinReg.h"
#include "Booster.h"

void PSU::Init(){
    LinReg::Init(); 
    Booster::Init(); 
}

void PSU::Update(){
    Booster::Update(); 
    //LinReg::Update(); 

    if(time_us_32() - update_timer > update_freq_us ){
        update_timer = time_us_32() ;
        
        Booster::SetVoltage(LinReg::GetVoltage()+2);
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

void PSU::SetVoltage(float voltage_V){
    LinReg::SetVoltage(voltage_V);
    Booster::SetVoltage(voltage_V+2);
}
void PSU::SetCurrent(float current_mA){
    LinReg::SetCurrent(current_mA);
} 