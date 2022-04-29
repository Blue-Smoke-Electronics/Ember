#ifndef COMANDLINEINTERFACE_H
#define COMANDLINEINTEFACE_H

#include <string>

class ComandLineInterface{
    private:
        const int update_freq_us = 1000*10; 
        int update_timer; 
        std::string rx_data; 
        int rx_data_ptr; 

        void desifre_command(std::string *command);

    public: 
        ComandLineInterface();
        void Update();

};

#endif