#include "GUI.h"
#include "Display.h"
#include "Flash.h"

#include <string>
#include <sstream>
#include <iomanip>


GUI::GUI(){
    Display::Init();

    update_timer = 0; 
    //logo = Flash::logo;
    
    state = State::booting; 
    newState = true; 


}
int i =0;
void GUI::Update(){
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
            }

            
            streamObj << std::fixed;
            streamObj << std::setprecision(2);
            streamObj << "V Set: " << 15.0f;
            Display::Draw_string(200,15,Flash::smalFont,streamObj.str());

            streamObj.str("");
            streamObj.clear();
            streamObj << 3.3f << "V";
            Display::Draw_string(250,60,Flash::bigFont,streamObj.str());


            streamObj.str("");
            streamObj.clear();
            streamObj << std::setprecision(0);
            streamObj <<"I Set: "<< 500.0f << " mA";
            Display::Draw_string(200,180,Flash::smalFont,streamObj.str());

            streamObj.str("");
            streamObj.clear();
            streamObj << i<< " mA";
            i++;
            Display::Draw_string(250,210,Flash::bigFont,streamObj.str());

                       
             streamObj.str("");
            streamObj.clear();
            streamObj << std::setprecision(1);
            streamObj <<"Power: "<< 5.2f << "W";
            Display::Draw_string(200,125,Flash::smalFont,streamObj.str());

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




