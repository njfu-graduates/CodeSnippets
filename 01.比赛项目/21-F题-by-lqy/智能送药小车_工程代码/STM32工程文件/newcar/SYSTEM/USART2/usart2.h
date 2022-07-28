#ifndef __USART2_H
#define __USART2_H
#include "stdio.h"	
#include "sys.h" 

extern u8 Dir;
extern u8 get_DA;

#define NO_ACT 			0
#define GO_AHEAD		1
#define TURN_LEFT 	2
#define TURN_RIGHT 	3

void USART2_Init(u32 My_BaudRate);

	
#endif
