#include "usart3.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	 
#include "usart.h"
#include "stdlib.h"

u8 is_end,is_cross;
u8 ROI[5];
 
u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; 				
u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 			
 

vu16 USART3_RX_STA=0;   	
 
int i=0;
void Openmv_Receive_Data(int16_t data)
{
	static u8 openmv[9];
	static u8 Openmv_state = 0;	
	static u8 bit_number=0;	
	if(Openmv_state==0 && data==0x55)
	{
		Openmv_state=1;
		openmv[bit_number++]=data;
	}
	else if(Openmv_state==1)
	{
		openmv[bit_number++]=data;
		if(bit_number>=8)
		{
			Openmv_state=2;
		}
	}
	else if(Openmv_state==2)
	{
		if(data == 0x56)
		{
			Openmv_state = 0;
			openmv[bit_number++]=data;
//			for(i=0;i<9;i++){
//				USART_SendData(USART1,openmv[i]);
//				while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
//			}
//			USART_SendData(USART1,0x12);
//			while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
			
			//另存数据			
			is_end = openmv[1];
			is_cross = openmv[2];
			for(i=0;i<5;i++)
			{
				ROI[i] = openmv[i+3];
			}
		}
		else if(data != 0x56)
		{
			Openmv_state = 0;
			bit_number=0;
			for(i=0;i<9;i++)
			{
				openmv[i]=0x00;
			}           
		}
	}    
	else
	{
		Openmv_state = 0;
		bit_number=0;
		for(i=0;i<9;i++)
		{
			openmv[i]=0x00;
		}
	}
}

 
void USART3_IRQHandler(void)
{
	u8 Res;	      
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{	 
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);//清除中断标志
		Res = USART3->DR;
		Openmv_Receive_Data(Res);//处理数据
		
//		Res = USART_ReceiveData(USART3);		
//		USART_SendData(USART1,Res);
//		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
//		USART_SendData(USART1,0x12);
//		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
	}  				 											 
}   
 
   
void usart3_init(u32 bound)
{  
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	is_end = 0;
	is_cross = 0;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); 
 
 	USART_DeInit(USART3);  
		 //USART3_TX   PB10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
   
    //USART3_RX	  PB11
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);  
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
	NVIC_Init(&NVIC_InitStructure);	
		
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
	USART_Init(USART3, &USART_InitStructure); 
  
	USART_Cmd(USART3, ENABLE);                  
	
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); 
	

	USART3_RX_STA=0;
}
 

void u3_printf(char* fmt,...)
{  
	u16 i,j; 
	va_list ap; 
	va_start(ap,fmt); 
	vsprintf((char*)USART3_TX_BUF,fmt,ap); 
	va_end(ap);
	i=strlen((const char*)USART3_TX_BUF);		
	for(j=0;j<i;j++)							
	{
	  while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); 
		USART_SendData(USART3,USART3_TX_BUF[j]); 
	} 
}
 
void USART3_RX_Data()
{
	u8 len=0;
	if(USART3_RX_STA&0x8000)
		{					   
			len=USART3_RX_STA&0X7FFF;
			USART3_RX_BUF[len]=0;	 	
			
			if(len>USART3_MAX_RECV_LEN-2)
			{
				len=USART3_MAX_RECV_LEN-1;
				USART3_RX_BUF[len]=0;	 	
			}
			
			USART3_RX_BUF[USART3_MAX_RECV_LEN-1]=0x01;
//			u3_printf("%s\r\n",USART3_RX_BUF);
			USART3_RX_STA=0;
		}
 
}
