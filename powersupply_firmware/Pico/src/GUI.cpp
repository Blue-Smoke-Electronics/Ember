#include "GUI.h"
#include "Display.h"
#include "Flash.h"

#include <string>
#include <sstream>
#include <iomanip>

#include "PSU.h"
#include "Battery.h"
#include "Onoff.h"

Font GUI::big_font;
Font GUI::Smal_font;
Sprite GUI::logo;

GUI::State GUI::state;
uint32_t GUI::boot_start_time;

bool GUI::newState;

uint32_t GUI::update_timer;

float GUI::voltageScaler;
float GUI::currentScaler;

void GUI::Init(){
    Display::Init();

    update_timer = 0; 

    state = State::booting; 
    newState = true; 

    voltageScaler =1.0f; 
    currentScaler =1.0f; 
}

void GUI::Update(){
    Display::Update();
    if (!Display::Isready())
        return;

    if(time_us_32() - update_timer > update_freq_us) {
        update_timer = time_us_32(); 
        std::ostringstream streamObj;

        switch (state) {
            case State::booting:
                if (newState){
                    newState = false; 
                    boot_start_time = time_us_32();

                    Display::Clear_all();
                    Display::Draw_pixel(100,100,0,0,0);
                    Display::Draw_sprite(Display::width/2-Flash::logo.width/2,Display::height/2-Flash::logo.height/2,Flash::logo);   
                }
                
                if(time_us_32() - boot_start_time > boot_time_us){
                    state = State::running; 
                    newState = true; 
                }

                if(!Onoff::IsOn){
                    state = State::charging;
                    newState = true;
                }

                break;
            case State::running:
                if (newState){
                    newState = false; 
                    Display::Clear_all();
                    Display::Draw_sprite(5, 5, Flash::batterySymbol);
                }

                // voltage level
                streamObj << std::fixed << std::setprecision(2);
                streamObj << "" <<  std::setw(5) << PSU::getTargetVoltage() <<"V";
                Display::Draw_string(Display::width - 6 * Flash::smallFont.char_widht - 5, 0, Flash::smallFont, streamObj.str());
                if(voltageScaler <= 0.01f) {
                    Display::Draw_sprite(Display::width - Flash::smallFont.char_widht * 2 - 5, Flash::smallFont.char_height, Flash::selectedMarker);
                    Display::Clear_square(Display::width - Flash::smallFont.char_widht * 3 - 5, Flash::smallFont.char_height, Flash::selectedMarker.width, Flash::selectedMarker.height);
                    Display::Clear_square(Display::width - Flash::smallFont.char_widht * 5 - 5, Flash::smallFont.char_height, Flash::selectedMarker.width, Flash::selectedMarker.height);
                }
                else if(voltageScaler <= 0.1f){
                    Display::Draw_sprite(Display::width - Flash::smallFont.char_widht * 3 - 5, Flash::smallFont.char_height, Flash::selectedMarker);
                    Display::Clear_square(Display::width - Flash::smallFont.char_widht* 2 - 5, Flash::smallFont.char_height, Flash::selectedMarker.width, Flash::selectedMarker.height);
                    Display::Clear_square(Display::width - Flash::smallFont.char_widht * 5 - 5, Flash::smallFont.char_height, Flash::selectedMarker.width, Flash::selectedMarker.height);
                }
                else if(voltageScaler <= 1.0f){
                    Display::Draw_sprite(Display::width - Flash::smallFont.char_widht * 5 - 5, Flash::smallFont.char_height, Flash::selectedMarker);
                    Display::Clear_square(Display::width - Flash::smallFont.char_widht * 3 - 5, Flash::smallFont.char_height, Flash::selectedMarker.width, Flash::selectedMarker.height);
                    Display::Clear_square(Display::width - Flash::smallFont.char_widht * 2 - 5, Flash::smallFont.char_height, Flash::selectedMarker.width, Flash::selectedMarker.height);
                }

                streamObj.str("");
                streamObj.clear();
                streamObj << std::setw(5) << PSU::getVoltage() << "";
                Display::Draw_string(Display::width - 5 * Flash::bigFont.char_widht - 5, Flash::smallFont.char_height + 10, Flash::bigFont, streamObj.str());
                
                // current level
                streamObj.str("");
                streamObj.clear();
                streamObj << std::setprecision(0);
                streamObj << "" << std::setw(4) << PSU::getTargetCurrent() << "mA";
                Display::Draw_string(Display::width - 6 * Flash::smallFont.char_widht - 5, Display::height - Flash::bigFont.char_height - Flash::smallFont.char_height , Flash::smallFont , streamObj.str());

                if(currentScaler <= 1.0f)
                    Display::Draw_sprite(Display::width - Flash::smallFont.char_widht * 3 - 5, Display::height-Flash::bigFont.char_height - Flash::selectedMarker.height, Flash::selectedMarker);
                else if(currentScaler <= 10.0f)
                    Display::Draw_sprite(Display::width - Flash::smallFont.char_widht * 4 - 5, Display::height - Flash::bigFont.char_height - Flash::selectedMarker.height, Flash::selectedMarker);
                else if(currentScaler <= 100.0f)
                    Display::Draw_sprite(Display::width - Flash::smallFont.char_widht * 5 - 5, Display::height - Flash::bigFont.char_height - Flash::selectedMarker.height, Flash::selectedMarker);

                streamObj.str("");
                streamObj.clear();
                streamObj << std::setw(4) << PSU::getCurrent() << "";
                Display::Draw_string(Display::width - 4 * Flash::bigFont.char_widht - 5, Display::height - Flash::bigFont.char_height, Flash::bigFont, streamObj.str());
                
                // output on/off
                if(PSU::IsEnabled())
                    Display::Draw_sprite(2, Display::height - Flash::outputOnSymbol.height, Flash::outputOnSymbol);
                else
                    Display::Draw_sprite(2, Display::height - Flash::outputOnSymbol.height, Flash::outputOffSymbol);

                // battery
                streamObj.str("");
                streamObj.clear();
                streamObj << std::setprecision(0);
                if((int)Battery::GetBatteryLife_s() / 3600 > 0)
                    streamObj << std::setw(2) << (int)Battery::GetBatteryLife_s() / 3600 << "h";
                else
                    streamObj << std::setw(2) << ((int)Battery::GetBatteryLife_s() % 3600 ) / 60 << "m";
                Display::Draw_string(5, 25, Flash::smallFont, streamObj.str());
                
                if(Battery::IsCharging()){
                    Display::Draw_sprite(45,5,Flash::chargingSymbol);
                }
                else{
                    Display::Clear_square(45,5,Flash::chargingSymbol.width,Flash::chargingSymbol.height);
                }

                for (int i =0; i < 5; i++){
                    if(Battery::GetBatteryProcentage() / 20 > i)
                        Display::Draw_sprite(8+i*6,8,Flash::batterySymbol_bar);
                    else
                        Display::Clear_square(8 + i * 6, 8, Flash::batterySymbol_bar.width, Flash::batterySymbol_bar.height);
                }

                if (Battery::GetBatteryProcentage() <= 0){
                    state = State::batteryEmpty;
                    newState = true;
                }

                if(!Onoff::IsOn){
                    state = State::charging;
                    newState = true;
                }

                break;
            case State::menu:
                /* code */
                break;
            case State::charging:
                if (newState){
                    newState = false;
                    Display::Clear_all();
                    Display::Draw_sprite(Display::width / 2 - Flash::chargingSymbol.width / 2, 30, Flash::chargingSymbol);
                }

                streamObj.str("");
                streamObj.clear();
                streamObj << std::setprecision(0) << std::setw(3);
                streamObj <<Battery::GetBatteryProcentage() << "%";
                Display::Draw_string(60,65,Flash::smallFont,streamObj.str());

                streamObj.str("");
                streamObj.clear();
                streamObj << std::setprecision(0);
                streamObj << std::setw(2) << (int)Battery::GetBatteryLife_s() / 3600 << "h" << std::setw(2)<< ((int)Battery::GetBatteryLife_s() % 3600) / 60 <<"m";
                Display::Draw_string(40, 65 + Flash::smallFont.char_height, Flash::smallFont, streamObj.str());

                if(Onoff::IsOn){
                    state = State::booting;
                    newState = true; 
                }

                break;
            case State::batteryEmpty:
                if (newState){
                    newState = false;

                    Display::Clear_all();
                    Display::Draw_sprite(60, 60, Flash::batterySymbol);
                }

                if(Battery::IsCharging()){
                    Onoff::Turn_off_device();
                    state = State::charging;
                    newState = true;
                }
                break;
            default:
                break;
        }
    }
}

void GUI::ChangeVoltageScaler(){
    if(Onoff::IsOn){
        voltageScaler /= 10;
        if (voltageScaler < 0.01f)
            voltageScaler = 1.00f; 
    }
}
 
void GUI::ChangeCurrentScaler(){
    if(Onoff::IsOn){
        currentScaler /= 10;
        if (currentScaler < 1.0f)
            currentScaler = 100.0f;
    }
}

float GUI::GetVoltageScaler(){
    return voltageScaler;
} 

float GUI::GetCurrentScaler(){
    return currentScaler;
}
