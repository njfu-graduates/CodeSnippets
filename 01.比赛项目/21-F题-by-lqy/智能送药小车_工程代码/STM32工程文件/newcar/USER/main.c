#include "stm32f10x.h"
#include "sys.h"

void init()
{
	TIM2_PWM_Init(100,36);
	Stop();

	MHsenser_Init();
	LED_init();

	uart_init(115200);
	USART2_Init(9600);
	usart3_init(115200);
	
	tracking_init();
}

//void moto_test(){
//		for(i=0;i<40000;i++)		Forward(SETSPEED);
//		for(i=0;i<40000;i++)		Back(SETSPEED);
//		for(i=0;i<40000;i++)		LCorrect(SETSPEED);
//		for(i=0;i<40000;i++)		Stop();	
//}

int main()
{

	init();
//	delay_ms(500);
	while(1)
	{		
//		find();	
//		turn_test();
		turn_3();
//		turn_1();
//		tracking();
		

//		USART_SendData(USART3,0xA2);
//		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET); //等待发送完成
//		delay_ms(500);
//		Forward(SETSPEED);
//		IR();
	}
}
