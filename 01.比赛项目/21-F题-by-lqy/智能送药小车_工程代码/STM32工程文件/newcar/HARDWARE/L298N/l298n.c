#include "l298n.h"

void TIM2_PWM_Init(u16 arr,u16 psc)
{
	//**结构体声明**//
	GPIO_InitTypeDef GPIO_InitStructure;            //声明GPIO
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; //声明定时器
	TIM_OCInitTypeDef  TIM_OCInitStructure;         //声明PWM通道
	
	//**时钟使能**//
	RCC_APB1PeriphClockCmd(L298N_EN_CLK, ENABLE);	//使能定时器TIM2时钟
	RCC_APB2PeriphClockCmd(L298N_CLK, ENABLE);	 //使能PA端口时钟
	
	//**PWM引脚**//
	GPIO_InitStructure.GPIO_Pin = EA | EB;				 //TIM2_CH1 --> PA0 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(L298N_EN_GPIO, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.0
	
	//**IN引脚**//	
	GPIO_InitStructure.GPIO_Pin = IN1	| IN2 | IN3 | IN4;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(L298N_GPIO, &GPIO_InitStructure);					 
	
	//初始化TIM2
	TIM_TimeBaseStructure.TIM_Period = arr-1; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc-1; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(L298N_PWM_TIM, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	//初始化TIM2 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset ;
	TIM_OCInitStructure.TIM_Pulse = 50; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性高
	TIM_OC1Init(L298N_PWM_TIM, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM2 OC1
	TIM_OC2Init(L298N_PWM_TIM, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM2 OC2

	TIM_OC1PreloadConfig(L298N_PWM_TIM, TIM_OCPreload_Enable);  //CH1预装载使能	 
	TIM_OC2PreloadConfig(L298N_PWM_TIM, TIM_OCPreload_Enable);  //CH2预装载使能
	
  TIM_ARRPreloadConfig(L298N_PWM_TIM, ENABLE); //使能TIM2在ARR上的预装载寄存器
 	
	TIM_Cmd(L298N_PWM_TIM, ENABLE);  //使能TIM2   
	TIM_CtrlPWMOutputs(L298N_PWM_TIM,ENABLE);	//MOE 主输出使能	
}

void L_MOTO(char state)
{
	if(state == GO)
	{
		IN1_HIGH;
		IN2_LOW;
	}
	if(state == BACK)
	{
		IN1_LOW;
		IN2_HIGH;
	}
	if(state == STOP)
	{
		IN1_LOW;
		IN2_LOW;
	}
}

void R_MOTO(char state)
{
	if(state == GO)
	{
		IN3_HIGH;
		IN4_LOW;
	}
	if(state == BACK)
	{
		IN3_LOW;
		IN4_HIGH;
	}
	if(state == STOP)
	{
		IN3_LOW;
		IN4_LOW;
	}	
}

void Car_Go(void)
{
	L_MOTO(GO);
	R_MOTO(GO);
}

void Car_Back(void)
{
	L_MOTO(BACK);
	R_MOTO(BACK);	
}

void Car_Turn_Left(void)
{
	L_MOTO(STOP);
	R_MOTO(GO);
}

void Car_Left(void)
{
	L_MOTO(BACK);
	R_MOTO(GO);
}

void Car_Turn_Right(void)
{
	L_MOTO(GO);
	R_MOTO(STOP);	
}

void Car_Right(void)
{
	L_MOTO(GO);
	R_MOTO(BACK);	
}

void Car_Stop(void)
{
	L_MOTO(STOP);
	R_MOTO(STOP);
}

//设置速度
void setSpeed(u16 LSpeed , u16 RSpeed)
{
	TIM_SetCompare1(L298N_PWM_TIM , LSpeed);
	TIM_SetCompare2(L298N_PWM_TIM , RSpeed);
}

void Forward(u16 speed)
{
	setSpeed(speed, speed);
	Car_Go();
}
void Back(u16 speed)
{
	setSpeed(speed , speed);
	Car_Back();
}

void Stop(void)
{
	setSpeed(0 , 0);
	Car_Stop();
}

void LCorrect(u16 speed)//前行向左修正
{
	setSpeed(speed , speed);
//	Car_Left();
	Car_Turn_Left();
}
void LLCorrect(u16 speed)//前行向左大幅修正
{
	setSpeed(speed , speed+4);
//	Car_Left();
	Car_Turn_Left();
}
void RCorrect(u16 speed)//前行向右修正
{
	setSpeed(speed , speed);
//	Car_Right();
	Car_Turn_Right();
}

void RRCorrect(u16 speed)//前行向右大幅修正
{
	setSpeed(speed+4 , speed);
//	Car_Right();
	Car_Turn_Right();
}

void Left(u16 speed)
{
	setSpeed(speed, speed);
	Car_Left();
}

void Right(u16 speed)
{
	setSpeed(speed , speed);
	Car_Right();
}
