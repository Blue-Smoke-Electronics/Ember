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
    if (!Display::Isready()){
        return;
    }

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
                    Display::Draw_sprite(0,0,Flash::bootscreen);   
                }
                
                if(time_us_32()-boot_start_time > boot_time_us ){
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
                }
            
                streamObj <<std::fixed<< std::setprecision(2);
                streamObj << "" <<  std::setw(5)<< PSU::getTargetVoltage()<<"V";
                Display::Draw_string(Display::width-6*Flash::smallFont.char_widht-5,0,Flash::smallFont,streamObj.str()); 
                if(voltageScaler <= 0.01f){
                    Display::Draw_sprite(Display::width-Flash::smallFont.char_widht*2-5,Flash::smallFont.char_height, Flash::selectedMarker);
                    Display::Clear_square(Display::width-Flash::smallFont.char_widht*3-5,Flash::smallFont.char_height,Flash::selectedMarker.width,Flash::selectedMarker.height);
                    Display::Clear_square(Display::width-Flash::smallFont.char_widht*5-5,Flash::smallFont.char_height,Flash::selectedMarker.width,Flash::selectedMarker.height);
                }
                else if(voltageScaler <= 0.1f){
                    Display::Draw_sprite(Display::width-Flash::smallFont.char_widht*3-5,Flash::smallFont.char_height, Flash::selectedMarker);
                    Display::Clear_square(Display::width-Flash::smallFont.char_widht*2-5,Flash::smallFont.char_height,Flash::selectedMarker.width,Flash::selectedMarker.height);
                    Display::Clear_square(Display::width-Flash::smallFont.char_widht*5-5,Flash::smallFont.char_height,Flash::selectedMarker.width,Flash::selectedMarker.height);
                }
                else if(voltageScaler <= 1.0f){
                    Display::Draw_sprite(Display::width-Flash::smallFont.char_widht*5-5,Flash::smallFont.char_height, Flash::selectedMarker);
                    Display::Clear_square(Display::width-Flash::smallFont.char_widht*3-5,Flash::smallFont.char_height,Flash::selectedMarker.width,Flash::selectedMarker.height);
                    Display::Clear_square(Display::width-Flash::smallFont.char_widht*2-5,Flash::smallFont.char_height,Flash::selectedMarker.width,Flash::selectedMarker.height);
                }
                

                streamObj.str("");
                streamObj.clear();
                streamObj<< std::setw(5) << PSU::getVoltage()<< "";
                Display::Draw_string(Display::width-5*Flash::bigFont.char_widht-5,Flash::smallFont.char_height+10,Flash::bigFont,streamObj.str());
                
                streamObj.str("");
                streamObj.clear();
                streamObj << std::setprecision(0);
                streamObj <<""<< std::setw(4)<< PSU::getTargetCurrent() << "mA";
                Display::Draw_string(Display::width-6*Flash::smallFont.char_widht-5,Display::height-Flash::bigFont.char_height-Flash::smallFont.char_height,Flash::smallFont,streamObj.str());
                if(currentScaler <= 1.0f){
                    Display::Draw_sprite(Display::width-Flash::smallFont.char_widht*3-5,Display::height-Flash::bigFont.char_height - Flash::selectedMarker.height, Flash::selectedMarker);
                }
                else if(currentScaler <= 10.0f){
                    Display::Draw_sprite(Display::width-Flash::smallFont.char_widht*4-5,Display::height-Flash::bigFont.char_height - Flash::selectedMarker.height, Flash::selectedMarker);
                }
                else if(currentScaler <= 100.0f){
                    Display::Draw_sprite(Display::width-Flash::smallFont.char_widht*5-5,Display::height-Flash::bigFont.char_height - Flash::selectedMarker.height, Flash::selectedMarker);
                }
                
                
                streamObj.str("");
                streamObj.clear();
                streamObj << std::setw(4)<<PSU::getCurrent()<< "";
                Display::Draw_string(Display::width-4*Flash::bigFont.char_widht-5,Display::height-Flash::bigFont.char_height,Flash::bigFont,streamObj.str());
                /*
                        
                streamObj.str("");
                streamObj.clear();
                streamObj << std::setprecision(1);
                streamObj <<"Power: "<< PSU::getPower() << "W";
                Display::Draw_string(200,125,Flash::smallFont,streamObj.str());
                */

                
                
                if(PSU::IsEnabled()){
                    Display::Draw_sprite(5,Display::height-Flash::outputOnSymbol.height-5 ,Flash::outputOnSymbol);
                }
                else{
                    Display::Draw_sprite(5,Display::height-Flash::outputOnSymbol.height-5,Flash::outputOffSymbol);
                }

                /*
                streamObj.str("");
                streamObj.clear();
                streamObj << std::setprecision(0)<< std::setw(3);
                streamObj <<Battery::GetBatteryProcentage() << "%";
                Display::Draw_string(50,10,Flash::smallFont,streamObj.str());
                */

                streamObj.str("");
                streamObj.clear();
                streamObj << std::setprecision(0);
                streamObj << std::setw(2)<<(int)Battery::GetBatteryLife_s()/3600 << "h";
                Display::Draw_string(0,5+Flash::batterySymbolLow.height,Flash::smallFont,streamObj.str());
                streamObj.str("");
                streamObj.clear();
                streamObj << std::setprecision(0);
                streamObj << std::setw(2)<< ((int)Battery::GetBatteryLife_s()%3600 )/60 <<"m";
                Display::Draw_string(0,5+Flash::batterySymbolLow.height + Flash::smallFont.char_height,Flash::smallFont,streamObj.str());
               
                if(Battery::GetBatteryProcentage() > 75){
                    Display::Draw_sprite(5,5,Flash::batterySymbolHigh);
                }
                else if(Battery::GetBatteryProcentage() > 50){
                    Display::Draw_sprite(5,5,Flash::batterySymbolMedHigh);
                }
                else if(Battery::GetBatteryProcentage() > 25){
                    Display::Draw_sprite(5,5,Flash::batterySymbolMedLow);
                }
                else if(Battery::GetBatteryProcentage() > 0){
                    Display::Draw_sprite(5,5,Flash::batterySymbolLow);
                }
                else{
                    Display::Draw_sprite(5,5,Flash::batterySymbolEmpty);
                }
                if(Battery::IsCharging()){
                    Display::Draw_sprite(8,15,Flash::chargingSymbol);
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
                    Display::Draw_sprite(68,30,Flash::chargingSymbol);
                }

                streamObj.str("");
                streamObj.clear();
                streamObj << std::setprecision(0)<< std::setw(3);
                streamObj <<Battery::GetBatteryProcentage() << "%";
                Display::Draw_string(60,65,Flash::smallFont,streamObj.str());

                streamObj.str("");
                streamObj.clear();
                streamObj << std::setprecision(0);
                streamObj << std::setw(2)<<(int)Battery::GetBatteryLife_s()/3600 << "h" << std::setw(2)<< ((int)Battery::GetBatteryLife_s()%3600 )/60 <<"m";
                Display::Draw_string(40,65+Flash::smallFont.char_height,Flash::smallFont,streamObj.str());

                if(Onoff::IsOn){
                    state = State::booting;
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
        if (voltageScaler< 0.01f){
            voltageScaler = 1.00f; 
        }
    }
}
 
void GUI::ChangeCurrentScaler(){
    if(Onoff::IsOn){
        currentScaler /= 10; 
        if (currentScaler< 1.0f){
            currentScaler = 100.0f; 
        }
    }
   
}

float GUI::GetVoltageScaler(){
    return voltageScaler; 
} 
float GUI::GetCurrentScaler(){
    return currentScaler; 
}



