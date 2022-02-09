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
#ifndef MY_UART_H
#define MY_UART_H

#include "project.h"



void Debug_uart_init();
void Usb_uart_init();
void Usb_uartprocess_resived_data();
void desifre_comand(int length);
void usbuart_print(const char * string); 
void usbuart_print_dynamic(uint8 * str, int length ); 

CY_ISR(Uart_rx_interrupt_handler);
CY_ISR(Uart_tx_interrupt_handler);


#endif 

/* [] END OF FILE */