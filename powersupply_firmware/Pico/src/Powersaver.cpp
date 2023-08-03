#include "Powersaver.h"
#include <stdio.h>
#include "Display.h"
#include "Onoff.h"

float Powersaver::idle_timer ; 
uint32_t Powersaver::update_timer;

void Powersaver::Init(){
    Reset_idle_timer();
    update_timer = time_us_32();
}
void Powersaver::Update(){
    if(time_us_32() - update_timer < 1000000){
        return;
    }

    idle_timer += (time_us_32() - update_timer)/1000000.0f;
    update_timer = time_us_32(); 

    // dim display
    if(Onoff::IsOn){
        if(idle_timer > Powersaver::display_off_time){
            Display::Set_backlight(5);
        }
        else {
            Display::Set_backlight(100);
        }
    }

    // automatic power off
    if(idle_timer > Powersaver::power_off_time){
        Onoff::Turn_off_device();
    }
}
void Powersaver::Reset_idle_timer(){
    idle_timer =0; 
}

