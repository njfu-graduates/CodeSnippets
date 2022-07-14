#include "./inc/myUart.h"
#include <ti/drivers/GPIO.h>

#include "./inc/myTask.h"

UART_Handle huart1;

char Uart_Rx_Buffer[Uart_Rx_Buffer_Size] = {0};

void Uart_TxCallback_Func(UART_Handle handle, void *buf, size_t count)
{
}

//接受满数据 后面的数据就会阻塞 需要处理完继续产生
void Uart_RxCallback_Func(UART_Handle handle, void *buf, size_t count)
{

    //控制命令的起始字符
    if (Uart_Rx_Buffer[0] == 0xff)
    {
        current_freq = (int)Uart_Rx_Buffer[1];
    }

    if (Uart_Rx_Buffer[0] == 0xfe)
    {
        current_freq = (int)Uart_Rx_Buffer[1] * 100;
    }
    //显示命令的起始字符
    if (Uart_Rx_Buffer[0] == 0 && flag_convesing == 0)
    {
        flag_date_raw = (int)Uart_Rx_Buffer[1];

        My_Task_Init(ADC_Task, 1, 4096);
    }

    //这个函数放在回调函数的后面 以开启下一次中断
    UART_read(huart1, Uart_Rx_Buffer, Uart_Rx_Buffer_Size);
}

void My_Uart_Init(UART_Handle *huart, uint_least8_t index, uint32_t BaudRate)
{
    UART_Params uartParams;
    // Initialize the UART driver.  UART_init() must be called before
    // calling any other UART APIs.
    UART_init();
    // Create a UART with data processing off.
    UART_Params_init(&uartParams);
    uartParams.readMode = UART_MODE_CALLBACK;
    //  uartParams.writeMode = UART_MODE_CALLBACK;
    uartParams.writeMode = UART_MODE_BLOCKING;
    uartParams.readCallback = Uart_RxCallback_Func;
    uartParams.writeCallback = Uart_TxCallback_Func;
    uartParams.writeDataMode = UART_DATA_TEXT;
    uartParams.readDataMode = UART_DATA_TEXT;
    uartParams.readReturnMode = UART_RETURN_NEWLINE;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.baudRate = BaudRate;
    // Open an instance of the UART drivers
    *huart = UART_open(index, &uartParams);
    if (*huart == NULL)
    {
        // UART_open() failed
        while (1)
            ;
    }
    UART_read(*huart, Uart_Rx_Buffer, Uart_Rx_Buffer_Size);
}

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
void UART_printf(UART_Handle handle, const char *format, ...)
{
    uint32_t length;
    va_list args;
    char TxBuffer[32] = {0};

    va_start(args, format);
    length = vsnprintf((char *)TxBuffer, sizeof(TxBuffer) + 1, (char *)format, args);
    va_end(args);

    UART_write(handle, TxBuffer, length);
}
