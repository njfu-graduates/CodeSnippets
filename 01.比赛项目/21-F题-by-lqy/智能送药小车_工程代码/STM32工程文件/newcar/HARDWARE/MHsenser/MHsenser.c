#include "MHsenser.h"

u8 is_load;

void MHsenser_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	is_load = 0;
	
	RCC_APB2PeriphClockCmd(MH_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = MH_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(MH_GPIO, &GPIO_InitStructure);
	GPIO_ResetBits(MH_GPIO,MH_Pin);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
}

void IR(void)
{
	if(GPIO_ReadInputDataBit(MH_GPIO,MH_Pin)==1)//未装载药品
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		is_load = 0;
	}
	else if(GPIO_ReadInputDataBit(MH_GPIO,MH_Pin)==0)//物药被取走
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
		is_load = 1;
	}
}
