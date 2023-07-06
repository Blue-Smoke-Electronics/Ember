#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/watchdog.h"
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
#include "Overheat.h"
#include "Powersaver.h"

int main() {
    stdio_init_all(); // allowing printf debug in onoff init
    
    watchdog_enable(1000,true);
    // wait for device to power off if wachdog triggered restart. 
    while ( watchdog_enable_caused_reboot());

    


    
    // keep device powered on, need to be the first thing that happens
    Onoff::Init();

    Heartbeat heartbeat = Heartbeat();
    Analog::Init();
    PSU::Init();
    CommandLineInterface::Init();
    GUI::Init();
    Battery::Init();
    Knobs::Init();
    Overheat::Init();
    Powersaver::Init();

    while (true) {
        heartbeat.Update();
        PSU::Update(); 
        CommandLineInterface::Update();
        GUI::Update();
        Knobs::Update();
        Battery::Update();
        Overheat::Update();
        Onoff::Update();
        Powersaver::Update();

        watchdog_update();
        
    }
}
