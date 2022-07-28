#include "tracking.h"

u8 car_state;
u8 Dir_record[DIR_SIZE] = {0};
u8 Dir_record_1;
u8 Dir_record_2;
u8 Dir_record_3;
u8 Dir_record_4;
u8 next_Dir;
int flag;
int cross_cnt;

//#define	START				0
//#define	GOTO_DA			1
//#define	AT_DA				2
//#define	BACKTO_SA		3
//#define	AT_SA				4

//#define NO_ACT 			0
//#define GO_AHEAD		1
//#define TURN_LEFT 	2
//#define TURN_RIGHT 	3

void tracking_init(void)
{
	car_state = START;
	cross_cnt = 0;
	next_Dir = NO_ACT;
	flag = 0;
	Dir_record_1 = NO_ACT;
	Dir_record_2 = NO_ACT;
	Dir_record_3 = NO_ACT;
	Dir_record_4 = NO_ACT;
}

//uint8_t mytick = uwtick;
//if(uwtick -mytick > 200)
//{
//	mytick = uwtick;
//	
//}

/**********循迹*********/
void tracking(void)
{
	if(car_state == START)
	{
		//当获取目标地址且装载药品后启动
		Stop();
		IR();
		if(get_DA == 1 && is_load == 1)
		{
			car_state = GOTO_DA;
			delay_ms(200);
		}
	}
	/**********前往目标病房****************/
	if(car_state == GOTO_DA)//前往目标病房
	{
		if(is_end == 1)
		{
			Stop();
			is_end = 0;
			car_state = AT_DA;
			RED_ON;
		}
		else if(is_cross == 1)
		{
//			RED_OFF;GREEN_ON;
//			USART_SendData(USART1,'c');
//			while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
			is_cross = 0;
//			Stop();
			Forward(TURN_SPEED);
			delay_ms(GO_TIME_1);//前进一段路到达路口中心
			Dir_record[cross_cnt] = Dir;//记录路口转向并记录经过路口数目
			cross_cnt++;
			switch(Dir)
			{
				case GO_AHEAD:
					Forward(TURN_SPEED);
					delay_ms(200);
					break;
				case TURN_LEFT:
					Left(TURN_SPEED);
					delay_ms(TURN_TIME_1);
					break;
				case TURN_RIGHT:
					Right(TURN_SPEED);
					delay_ms(TURN_TIME_1);
					break;
				default:break;
			}
		}
//		if(is_end == 0 && is_cross == 0)//五路寻迹
		else
		{
			if( F1 == 0 && F2 == 1 && F4 == 0 && F5 == 0 )
			{
				LCorrect(CORRECT_SPEED);
			}
			else if( F1 == 1 && F4 == 0 && F5 == 0 )
			{
				LLCorrect(CORRECT_SPEED);
			}
			else if( F1 == 0 && F2 == 0 && F4 == 1 && F5 == 0 )
			{
				RCorrect(CORRECT_SPEED);
			}
			else if( F1 == 0 && F2 == 0 && F5 == 1 )
			{
				RRCorrect(CORRECT_SPEED);
			}	
			else
			{
				Forward(SETSPEED);
			}
		}
		
	}
	/**********到达目标病房*********/
	if(car_state == AT_DA)
	{
		//等待药品被取走
		Stop();
		IR();
		if(is_load == 0)
		{
			car_state = BACKTO_SA;
			RED_OFF;
			delay_ms(200);
			//原地调头
			Left(TURN_SPEED);
			delay_ms(BACK_TIME);
		}
	}
	/**********返回药房****************/
	if(car_state == BACKTO_SA)//返回药房
	{
		if(is_end == 1)
		{
			is_end = 0;
			Stop();
			car_state = AT_SA;
			GREEN_ON;
		}
		else if(is_cross == 1)
		{
			is_cross = 0;
//			Stop();
			Forward(TURN_SPEED);
			delay_ms(GO_TIME_2);//前进一段路到达路口中心
			cross_cnt--;
//			Right(TURN_SPEED);
//			delay_ms(TURN_TIME_2);
			switch(Dir_record[cross_cnt])
			{
				case GO_AHEAD:
					Forward(TURN_SPEED);
					delay_ms(200);
					break;
				case TURN_LEFT:
					Right(TURN_SPEED);
					delay_ms(TURN_TIME_2);
					break;
				case TURN_RIGHT:
					Left(TURN_SPEED);
					delay_ms(TURN_TIME_2);
					break;
				default:break;
			}
		}
//		if(is_end == 0 && is_cross == 0)//五路寻迹
		else
		{
			if( F1 == 0 && F2 == 1 && F4 == 0 && F5 == 0 )
			{
				LCorrect(CORRECT_SPEED);
			}
			else if( F1 == 1 && F4 == 0 && F5 == 0 )
			{
				LLCorrect(CORRECT_SPEED);
			}
			else if( F1 == 0 && F2 == 0 && F4 == 1 && F5 == 0 )
			{
				RCorrect(CORRECT_SPEED);
			}
			else if( F1 == 0 && F2 == 0 && F5 == 1 )
			{
				RRCorrect(CORRECT_SPEED);
			}	
			else
			{
				Forward(SETSPEED);
			}
		}
	}	
	if(car_state == AT_SA)
	{
		Stop();
	}		
	
}

