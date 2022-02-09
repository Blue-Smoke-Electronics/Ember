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
#include "knobs.h"
#include <stdio.h>
#include "statemachine.h"

volatile int sw_on_off_press_cnt; 
volatile int sw_voltage_press_cnt; 
volatile int sw_current_press_cnt; 
volatile int sw_encoder_press_cnt; 
volatile int encoder_step_cnt; 

void Knobs_init(){
    isr_sw_encoder_StartEx(sw_encoder_interrupt_handler);
    isr_encoder_StartEx(encoder_interrupt_handler);
    isr_sw_voltage_StartEx(sw_voltage_interrupt_handler);
    isr_sw_current_StartEx(sw_current_interrupt_handler);
    isr_sw_on_off_StartEx(sw_on_off_interrupt_handler);
   
    sw_on_off_press_cnt = 0; 
    sw_voltage_press_cnt = 0; 
    sw_current_press_cnt = 0; 
    sw_encoder_press_cnt = 0; 
    encoder_step_cnt = 0; 
}




int knobs_get_on_off_switch_presses(){
    int cnt = sw_on_off_press_cnt;
    sw_on_off_press_cnt = 0; 
    return cnt; 
}

int knobs_get_voltage_switch_presses(){
    int cnt = sw_voltage_press_cnt; 
    sw_voltage_press_cnt =0; 
    return cnt; 
}

int knobs_get_current_switch_presses(){
    int cnt = sw_current_press_cnt; 
    sw_current_press_cnt =0; 
    return cnt; 
}

int knobs_get_encoder_switch_presses(){
    int cnt = sw_encoder_press_cnt; 
    sw_encoder_press_cnt =0; 
    return cnt; 
}

int knobs_get_encoder_steps(){
    // returns negativ values for left and positve values for right 
    int steps = encoder_step_cnt;
    encoder_step_cnt = 0; 
    return steps; 
}


CY_ISR(sw_encoder_interrupt_handler){
    sw_encoder_press_cnt++;
    printf("encoder pressed\r\n");
}
CY_ISR(encoder_interrupt_handler){
    if (ENCODER_A_Read()&&ENCODER_A_Read() == ENCODER_B_Read()){
        encoder_step_cnt--;
        //printf("A: Encoder Left\r\n");
    }
    else if (ENCODER_A_Read()&&ENCODER_A_Read() != ENCODER_B_Read()){
        encoder_step_cnt++;
        //printf("A: Encoder Right\r\n");
        
    }
}


CY_ISR(sw_voltage_interrupt_handler){
    sw_voltage_press_cnt++;
    printf("SW voltage pressed\r\n");
}
CY_ISR(sw_current_interrupt_handler){
    sw_current_press_cnt++;
    printf("SW current pressed\r\n");
}

CY_ISR(sw_on_off_interrupt_handler){
    sw_on_off_press_cnt++;
    printf("SW on/off pressed\r\n");
}