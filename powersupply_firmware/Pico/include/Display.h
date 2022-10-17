/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

/* [] END OF FILE */
#ifndef DISPLAY_H
#define DISPLAY_H

#include "stdio.h"
#include "pico/stdlib.h"
#include "Sprite.h"
#include "hardware/dma.h"
#include "Font.h"

#include <string>
#include <queue>

#define DISPLAY_COMMAND 0
#define DISPLAY_DATA 1

class SpiData{
    private:
	public:
        bool isCommand;
        bool staticInput;
        bool useAddress;
        const uint8_t *  data_ptr ;
        uint8_t data;
        int size;
        SpiData();
        SpiData(bool isCommand, uint8_t data);
        SpiData(uint8_t data,int size);
        SpiData(const uint8_t * data,int size);
};

class Display{
    private:
        static uint backlight_pwm_slice_num;

        static uint8_t static_byte;
        static int dma_channal;
        static dma_channel_config dma_channal_config;

        static SpiData spiQueue [];
        static const uint16_t spiQueue_size;
        static uint16_t spiQueue_read_pos;
        static uint16_t spiQueue_cnt;

        static void Send_packet(uint8_t command, uint8_t * data);
        static void WriteComm(uint8_t data);
        static void WriteData(uint8_t data);
        static void Draw_image(uint16_t x,uint16_t y,uint16_t width, uint16_t heigth, const uint8_t * img);
        static void Push_to_spiQueue(SpiData spiData);

        static bool display_queue_overflow;
    public:
        static const int width;
        static const int height;
        static void Init();
        static void Update();
        static bool Isready();
        static void Draw_pixel(int xpos,int ypos,uint8_t red, uint8_t green, uint8_t blue);
        static void Draw_sprite(int xpos, int ypos, Sprite sprite);
        static void Draw_char(int xpos, int ypos, Font font, char c);
        static void Draw_string(int xpos, int ypos, Font font, std::string s);
        static void Clear_square(int x,int y,int width, int heigth);
        static void Clear_all();
        static void Set_backlight(int percent);
};

#endif