/***************循线函数**************/
void find(void)
{
	if( F1 == 0 && F2 == 0 && F4 == 0 && F5 == 0 )
	{
		Forward(SETSPEED);
	}
	else if( F1 == 0 && F2 == 1 && F4 == 0 && F5 == 0 )
	{
		LCorrect(CORRECT_SPEED);
	}
	else if( F1 == 1 && F4 == 0 && F5 == 0 )
	{
		LLCorrect(CORRECT_SPEED);
	}
	else if( F1 == 0 && F2 == 0 && F4 == 1 && F5 == 0 )
	{
		RCorrect(CORRECT_SPEED);
	}
	else if( F1 == 0 && F2 == 0 && F5 == 1 )
	{
		RRCorrect(CORRECT_SPEED);
	}	
}	

/***************测试转弯函数**************/

void turn_1(void)
{
	if(flag == 0){
		if(is_end == 1)
		{
			Stop();
			RED_ON;
			flag = 1;
		}
		else if(is_cross == 1)
		{
			is_cross = 0;
			GREEN_ON;
			Stop();
			Forward(TURN_SPEED);
			delay_ms(GO_TIME_1);//前进一段路到达路口中心			
			Stop();
			Left(TURN_SPEED);
			delay_ms(400);
			while(F2 == 0)
			{
				delay_ms(DELAY_TIME);
			}
			Stop();
		}
		else if( F1 == 0 && F2 == 0 && F4 == 0 && F5 == 0 )
		{
			Forward(SETSPEED);
		}
		else if( F1 == 0 && F2 == 1 && F4 == 0 && F5 == 0 )
		{
			LCorrect(CORRECT_SPEED);
		}
		else if( F1 == 1 && F4 == 0 && F5 == 0 )
		{
			LLCorrect(CORRECT_SPEED);
		}
		else if( F1 == 0 && F2 == 0 && F4 == 1 && F5 == 0 )
		{
			RCorrect(CORRECT_SPEED);
		}
		else if( F1 == 0 && F2 == 0 && F5 == 1 )
		{
			RRCorrect(CORRECT_SPEED);
		}
		
	}
	
}

/************转向************/
void Direction(void)
{
	switch(next_Dir)
	{
		case NO_ACT:
			Stop();
			break;
		case GO_AHEAD:
			while(is_cross == 1)
			{
				find();
			}
			Stop();
			break;
		case TURN_LEFT:
			Left(TURN_SPEED);
			delay_ms(TURN_TIME_1);
			while(F2 == 0)
			{
				delay_ms(DELAY_TIME);
			}
			Stop();
			break;
		case TURN_RIGHT:
			Right(TURN_SPEED);
			delay_ms(TURN_TIME_2);
			while(F2 == 0)
			{
				delay_ms(DELAY_TIME);
			}
			Stop();
			break;
		default:break;
	}
}

