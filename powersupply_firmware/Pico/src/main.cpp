#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"





#include "Display.h"
#include "Flash.h"
#include "Pcb.h"
#include "Onoff.h"
#include "Hartbeat.h"
#include "ComandlineInterface.h"
#include "GUI.h"
#include "PSU.h"
#include "Analog.h"
#include "Knobs.h"


int main() {
    // keap device powered on, need to be the frist thin that happens 
    Onoff::Init(); 
    
    stdio_init_all();    

    Hartbeat hartbeat = Hartbeat(); 
    Analog::Init(); 
    PSU::Init(); 
    ComandLineInterface::Init();
    GUI::Init();
    Knobs::Init();



    PSU::SetVoltage(5.0f);
    PSU::SetCurrent(150.0f);
    PSU::Enable();


    while (true) {
        

        hartbeat.Update(); 
    
        PSU::Update(); 
        ComandLineInterface::Update();
        GUI::Update();
        Display::Update();
        Knobs::Update(); 
        


     
    }
}