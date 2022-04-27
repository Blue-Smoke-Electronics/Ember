#ifndef GUD_H
#define GUD_H

#include "Display.h"
#include "Font.h"
#include"Sprite.h"

class GUI{
    private:
        Display display; 
        Font big_font; 
        Font Smal_font;
        Sprite logo; 


        //void Draw_char(char c, int xpos, int ypos, Font font); 

    public:
        GUI(); 
        void show_boot_screen();

};



#endif