/**************test2****************/
void turn_2(void)
{
	if(flag == 0){
		if(is_end == 1)
		{
			Stop();
			RED_ON;
			flag = 1;
		}
		else if(is_cross == 1)
		{
			is_cross = 0;
//			GREEN_TOGGLE;
			Stop();
			Forward(TURN_SPEED);
			delay_ms(GO_TIME_1);//前进一段路到达路口中心			
			Stop();
			
			if(cross_cnt == 0)
			{
				Dir_record_1 = TURN_LEFT;
			}
			
//			Dir_record[cross_cnt] = TURN_LEFT;//记录路口转向并记录经过路口数目
			next_Dir = TURN_LEFT;
			cross_cnt++;
			Direction();

			Stop();
		}
		else
		{
			find();
		}	
	}
	else if(flag == 1)
	{
		
		delay_ms(200);
		RED_OFF;
		//原地调头
		Left(TURN_SPEED);
		delay_ms(BACK_TIME);
		while(F2 == 0)
		{
			delay_ms(DELAY_TIME);
		}
		flag = 2;
		Stop();
	}
	else if(flag == 2)
	{
		if(is_end == 1)
		{
			Stop();
			GREEN_ON;
			flag = 3;
		}
		else if(is_cross == 1)
		{
			is_cross = 0;
//			RED_TOGGLE;
			Stop();
			Forward(TURN_SPEED);
			delay_ms(GO_TIME_2);//前进一段路到达路口中心			
			Stop();
			
			if(cross_cnt == 1)
			{
				next_Dir = Dir_record_1;
			}
			cross_cnt--;
//			next_Dir = Dir_record[cross_cnt];
			switch(next_Dir)
			{
				case GO_AHEAD:
					next_Dir = GO_AHEAD;
					break;
				case TURN_LEFT:
					next_Dir = TURN_RIGHT;
					break;
				case TURN_RIGHT:
					next_Dir = TURN_LEFT;
					break;
			}
			Direction();

			Stop();
		}
		else
		{
			find();
		}
	}
}

/**********************************************/
void turn_3(void)
{
	if(car_state == START)
	{
		Stop();
		IR();
		if(get_DA == 1 && is_load == 1)
		{
			car_state = GOTO_DA;
			delay_ms(300);
		}
	}
	if(car_state == GOTO_DA){
		if(is_end == 1)
		{
			Stop();
			RED_ON;
			car_state = AT_DA;
		}
		else if(is_cross == 1)
		{
			is_cross = 0;
//			GREEN_TOGGLE;
			Stop();
			Forward(TURN_SPEED);
			delay_ms(GO_TIME_1);//前进一段路到达路口中心			
			Stop();
			
			if(cross_cnt == 0)
			{
				next_Dir = Dir_record_1;
			}
			else if(cross_cnt == 1)
			{
				next_Dir = Dir_record_2;
			}
			else if(cross_cnt == 2)
			{
				next_Dir = Dir_record_3;
			}
			else if(cross_cnt == 3)
			{
				next_Dir = Dir_record_4;
			}
			
//			Dir_record[cross_cnt] = TURN_LEFT;//记录路口转向并记录经过路口数目

			cross_cnt++;
			Direction();

			Stop();
		}
		else
		{
			find();
		}	
	}
	else if(car_state == AT_DA)
	{
		IR();
		if(is_load == 0)
		{
			//原地调头
			RED_OFF;
			delay_ms(300);
			Left(TURN_SPEED);
			delay_ms(BACK_TIME);
			while(F2 == 0)
			{
				delay_ms(DELAY_TIME);
			}
			car_state = BACKTO_SA;
			Stop();
		}
		
	}
	else if(car_state == BACKTO_SA)
	{
		if(is_end == 1)
		{
			Stop();
			GREEN_ON;
			car_state = AT_SA;
		}
		else if(is_cross == 1)
		{
			is_cross = 0;
//			RED_TOGGLE;
			Stop();
			Forward(TURN_SPEED);
			delay_ms(GO_TIME_2);//前进一段路到达路口中心			
			Stop();
			
			if(cross_cnt == 1)
			{
				next_Dir = Dir_record_1;
			}
			else if(cross_cnt == 2)
			{
				next_Dir = Dir_record_2;
			}
			else if(cross_cnt == 3)
			{
				next_Dir = Dir_record_3;
			}
			else if(cross_cnt == 4)
			{
				next_Dir = Dir_record_4;
			}
			cross_cnt--;
//			next_Dir = Dir_record[cross_cnt];
			switch(next_Dir)
			{
				case GO_AHEAD:
					next_Dir = GO_AHEAD;
					break;
				case TURN_LEFT:
					next_Dir = TURN_RIGHT;
					break;
				case TURN_RIGHT:
					next_Dir = TURN_LEFT;
					break;
			}
			Direction();

			Stop();
		}
		else
		{
			find();
		}
	}
	else if(car_state == AT_SA)
	{
		Stop();
	}
}

