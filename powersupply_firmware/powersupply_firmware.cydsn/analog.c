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

#include "analog.h"
#include <project.h>


void analog_init(){
    ADC_SAR_Start();
}

int analog_get_voltage(){
    ADC_SAR_StartConvert();
    ADC_SAR_IsEndConversion(ADC_SAR_WAIT_FOR_RESULT);
    return ADC_SAR_CountsTo_mVolts(ADC_SAR_GetResult16(1))*10*1.097;
}

int analog_get_current(){
    ADC_SAR_StartConvert();
    ADC_SAR_IsEndConversion(ADC_SAR_WAIT_FOR_RESULT);
    int mV =ADC_SAR_CountsTo_mVolts(ADC_SAR_GetResult16(0)); 
    int mA = mV/2.5f; 
    return mA;
}
int analog_get_battery_current(){
    ADC_SAR_StartConvert();
    ADC_SAR_IsEndConversion(ADC_SAR_WAIT_FOR_RESULT);
    int mV =ADC_SAR_CountsTo_mVolts(ADC_SAR_GetResult16(2)); 
    int mA = mV/2.5f; 
    return mA;
}
int analog_get_usb_current(){
    ADC_SAR_StartConvert();
    ADC_SAR_IsEndConversion(ADC_SAR_WAIT_FOR_RESULT);
    int mV =ADC_SAR_CountsTo_mVolts(ADC_SAR_GetResult16(3)); 
    int mA = mV/2.5f; 
    return mA;
}

int analog_get_battery_voltage1(){
    ADC_SAR_StartConvert();
    ADC_SAR_IsEndConversion(ADC_SAR_WAIT_FOR_RESULT);
    return ADC_SAR_CountsTo_mVolts(ADC_SAR_GetResult16(4))*11;
}

int analog_get_battery_voltage2(){
    ADC_SAR_StartConvert();
    ADC_SAR_IsEndConversion(ADC_SAR_WAIT_FOR_RESULT);
    return ADC_SAR_CountsTo_mVolts(ADC_SAR_GetResult16(5)-ADC_SAR_GetResult16(4))*11;
}
int analog_get_battery_voltage3(){
    ADC_SAR_StartConvert();
    ADC_SAR_IsEndConversion(ADC_SAR_WAIT_FOR_RESULT);
    return ADC_SAR_CountsTo_mVolts(ADC_SAR_GetResult16(6)-ADC_SAR_GetResult16(5))*11;
}
int analog_get_battery_voltage4(){
        ADC_SAR_StartConvert();
    ADC_SAR_IsEndConversion(ADC_SAR_WAIT_FOR_RESULT);
    return ADC_SAR_CountsTo_mVolts(ADC_SAR_GetResult16(7)-ADC_SAR_GetResult16(6))*11;
}
int analog_get_battery_voltage5(){
        ADC_SAR_StartConvert();
    ADC_SAR_IsEndConversion(ADC_SAR_WAIT_FOR_RESULT);
    return ADC_SAR_CountsTo_mVolts(ADC_SAR_GetResult16(8)-ADC_SAR_GetResult16(7))*11;
}
int analog_get_battery_voltage6(){
        ADC_SAR_StartConvert();
    ADC_SAR_IsEndConversion(ADC_SAR_WAIT_FOR_RESULT);
    return ADC_SAR_CountsTo_mVolts(ADC_SAR_GetResult16(9))*21-ADC_SAR_CountsTo_mVolts(ADC_SAR_GetResult16(8))*11;
}

