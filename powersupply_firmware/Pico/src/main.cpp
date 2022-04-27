#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"

#include "FreeRTOS.h"

#include "test.pio.h"
#include "Display.h"
#include "Flash.h"
#include "Pcb.h"
#include "Onoff.h"







int main() {
    // keap device powered on, need to be the frist thin that happens 
    Onoff onoff = Onoff(); 



    const uint led_pin = Pcb::debbug_led_pin;

    // Initialize LED pin
    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);
    gpio_put(led_pin,true);


    // Initialize chosen serial port
    stdio_init_all();



    float pio_freq = 2000; 

    PIO pio = pio0;
    uint sm = pio_claim_unused_sm(pio,true);
    uint offset = pio_add_program(pio,&test_program);

    float div = (float)clock_get_hz(clk_sys)/pio_freq;

    //test_program_init(pio, sm, offset, led_pin,div);
    //pio_sm_set_enabled(pio,sm,true);


   
   Flash flash; 

    /*
    
    flash.start_data_stream(flash.logo_address);
    for (int i=0; i< flash.logo_size;i++){
        uint8_t r =getchar();
        printf("%d\n",i);
        flash.stream_byte(r);

    }
    flash.stop_data_stream();

    for (int i=0; i< flash.logo_size/3;i++){
        printf("flash[i]: %d \r\n",flash.logo_address[i]);
    }
  */
  
  

    Display display = Display();

    Sprite logo = Sprite();
    logo.flash_address = (uint8_t*)flash.logo_address;
    logo.height = flash.logo_height;
    logo.width = flash.logo_width;
    logo.size = flash.logo_size; 

    display.Draw_sprite(0,0,logo);

    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_gpio_init(Pcb::booster_voltage_sens_pin);
    adc_gpio_init(Pcb::battery_voltage_sens_pin);
    adc_gpio_init(Pcb::output_voltage_sens_pin);
    adc_gpio_init(Pcb::output_current_sens_pin);

    // enable booster pwm 
    gpio_set_function(Pcb::booster_pwm_pin,GPIO_FUNC_PWM);
    uint booster_pwm_slice_num = pwm_gpio_to_slice_num(Pcb::booster_pwm_pin);
    pwm_set_wrap(booster_pwm_slice_num,1000); // 133kHz
    pwm_set_enabled(booster_pwm_slice_num,true);
    int booster_pwm_value = 0; 
    float booster_target_voltage = 10.0f; 

    // enable linreg pwm
    gpio_set_function(Pcb::voltage_limit_pwm_pin,GPIO_FUNC_PWM);
    uint voltage_limit_pwm_slice_num = pwm_gpio_to_slice_num(Pcb::voltage_limit_pwm_pin);
    pwm_set_wrap(voltage_limit_pwm_slice_num,10000); // 13.3kHz
    pwm_set_enabled(voltage_limit_pwm_slice_num,true);

    gpio_set_function(Pcb::current_limit_pwm_pin,GPIO_FUNC_PWM);
    uint current_limit_pwm_slice_num = pwm_gpio_to_slice_num(Pcb::current_limit_pwm_pin);
    pwm_set_wrap(current_limit_pwm_slice_num,10000); // 13.3kHz
    pwm_set_enabled(current_limit_pwm_slice_num,true);


    // set linreg pwm
    float linreg_output_voltage = 2.0f; 
    
    float current_limit = .5f;  
    pwm_set_chan_level(voltage_limit_pwm_slice_num,pwm_gpio_to_channel(Pcb::voltage_limit_pwm_pin),(linreg_output_voltage/4.7037)*(10000/3.3f));
    pwm_set_chan_level(current_limit_pwm_slice_num,pwm_gpio_to_channel(Pcb::current_limit_pwm_pin),(current_limit*2.5f)*(10000/3.3f));



    while (true) {
        

        
        // Blink LED
        printf("Blinking!\r\n");

        adc_select_input(Pcb::temprature_adc_channal);
        uint16_t raw_temp_data = adc_read();
        float voltage = raw_temp_data * 3.3f/(1<<12);
        float temp = 27-(voltage-0.706)/0.001721;

        adc_select_input(Pcb::output_current_sens_adc_channal);
        uint16_t raw_output_current = adc_read();
        float output_current = raw_output_current * 3.3f/(1<<12)/2.5f;
        
        adc_select_input(Pcb::output_voltage_sens_adc_channal);
        uint16_t raw_output_voltage = adc_read();
        float output_voltage = raw_output_voltage * 3.3f/(1<<12)*11.0f;
        
        
        adc_select_input(Pcb::booster_voltage_sens_adc_channal);
        uint16_t booster_voltage_raw = adc_read(); 
        float booster_voltage =   (float)booster_voltage_raw * 3.3f/(1<<12) *11; // reading to high values ???  

        
        printf("temp: %f , booster voltage: %f output current: %f output voltage: %f \r\n", temp,booster_voltage ,output_current,output_voltage); 

        if (booster_voltage < booster_target_voltage && booster_pwm_value < 900 ){
            //booster_pwm_value ++; 
        }
        if (booster_voltage > booster_target_voltage && booster_pwm_value > 0){
            booster_pwm_value --; 
        }
        pwm_set_chan_level(booster_pwm_slice_num,pwm_gpio_to_channel(Pcb::booster_pwm_pin),booster_pwm_value);

     
    }
}