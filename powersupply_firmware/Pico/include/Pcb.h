#ifndef PCB_H
#define PCB_H

#include "pico/stdlib.h"
#include "hardware/spi.h"

class Pcb{
    private:

    public: 
        static const int uart_tx_pin = 0;  
        static const int uart_rx_pin = 1;
        static const int display_SCL_pin = 2; 
        static const int display_MISO_pin = 3; 
        static const int display_NCS_pin = 5; 
        static const int display_DC_pin = 6; 
        static const int dispaly_RST_pin = 7; 
        static const int current_limit_pwm_pin = 8; 
        static const int voltage_limit_pwm_pin=9; 
        static const int encoder_voltage_A_pin  =10; 
        static const int encoder_voltage_B_pin = 11; 
        static const int encoder_voltage_switch_pin = 12;        
        static const int encoder_current_A_pin = 13; 
        static const int encoder_current_B_pin = 14; 
        static const int encoder_current_switch_pin = 15; 
        static const int on_off_switch_pin = 16; 
        static const int on_off_latch_pin = 17; 
        static const int ouput_on_off_switch_pin  =18; 
        static const int ouput_on_off_led_pin = 19; 
        static const int booster_pwm_pin = 20; 
        static const int display_BACKLIGHT_pin = 22; 
        static const int usb_connected_pin = 24; 
        static const int debbug_led_pin = 25; 
        static const int output_voltage_sens_pin = 26; 
        static const int battery_voltage_sens_pin = 27; 
        static const int output_current_sens_pin = 28; 
        static const int booster_voltage_sens_pin = 29;


        static const int output_voltage_sens_adc_channal = 0; 
        static const int battery_voltage_sens_adc_channal= 1; 
        static const int output_current_sens_adc_channal = 2; 
        static const int booster_voltage_sens_adc_channal = 3; 
        static const int temprature_adc_channal = 4;

        static const int  flash_size_bytes = PICO_FLASH_SIZE_BYTES;//(16*1024*1024);  // need to find a way to update SDK before this can be increesed
};

#endif
