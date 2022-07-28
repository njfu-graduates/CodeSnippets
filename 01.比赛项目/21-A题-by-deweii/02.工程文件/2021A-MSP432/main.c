/*
 *  ======== main_tirtos.c ========
 */

#include "./inc/main.h"

/* POSIX Header files */
#include <pthread.h>

/* RTOS header files */
#include <ti/sysbios/BIOS.h>

/* Driver configuration */
#include <ti/drivers/Board.h>

#include "./inc/myTask.h"
#include "./inc/myADC.h"
#include "./inc/myUart.h"

uint32_t adc_sequence[adc_sequence_size] = {0};
uint32_t adc_kal_sequence[adc_sequence_size] = {0};
// uint32_t adc_pixel_sequence[pixel_width] = {0};
uint8_t flag_date_raw = 1;
double A[7] = {0};
// double THD;
uint16_t current_freq = default_freq;
double param[param_size] = {0};
//作为更高阶的选择的初始值
uint8_t flag_convesing = 0;

void S1_Callback_Function(void)
{
    usleep(100000);
    if (!GPIO_read(S1))
    {
        GPIO_write(LED2, CONFIG_GPIO_LED_ON);
        while (!GPIO_read(S1)) //avoid shaking
            ;
        flag_date_raw = 1;
        My_Task_Init(ADC_Task, 1, 4096);

        GPIO_write(LED2, CONFIG_GPIO_LED_OFF);
    }
}

void S2_Callback_Function(void)
{
    usleep(100000);
    if (!GPIO_read(S2))
    {
        GPIO_write(LED2, CONFIG_GPIO_LED_ON);
        while (!GPIO_read(S2)) //avoid shaking
            ;

        flag_date_raw = 0;
        My_Task_Init(ADC_Task_1KHz, 1, 4096);

        GPIO_write(LED2, CONFIG_GPIO_LED_OFF);
    }
}

void __attribute__((weak)) NDK_hookInit(int32_t id) {}

/*
 *  ======== main ========
 */
int main(void)
{
    /* Call driver init functions */
    Board_init();
    GPIO_init();
    My_Task_Init(mainThread, 1, 1024);
    BIOS_start();
    return (0);
}

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    My_Task_Init(LED_Task, 9, 1024);
    My_Uart_Init(&huart1, USB_UART, 115200);
    My_ADC_Init(&hadc1, adc1);
    GPIO_enableInt(S1); //open the interrupt
    GPIO_enableInt(S2);
    while (1)
    {
    }
}
