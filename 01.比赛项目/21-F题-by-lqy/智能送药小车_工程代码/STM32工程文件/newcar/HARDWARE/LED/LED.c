#include "LED.h"

void LED_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(LED_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GREEN_Pin | RED_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(LED_GPIO, &GPIO_InitStructure);
//	GPIO_SetBits(LED_GPIO , GREEN_Pin | RED_Pin);
	RED_OFF;GREEN_OFF;
}
