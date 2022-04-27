#include "GUI.h"
#include "Display.h"
#include "Flash.h"



GUI::GUI(){
    //display = Display(480,320); 
    //logo = Flash::logo;


}

void GUI::show_boot_screen(){
    
    display.Draw_sprite(0,0,logo); 
}