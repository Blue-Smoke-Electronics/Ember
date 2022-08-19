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
    //logo = Flash::logo;
    
    state = State::booting; 
    newState = true; 

    voltageScaler =1.0f; 
    currentScaler =1.0f; 


}
void GUI::Update(){
    Display::Update();

    if(time_us_32() - update_timer > update_freq_us )
    {
        update_timer = time_us_32(); 
        std::ostringstream streamObj;

         

        switch (state)
        {
        case State::booting:
            if (newState){
                newState = false; 
                boot_start_time = time_us_32();
                //Display::Clear_all();
                Display::Draw_char(10,10,Flash::smalFont,'a');

                
                Display::Clear_all();
                Display::Draw_sprite(50,60,Flash::logo);   
            }
            /*
            if(time_us_32()-boot_start_time > boot_time_us ){
                state = State::running; 
                newState = true; 
            }
            if(!Onoff::IsOn){
                state = State::charging;
                newState = true; 
            }
            */

            break;
        case State::running:
            if (newState){
                newState = false; 
                Display::Clear_all();
            }
            streamObj <<std::fixed<< std::setprecision(2);
            streamObj << "V Set:" <<  std::setw(5)<< PSU::getTargetVoltage();
            Display::Draw_string(200,15,Flash::smalFont,streamObj.str()); 
            if(voltageScaler <= 0.01f){
                Display::Draw_sprite(200+Flash::smalFont.char_widht*10,38,Flash::selectedMarker);
            }
            else if(voltageScaler <= 0.1f){
                Display::Draw_sprite(200+Flash::smalFont.char_widht*9,38,Flash::selectedMarker);
            }
            else if(voltageScaler <= 1.0f){
                Display::Draw_sprite(200+Flash::smalFont.char_widht*7,38,Flash::selectedMarker);
            }
            

            streamObj.str("");
            streamObj.clear();
            streamObj<< std::setw(5) << PSU::getVoltage()<< "V";
            Display::Draw_string(240,60,Flash::bigFont,streamObj.str());


            streamObj.str("");
            streamObj.clear();
            streamObj << std::setprecision(0);
            streamObj <<"I Set: "<< std::setw(4)<< PSU::getTargetCurrent() << " mA";
            Display::Draw_string(200,180,Flash::smalFont,streamObj.str());
            if(currentScaler <= 1.0f){
                Display::Draw_sprite(200+Flash::smalFont.char_widht*10,203,Flash::selectedMarker);
            }
            else if(currentScaler <= 10.0f){
                Display::Draw_sprite(200+Flash::smalFont.char_widht*9,203,Flash::selectedMarker);
            }
            else if(currentScaler <= 100.0f){
                Display::Draw_sprite(200+Flash::smalFont.char_widht*8,203,Flash::selectedMarker);
            }
            

            streamObj.str("");
            streamObj.clear();
            streamObj << std::setw(4)<<PSU::getCurrent()<< "mA";
            Display::Draw_string(240,210,Flash::bigFont,streamObj.str());
            
                       
             streamObj.str("");
            streamObj.clear();
            streamObj << std::setprecision(1);
            streamObj <<"Power: "<< PSU::getPower() << "W";
            Display::Draw_string(200,125,Flash::smalFont,streamObj.str());

            
            if(PSU::IsEnabled()){
                Display::Draw_sprite(20,Display::height-Flash::outputOnSymbol.height-20 ,Flash::outputOnSymbol);
            }
            else{
                Display::Draw_sprite(20,Display::height-Flash::outputOnSymbol.height-20,Flash::outputOffSymbol);
            }
            

            streamObj.str("");
            streamObj.clear();
            streamObj << std::setprecision(0)<< std::setw(3);
            streamObj <<Battery::GetBatteryProcentage() << "%";
            Display::Draw_string(50,10,Flash::smalFont,streamObj.str());


            streamObj.str("");
            streamObj.clear();
            streamObj << std::setprecision(0);
            streamObj << std::setw(2)<<(int)Battery::GetBatteryLife_s()/3600 << "h" << std::setw(2)<< ((int)Battery::GetBatteryLife_s()%3600 )/60 <<"m";
            Display::Draw_string(50,20+Flash::smalFont.char_height,Flash::smalFont,streamObj.str());

            if(Battery::GetBatteryProcentage() > 75){
                Display::Draw_sprite(10,20,Flash::batterySymbolHigh);
            }
            else if(Battery::GetBatteryProcentage() > 50){
                Display::Draw_sprite(10,20,Flash::batterySymbolMedHigh);
            }
            else if(Battery::GetBatteryProcentage() > 25){
                Display::Draw_sprite(10,20,Flash::batterySymbolMedLow);
            }
            else if(Battery::GetBatteryProcentage() > 0){
                Display::Draw_sprite(10,20,Flash::batterySymbolLow);
            }
            else{
                Display::Draw_sprite(10,20,Flash::batterySymbolEmpty);
            }

            if(Battery::IsCharging()){
                Display::Draw_sprite(13,30,Flash::batteryChargingSymbol);
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
                Display::Draw_sprite(220,150,Flash::batteryChargingSymbol);
            }

            streamObj.str("");
            streamObj.clear();
            streamObj << std::setprecision(0)<< std::setw(3);
            streamObj <<Battery::GetBatteryProcentage() << "%";
            Display::Draw_string(180,80,Flash::smalFont,streamObj.str());


            streamObj.str("");
            streamObj.clear();
            streamObj << std::setprecision(0);
            streamObj << std::setw(2)<<(int)Battery::GetBatteryLife_s()/3600 << "h" << std::setw(2)<< ((int)Battery::GetBatteryLife_s()%3600 )/60 <<"m";
            Display::Draw_string(180,80+Flash::smalFont.char_height,Flash::smalFont,streamObj.str());

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