/**********新循迹测试***********/
void turn_test(void)
{
	if(car_state == GOTO_DA)//前往病房
	{
		if(is_end == 1)
		{
			Stop();
			car_state = AT_DA;
			RED_ON;
		}
		else if(is_cross == 1)
		{
			is_cross = 0;
			Stop();
			Forward(TURN_SPEED);
			delay_ms(GO_TIME_1);//前进一段路到达路口中心
			Stop();
			Dir_record[cross_cnt] = Dir;//记录路口转向并记录经过路口数目
			next_Dir = Dir;
			cross_cnt++;
		}
		else if( F1 == 0 && F2 == 0 && F4 == 0 && F5 == 0 )
		{
			Forward(SETSPEED);
		}
		else if( F1 == 0 && F2 == 1 && F4 == 0 && F5 == 0 )
		{
			LCorrect(CORRECT_SPEED);
		}
		else if( F1 == 1 && F4 == 0 && F5 == 0 )
		{
			LLCorrect(CORRECT_SPEED);
		}
		else if( F1 == 0 && F2 == 0 && F4 == 1 && F5 == 0 )
		{
			RCorrect(CORRECT_SPEED);
		}
		else if( F1 == 0 && F2 == 0 && F5 == 1 )
		{
			RRCorrect(CORRECT_SPEED);
		}
	}
	else if(car_state == BACKTO_SA)//返回药房
	{
		if(is_end == 1)
		{
			Stop();
			car_state = AT_SA;
			GREEN_ON;
		}
		else if(is_cross == 1)
		{
			is_cross = 0;
			Stop();
			Forward(TURN_SPEED);
			delay_ms(GO_TIME_1);//前进一段路到达路口中心
			Stop();
			cross_cnt--;
			switch(Dir_record[cross_cnt])
			{
				case GO_AHEAD:
					next_Dir = GO_AHEAD;
					break;
				case TURN_LEFT:
					next_Dir = TURN_RIGHT;
					break;
				case TURN_RIGHT:
					next_Dir = TURN_LEFT;
					break;
			}
		}
		else if( F1 == 0 && F2 == 0 && F4 == 0 && F5 == 0 )
		{
			Forward(SETSPEED);
		}
		else if( F1 == 0 && F2 == 1 && F4 == 0 && F5 == 0 )
		{
			LCorrect(CORRECT_SPEED);
		}
		else if( F1 == 1 && F4 == 0 && F5 == 0 )
		{
			LLCorrect(CORRECT_SPEED);
		}
		else if( F1 == 0 && F2 == 0 && F4 == 1 && F5 == 0 )
		{
			RCorrect(CORRECT_SPEED);
		}
		else if( F1 == 0 && F2 == 0 && F5 == 1 )
		{
			RRCorrect(CORRECT_SPEED);
		}		
	}
	else if(car_state == START)
	{
		//当获取目标地址且装载药品后启动
		Stop();
		IR();
		if(get_DA == 1 && is_load == 1)
		{
			car_state = GOTO_DA;
			delay_ms(200);
		}
	}
	else if(car_state == AT_DA)
	{
		//等待药品被取走
		Stop();
		IR();
		if(is_load == 0)
		{
			car_state = BACKTO_SA;
			RED_OFF;
			delay_ms(200);
			//原地调头
			Left(TURN_SPEED);
			delay_ms(BACK_TIME);
			while(F2 == 0)
			{
				delay_ms(DELAY_TIME);
			}
		}
	}
	else if(car_state == AT_SA)
	{
		Stop();
	}		
}
