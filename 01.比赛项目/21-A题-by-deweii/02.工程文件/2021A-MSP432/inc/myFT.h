#ifndef INC_MYFT_H_
#define INC_MYFT_H_

#include "./inc/main.h"
#include "math.h"

#define PI 3.1416//M_PI
#define adc_max 3300000
#define adc_min 0

double my_adc_ft(int n, uint32_t dstBuffer[adc_sequence_size], int seq_size);
double my_adc_get(int n, uint32_t dstBuffer[adc_sequence_size], int seq_size);
void refresh_param(void);

#endif /* INC_MYFT_H_ */
