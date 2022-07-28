#ifndef __L298N_H
#define __L298N_H 			   
#include "stm32f10x.h"
#include "sys.h"

#define XiuZhengValue 0.975

#define	EA							GPIO_Pin_0
#define	EB							GPIO_Pin_1	
#define L298N_EN_GPIO		GPIOA
#define L298N_EN_CLK		RCC_APB1Periph_TIM2
#define L298N_PWM_TIM		TIM2

#define	IN1							GPIO_Pin_4	
#define	IN2							GPIO_Pin_5	
#define	IN3							GPIO_Pin_6	
#define	IN4							GPIO_Pin_7	
#define L298N_GPIO			GPIOA
#define L298N_CLK				RCC_APB2Periph_GPIOA

#define IN1_HIGH	GPIO_SetBits( L298N_GPIO , IN1 )
#define IN1_LOW		GPIO_ResetBits( L298N_GPIO , IN1 )
#define IN2_HIGH	GPIO_SetBits( L298N_GPIO , IN2 )
#define IN2_LOW		GPIO_ResetBits( L298N_GPIO , IN2 )
#define IN3_HIGH	GPIO_SetBits( L298N_GPIO , IN3 )
#define IN3_LOW		GPIO_ResetBits( L298N_GPIO , IN3 )
#define IN4_HIGH	GPIO_SetBits( L298N_GPIO , IN4 )
#define IN4_LOW		GPIO_ResetBits( L298N_GPIO , IN4 )

//state	车轮转向
#define GO    0//定义电机状态 前进
#define BACK  1//后退
#define STOP  2//停车

void TIM2_PWM_Init(u16 arr,u16 psc);

/*****车轮转向控制******/
void L_MOTO(char state);
void R_MOTO(char state);

/*****行驶方向控制*****/
void Car_Go(void);
void Car_Back(void);									
void Car_Turn_Left(void);
void Car_Left(void);
void Car_Turn_Right(void);
void Car_Right(void);
void Car_Stop(void);

void setSpeed(u16 LSpeed , u16 RSpeed);//设置速度

/*******行驶方向和速度控制*******/
void Forward(u16 speed);//直行
void Back(u16 speed);//后退
void Stop(void);//停止
void LCorrect(u16 speed);//前行向左修正
void LLCorrect(u16 speed);//前行向左大幅修正
void RCorrect(u16 speed);//前行向右修正
void RRCorrect(u16 speed);//前行向右大幅修正
void Left(u16 speed);//原地向左转弯或旋转
void Right(u16 speed);//原地向右转弯或旋转

#endif
