#ifndef __USART3_H
#define __USART3_H	 
#include "sys.h"  
 
#define USART3_MAX_RECV_LEN		60					
#define USART3_MAX_SEND_LEN		600					
#define USART3_RX_EN 			1					
 
extern u8 is_end,is_cross;
extern u8 ROI[5];
 
void Openmv_Receive_Data(int16_t data);
 
extern u8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; 		
extern u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 		
extern vu16 USART3_RX_STA;   					

void usart3_init(u32 bound);				
void u3_printf(char* fmt,...);
void USART3_RX_Data(void);
#endif
