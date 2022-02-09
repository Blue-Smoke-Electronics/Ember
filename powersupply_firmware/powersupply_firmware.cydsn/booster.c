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

#include "booster.h"
#include <stdio.h>

// booster datasheet: https://www.ti.com/lit/ds/symlink/tps55288.pdf?ts=1603712542873&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FTPS55288

void Booster_init(){
    
    
    // starting up i2c module on cypress 
    I2C_BOOSTER_Start();
    
    // starting up booster chip
    Booster_enable_Write(1);
    
    // setting output voltage to 0 V 
    Booster_set_voltage(0);
    
    // disable booster current limit
    i2c_write_reg(2,0b11111111);
    
    // enable output 
    i2c_write_reg(6,0b10100011);
    
    
    // slow down voltage change and over current protection response 
    i2c_write_reg(3,0b00110000);
    
    
    i2c_write_reg(4,0b00000010);
    i2c_write_reg(5,0b11100000);
    
    isr_booster_fault_StartEx(booster_fault_interrupt_handler);
    
    
}

uint8_t i2c_read_reg(uint8_t reg){
    uint8_t  wrData= reg; 
    I2C_BOOSTER_MasterWriteBuf(0x74, &wrData, 1, I2C_BOOSTER_MODE_COMPLETE_XFER);
    for(;;) {    
        if(0u != (I2C_BOOSTER_MasterStatus() & I2C_BOOSTER_MSTAT_WR_CMPLT))    {
            /* Transfer complete. Check Master status to make sure that transfer           completed without errors. */ 
            I2C_BOOSTER_MasterClearStatus();
            break;    
        } 
    }
    uint8_t rdData; 
    I2C_BOOSTER_MasterReadBuf(0x74,&rdData,1,I2C_BOOSTER_MODE_COMPLETE_XFER);
    for(;;) {
        if(0u != (I2C_BOOSTER_MasterStatus() & I2C_BOOSTER_MSTAT_RD_CMPLT))    {
            /* Transfer complete. Check Master status to make sure that transfer           completed without errors. */ 
            I2C_BOOSTER_MasterClearStatus();
            break;    
        } 
    }
    return rdData; 
}

void i2c_write_reg(uint8_t reg,uint8_t data){
    
    uint8_t  wrData[2]= {reg,data}; 
    I2C_BOOSTER_MasterWriteBuf(0x74, wrData, 2, I2C_BOOSTER_MODE_COMPLETE_XFER);
    for(;;) {    
        if(0u != (I2C_BOOSTER_MasterStatus() & I2C_BOOSTER_MSTAT_WR_CMPLT))    {
            /* Transfer complete. Check Master status to make sure that transfer           completed without errors. */ 
            I2C_BOOSTER_MasterClearStatus();
            break;    
        } 
    }
}

void Booster_set_voltage(uint16_t mV){
    
    // remove when fixed opamp error 
    float devider_voltage = mV*0.0752;
    float reg_value = (devider_voltage-45)/1.129;
    if(reg_value < 0){
     reg_value =0;    
    }
    
    // set internal feedback ratio to 15mV per resolution (max 15V) , and use internal ref 
    i2c_write_reg(4,0b00000010);
    
    
    // set vref
    uint16_t reg = reg_value; 
    //printf("vref: %d\r\n",vref);
    if ( reg > 0x3C0){
        printf("booster vref is out of range, vref = %x\r\n", reg);
        reg = 0x3C0;
    }
    
    i2c_write_reg(0,reg);// lsb 
    i2c_write_reg(1,reg>>8);    // msb 
    
    
}
void Booster_set_current_limit(uint16_t mA){
    //enable and set current limit
    if (mA >= 650){
        printf("booster max Current reached: %d ", mA);
        mA = 650; 
    }
     
    printf("current limit reg sendt : %d\r\n", (mA)/8|(1<<7));
    //i2c_write_reg(2,(mA)/8|(1<<7));
    i2c_write_reg(2,0b11);
    
}


volatile int fault; 

uint last_buster_status =0; 
void Booster_print_status(){
    if(fault ){
        printf("booster fault PIN ERROR\r\n");
        fault =0;
    }
    
    uint8_t status_reg = i2c_read_reg(7);
   // printf("status reg: %d\r\n",status_reg);
    if (last_buster_status != status_reg){
        last_buster_status = status_reg;
       
        if(status_reg & (1<<7)){
            printf("Booster short circuit error\r\n");
        }
        if(status_reg & (1<<6)){
            printf("Booster over current error\r\n");
        }
        if(status_reg & (1<<5)){
            printf("Booster overvoltage error\r\n");
        }
        if((status_reg & 0b11) == 0){
            printf("boostermode = boost\r\n");
        }
         if((status_reg & 0b11) == 1){
            printf("boostermode = buck\r\n");
        }
         if((status_reg & 0b11) == 2){
            printf("boostermode = boost/buck\r\n");
        }
         if((status_reg & 0b11) == 3){
            printf("boostermode = reserved\r\n");
        }
    }
}


CY_ISR(booster_fault_interrupt_handler){
    fault =1; 
}
