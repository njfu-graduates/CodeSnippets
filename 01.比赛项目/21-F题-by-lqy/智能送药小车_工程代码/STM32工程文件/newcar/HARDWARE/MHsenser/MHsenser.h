#ifndef __MHSENSER_H
#define __MHSENSER_H 			   
#include "stm32f10x.h"
#include "sys.h"

#define MH_CLK		RCC_APB2Periph_GPIOB
#define MH_Pin		GPIO_Pin_0
#define MH_GPIO		GPIOB

extern u8 is_load;//是否装载药品标志位

void MHsenser_Init(void);
void IR(void);
#endif
