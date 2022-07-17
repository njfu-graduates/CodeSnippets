/*
 * myADC.h
 *
 *  Created on: 2021��11��5��
 *      Author: 49945
 */

#ifndef INC_MYADC_H_
#define INC_MYADC_H_

#include "./inc/main.h"

// Import ADC Driver definitions
#include <ti/drivers/ADC.h>

void My_ADC_Init(ADC_Handle *adcHandle, uint_least8_t index);
uint32_t Get_Micro_Volts(ADC_Handle *adcHandle);


extern ADC_Handle hadc1;

#endif /* INC_MYADC_H_ */
