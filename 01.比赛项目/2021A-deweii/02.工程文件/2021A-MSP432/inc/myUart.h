/*
 * myUart.h
 *
 *  Created on: 2021��11��5��
 *      Author: 49945
 */

#ifndef INC_MYUART_H_
#define INC_MYUART_H_

#include "./inc/main.h"

// Import the UART driver definitions
#include <ti/drivers/UART.h>

#define Uart_Rx_Buffer_Size 2

extern char Uart_Rx_Buffer[Uart_Rx_Buffer_Size];

void My_Uart_Init(UART_Handle *huart, uint_least8_t index, uint32_t BaudRate);
void UART_printf(UART_Handle handle, const char *format,...);

//Example
//My_Uart_Init(&huart1, USB_UART, 115200);
//UART_write(huart1, "OK\r\n", 5);

extern UART_Handle huart1;

#endif /* INC_MYUART_H_ */
