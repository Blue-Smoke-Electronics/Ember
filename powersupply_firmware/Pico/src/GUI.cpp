#include "GUI.h"
#include "Display.h"
#include "Flash.h"



GUI::GUI(){
    Display::Init();

    update_timer = 0; 
    //logo = Flash::logo;
    
    state = State::booting; 
    newState = true; 


}

void GUI::Update(){
    if(time_us_32() - update_timer > update_freq_us )
    {
        update_timer = time_us_32(); 

        Display::Draw_sprite(100,100,Flash::logo);
        

        switch (state)
        {
        case State::booting:
            if (newState){
                newState = false; 
                boot_start_time = time_us_32();
                Show_boot_screen();  
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
            state = State::booting;
            newState = true; 

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


void GUI::Show_boot_screen(){
    
   Display::Draw_sprite(0,0,Flash::bootscreen); 


}

