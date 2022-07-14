#ifndef INC_MYTASK_H_
#define INC_MYTASK_H_

#include "./inc/main.h"

void *mainThread(void *arg0);
void My_Task_Init(void *(*startroutine)(void *), int priority, size_t stacksize);

void *LED_Task(void *arg0);
void *ADC_Task(void *arg0);
void *ADC_Task_1KHz(void *arg0);

#endif /* INC_MYTASK_H_ */
