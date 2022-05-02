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


int main() {
    // keap device powered on, need to be the frist thin that happens 
    Onoff onoff = Onoff(); 
    
    stdio_init_all();    

    Hartbeat hartbeat = Hartbeat(); 
    Analog::Init(); 
    PSU::Init(); 
    ComandLineInterface cli = ComandLineInterface();
    GUI gui = GUI(); 



    PSU::SetVoltage(3.0f);
    PSU::SetCurrent(150.0f);


    while (true) {
        

        hartbeat.Update(); 
    
        PSU::Update(); 
        cli.Update();
        gui.Update();
        Display::Update();
        


     
    }
}