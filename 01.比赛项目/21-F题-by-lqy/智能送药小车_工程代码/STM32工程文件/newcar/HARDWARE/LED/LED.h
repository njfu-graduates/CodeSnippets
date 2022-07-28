#ifndef __LED_H
#define __LED_H 			   
#include "stm32f10x.h"
#include "sys.h"

#define LED_CLK		RCC_APB2Periph_GPIOC
#define GREEN_Pin		GPIO_Pin_14
#define RED_Pin		GPIO_Pin_15
#define LED_GPIO		GPIOC

void LED_init(void);

#define RED_ON		GPIO_SetBits( LED_GPIO , RED_Pin )
#define RED_OFF		GPIO_ResetBits( LED_GPIO , RED_Pin )
#define RED_TOGGLE	LED_GPIO->ODR ^= RED_Pin
#define GREEN_ON		GPIO_SetBits( LED_GPIO , GREEN_Pin )
#define GREEN_OFF		GPIO_ResetBits( LED_GPIO , GREEN_Pin )
#define GREEN_TOGGLE	LED_GPIO->ODR ^= GREEN_Pin

#endif
