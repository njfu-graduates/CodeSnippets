/*
 * myADC.c
 *
 *  Created on: 2021��11��5��
 *      Author: 49945
 */

#include "./inc/myADC.h"

ADC_Handle hadc1;

void My_ADC_Init(ADC_Handle *adcHandle, uint_least8_t index)
{
    // One-time init of ADC driver
    ADC_init();
    // initialize optional ADC parameters
    ADC_Params params;
    ADC_Params_init(&params);
    params.isProtected = true;
    // Open ADC channels for usage
    *adcHandle = ADC_open(index, &params);
}

//330000-3.3v
//     0-  0v
uint32_t Get_Micro_Volts(ADC_Handle *adcHandle)
{
    uint16_t AdcRaw = 0;
    // Sample the analog output from the Thermocouple
    ADC_convert(*adcHandle, &AdcRaw);
    // Convert the sample to microvolts
    return ADC_convertToMicroVolts(*adcHandle, AdcRaw);
}


