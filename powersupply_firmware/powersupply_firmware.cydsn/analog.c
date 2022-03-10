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
    int mA = (mV-38)*0.73; 
    if (mA < 0){
        mA =0; 
    }
    return mA;
}


int analog_get_battery_voltage(){
    ADC_SAR_StartConvert();
    ADC_SAR_IsEndConversion(ADC_SAR_WAIT_FOR_RESULT);
    return ADC_SAR_CountsTo_mVolts(ADC_SAR_GetResult16(2))*2;
}
