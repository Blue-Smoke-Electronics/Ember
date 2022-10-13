#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "Display.h"
#include "Flash.h"
#include "Pcb.h"
#include "Onoff.h"
#include "Heartbeat.h"
#include "CommandlineInterface.h"
#include "GUI.h"
#include "PSU.h"
#include "Analog.h"
#include "Knobs.h"
#include "Battery.h"

int main() {
    stdio_init_all();   // allowing printf debung in onoff init

    // keap device powered on, need to be the frist thin that happens 
    Onoff::Init();

    Heartbeat heartbeat = Heartbeat();
    Analog::Init(); 
    PSU::Init(); 
    CommandLineInterface::Init();
    GUI::Init();
    Battery::Init();
    Knobs::Init();

    int i =0; 
    while (true) {
        heartbeat.Update();
        PSU::Update(); 
        CommandLineInterface::Update();
        GUI::Update();
        Knobs::Update(); 
        Battery::Update();    
    }
}
