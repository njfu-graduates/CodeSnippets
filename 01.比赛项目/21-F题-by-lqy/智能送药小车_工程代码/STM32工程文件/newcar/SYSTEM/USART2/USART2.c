#include "usart2.h"

u8 Dir;
u8 get_DA;

void USART2_Init(u32 My_BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStrue;
	USART_InitTypeDef USART_InitStrue;
	NVIC_InitTypeDef NVIC_InitStrue;
	
	Dir = NO_ACT;
	get_DA = 0;
	
	//外设使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	USART_DeInit(USART2);  //复位串口2 -> 可以没有
	
	//初始化 串口对应IO口  TX-PA2  RX-PA3
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStrue);
	
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_3;
  GPIO_Init(GPIOA,&GPIO_InitStrue);
	
	//初始化 中断优先级
	NVIC_InitStrue.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStrue.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStrue);
	
	//初始化 串口模式状态
	USART_InitStrue.USART_BaudRate=My_BaudRate; // 波特率
	USART_InitStrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None; //硬件流控制
	USART_InitStrue.USART_Mode=USART_Mode_Tx|USART_Mode_Rx; //发送 接收 模式都是用
	USART_InitStrue.USART_Parity=USART_Parity_No; //没有奇偶校验
	USART_InitStrue.USART_StopBits=USART_StopBits_1; //一位停止位
	USART_InitStrue.USART_WordLength=USART_WordLength_8b; //每次发送数据宽度为8位
	USART_Init(USART2,&USART_InitStrue);
	
	USART_Cmd(USART2,ENABLE);//使能串口
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//开启接收中断
}
 
void USART2_IRQHandler(void) //串口2中断服务函数
{
	u8 res;
	if(USART_GetITStatus(USART2,USART_IT_RXNE)) //中断标志
	{
		res= USART_ReceiveData(USART2);  //串口2 接收
		if(get_DA == 0)
		{
			if(res == 'S')
			{
				GREEN_ON;RED_ON;
				delay_ms(500);
				GREEN_OFF;RED_OFF;
			}
			else if(res == 'E')
			{
				get_DA = 1;
				USART_SendData(USART1,get_DA);
				while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); //等待发送完成
			}
		}
		else if(get_DA == 1)
		{
			switch(res)
			{
				case 'L': Dir = TURN_LEFT;break;
				case 'R': Dir = TURN_RIGHT;break;
//				case 'N':Dir = GO_AHEAD;break;
				default:Dir = GO_AHEAD;break;
			}
			if(car_state == GOTO_DA)
			{
				if(cross_cnt == 0 && Dir_record_1 == NO_ACT)
				{
					Dir_record_1 = Dir;
				}
				else if(cross_cnt == 1 && Dir_record_2 == NO_ACT)
				{
					Dir_record_2 = Dir;
				}
				else if(cross_cnt == 2 && Dir_record_3 == NO_ACT)
				{
					Dir_record_3 = Dir;
				}
				else if(cross_cnt == 3 && Dir_record_4 == NO_ACT)
				{
					Dir_record_4 = Dir;
				}	
			}
			
		}
		
//		USART_SendData(USART1,res);
//		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); //等待发送完成
//		USART_SendData(USART1,0x12);
//		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); //等待发送完成
//     USART_SendData(USART2,res);   //串口2 发送
  }
}
