#include "GUI.h"
#include "Display.h"
#include "Flash.h"

#include <string>
#include <sstream>
#include <iomanip>

#include "PSU.h"

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
                Display::Draw_sprite(0,0,Flash::bootscreen);   
            }

            if(time_us_32()-boot_start_time > boot_time_us ){
                state = State::running; 
                newState = true; 
            }

            break;
        case State::running:
            if (newState){
                newState = false; 
                Display::Clear_all();
                Display::Draw_sprite(100,20,Flash::logo);


            }
            streamObj << std::fixed;
            streamObj << std::setprecision(2);
            streamObj << "V Set: " << PSU::getTargetVoltage();
            Display::Draw_string(200,15,Flash::smalFont,streamObj.str());

            streamObj.str("");
            streamObj.clear();
            streamObj<< std::setw(5) << PSU::getVoltage()<< "V";
            Display::Draw_string(240,60,Flash::bigFont,streamObj.str());


            streamObj.str("");
            streamObj.clear();
            streamObj << std::setprecision(0);
            streamObj <<"I Set: "<< PSU::getTargetCurrent() << " mA";
            Display::Draw_string(200,180,Flash::smalFont,streamObj.str());

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
            
            //state = State::booting;
            //newState = true; 

            break;
        case State::menu:
            /* code */
            break;
        case State::charging:
            /* code */
            break;
        default:
            break;
        }

    }
}


void GUI::ChangeVoltageScaler(){

}
 
void GUI::ChangeCurrentScaler(){
    
}

float GUI::GetVoltageScaler(){
    return 1; 
} 
float GUI::GetCurrentScaler(){
    return 1; 
}



