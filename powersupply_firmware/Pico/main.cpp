#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "test.pio.h"
#include "hardware/pwm.h"
#include "Display.h"
#include "hardware/adc.h"
#include "Flash.h"
//#include "logo.h"






int main() {

    const uint led_pin = 25;

    // Initialize LED pin
    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);

    // Initialize chosen serial port
    stdio_init_all();

    float pio_freq = 2000; 

    PIO pio = pio0;
    uint sm = pio_claim_unused_sm(pio,true);
    uint offset = pio_add_program(pio,&test_program);

    float div = (float)clock_get_hz(clk_sys)/pio_freq;

    //test_program_init(pio, sm, offset, led_pin,div);
    //pio_sm_set_enabled(pio,sm,true);

    gpio_set_function(led_pin,GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(led_pin);
    pwm_set_wrap(slice_num,1000);

    pwm_set_enabled(slice_num,true);

   
/*   Flash flash; 

    
    
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

    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);



    while (true) {
        
        pwm_set_chan_level(slice_num,pwm_gpio_to_channel(led_pin),100);
        // Blink LED
        printf("Blinking!\r\n");
        uint16_t raw_temp_data = adc_read();
        float voltage = raw_temp_data * 3.3f/(1<<12);
        float temp = 27-(voltage-0.706)/0.001721;
        printf("temp: %f\r\n", temp);
        //gpio_put(led_pin, true);
        sleep_ms(1000);
        pwm_set_chan_level(slice_num,pwm_gpio_to_channel(led_pin),800);
        //gpio_put(led_pin, false);
        sleep_ms(1000);
     
    }
}