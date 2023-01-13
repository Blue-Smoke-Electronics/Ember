#include "Overheat.h"

#include "Analog.h"
#include "PSU.h"
#include "GUI.h"



uint32_t Overheat::update_timer;
float Overheat::maxTemp;
int32_t Overheat::hysteresisTimer;

void Overheat::Init(){
    update_timer =0;
    maxTemp = 80; 
    hysteresisTimer =0; 
}

void Overheat::Update(){
    if(time_us_32() - update_timer > update_freq_us) {
        hysteresisTimer -= time_us_32() - update_timer; 
        if (hysteresisTimer < 0){
            hysteresisTimer = 0;
        }
        
        update_timer = time_us_32(); 
        
        if(IsOverheated()){
            PSU::Disable();
            printf("Overheat!! %f\r\n",Analog::GetTemp() );
        }
        
       
    }
}

bool Overheat::IsOverheated(){
    if (Analog::GetTemp() > maxTemp){
        hysteresisTimer = 20*1000*1000;
    }
    return hysteresisTimer > 0;
}
