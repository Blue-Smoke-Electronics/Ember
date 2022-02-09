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
#include "project.h"
#include "uart.h"
#include <stdio.h>
#include "display.h"
#include "GUI.h"
#include "BigFont.h"
#include <stdlib.h>
#include "psu.h"
#include "eeprom.h"


// ascii 
#define ASCII_ENTER 13
#define ASCII_TAB 9
#define ASCII_SPACE 32
#define ASCII_BACKSPACE 127

volatile char rxdata[100];
volatile int read_ptr;


// enabeling printf
int _write(int file, char *ptr, int len)
{
    int i;
    file = file;
    for (i = 0; i < len; i++)
    {
        UART_PutChar(*ptr++);
    }
    return (len);
}
asm (".global _printf_float");



void Debug_uart_init(){
    read_ptr =0; 
    UART_Start();
    isr_rx_StartEx(Uart_rx_interrupt_handler);
    
}

void Usb_uart_init(){
    read_ptr =0; 
    USBUART_Start(0,USBUART_5V_OPERATION);
    
    
    
}


volatile char rxdata[100];
volatile int read_ptr =0;
CY_ISR(Uart_rx_interrupt_handler){
    
    char newchar = UART_ReadRxData();
    
    // echo every caracther
    printf("%c",newchar);
    fflush(stdout); // printf only fluchs on newline
    Display_fill_square(10,10,8*10,8*10,COLORRGB_WHITE);
    GUI_draw_char(newchar,10,10,COLORRGB_RED,BigFont);
   
    // echo resived line
    rxdata[read_ptr] = newchar;
    if (newchar == ASCII_ENTER) {
        rxdata[read_ptr] = '\0';
        printf("\n\recho: %s\r\n\n",rxdata);
        read_ptr =0; 
    }else {
        read_ptr +=1; 
    }
}

uint8 data [100]; 
uint8 comand[63]; 
int comand_index =0; 
 
void Usb_uartprocess_resived_data(){
    
    if (0u != USBUART_IsConfigurationChanged())
    {
        /* Initialize IN endpoints when device is configured. */
        if (0u != USBUART_GetConfiguration())
        {
            /* Enumeration is done, enable OUT endpoint to receive data 
             * from host. */
            USBUART_CDC_Init();
        }
    }
    /* Service USB CDC when device is configured. */
    
    if (0u != USBUART_GetConfiguration()){
        while(USBUART_DataIsReady()){
            int cnt = USBUART_GetAll(data);
            
            USBUART_PutData(data,cnt);
            for ( int i  = 0 ; i < cnt; i++){
                if (data[i] == '\n'||data[i] == '\r'){
                    desifre_comand(comand_index);
                    comand_index =0; 
                    continue;
                }
                if((data[i] < 65 || data[i] > 90)/*A-Z*/ &&(data[i] < 97 || data[i] > 122)/*a-z*/ && (data[i]< 48 || data[i]>57)/*0-9*/ && (data[i] != 46)/*.*/&& (data[i] != 32)/* */){
                    continue;   
                }
                comand[comand_index] = data[i]; 
                comand_index++; 
                if(comand_index > 61){
                    comand_index = 61;
                }
            }
            
        }
    }
    
 
    
}

void desifre_comand(int length){
    bool isEqual; 
    comand[length] ='\n';
    
    char * c = "VSET";
    isEqual = true;
    for (uint i = 0 ; i < strlen(c);i++){
        if (toupper(comand[i]) != toupper(c[i])){
            isEqual = false; 
        }
    }
    
    if (isEqual){
        float voltValue = atof((char*)comand+strlen(c));
        if (voltValue > 12){
            voltValue = 12.0f;    
        }
        printf("setting max Volt to %f \n\r",voltValue);
        psu_set_voltage(voltValue*1000);
        GUI_Draw_voltage_limit(voltValue*1000,false,0);
        eeprom_write_max_voltage_value(voltValue*1000);
        usbuart_print("\r\nseting max Voltage \n\r");
        return;
    }
    
    
    c = "ISET";
    isEqual = true;
    for (uint i = 0 ; i < strlen(c);i++){
        if (toupper(comand[i]) != toupper(c[i])){
            isEqual = false; 
        }
    }
    
    if (isEqual){
        float curretnValue = atof((char*)comand+strlen(c));
        if (curretnValue > 500){
            curretnValue = 500.0f;   
        }
        printf("setting max Current to %f \n\r",curretnValue);
        psu_set_current(curretnValue);
        GUI_Draw_current_limit(curretnValue,false,0);
        eeprom_write_max_current_value(curretnValue);
        usbuart_print("\r\nseting max Current \n\r");
        return;
    }
    
    c = "ON";
    isEqual = true;
    for (uint i = 0 ; i < strlen(c);i++){
        if (toupper(comand[i]) != toupper(c[i])){
            isEqual = false; 
        }
    }
    
    if (isEqual){
        printf("turning on Powersupply \n\r");
        psu_start();
        GUI_Draw_on_off(true);
        usbuart_print("\r\nTurning on Powersupply \n\r");
        return;
    }
    
        c = "OFF";
    isEqual = true;
    for (uint i = 0 ; i < strlen(c);i++){
        if (toupper(comand[i]) != toupper(c[i])){
            isEqual = false; 
        }
    }
    
    if (isEqual){
        printf("turning off Powersupply \n\r");
        psu_stop();
        GUI_Draw_on_off(false);
        usbuart_print("\r\nTurning off Powersupply \n\r");
        return;
    }
    
    
    usbuart_print("\r\ninvalid comand: "); 
    usbuart_print_dynamic(comand,length); 
    usbuart_print("\r\n");
    
}

void usbuart_print(const char * str ){
    if (0u != USBUART_IsConfigurationChanged())
    {
        /* Initialize IN endpoints when device is configured. */
        if (0u != USBUART_GetConfiguration())
        {
            /* Enumeration is done, enable OUT endpoint to receive data 
             * from host. */
            USBUART_CDC_Init();
        }
    }

    /* Service USB CDC when device is configured. */
    if (0u != USBUART_GetConfiguration())
    {
        while (0u == USBUART_CDCIsReady())
        {
            
        }
        USBUART_PutString(str);
    }     
}

void usbuart_print_dynamic(uint8 * str, int length ){
    if (0u != USBUART_IsConfigurationChanged())
    {
        /* Initialize IN endpoints when device is configured. */
        if (0u != USBUART_GetConfiguration())
        {
            /* Enumeration is done, enable OUT endpoint to receive data 
             * from host. */
            USBUART_CDC_Init();
        }
    }

    /* Service USB CDC when device is configured. */
    if (0u != USBUART_GetConfiguration())
    {
        while (0u == USBUART_CDCIsReady())
        {
            
        }
        USBUART_PutData(str,length);
    }     
}


/* [] END OF FILE */
