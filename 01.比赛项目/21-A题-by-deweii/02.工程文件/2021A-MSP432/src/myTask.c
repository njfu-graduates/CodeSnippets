/* POSIX Header files */
#include <pthread.h>

/* RTOS header files */
#include <ti/sysbios/BIOS.h>

#include "./inc/myTask.h"
#include "./inc/myADC.h"
#include "./inc/myUart.h"
#include "./inc/myFT.h"

void My_Task_Init(void *(*startroutine)(void *), int priority, size_t stacksize)
{
    pthread_t thread;
    pthread_attr_t attrs;
    struct sched_param priParam;
    int retc;

    /* Initialize the attributes structure with default values */
    pthread_attr_init(&attrs);

    /* Set priority, detach state, and stack size attributes */
    priParam.sched_priority = priority;
    retc = pthread_attr_setschedparam(&attrs, &priParam);
    retc |= pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_DETACHED);
    retc |= pthread_attr_setstacksize(&attrs, stacksize);
    if (retc != 0)
    {
        /* failed to set attributes */
        while (1)
        {
        }
    }

    retc = pthread_create(&thread, &attrs, startroutine, NULL);
    if (retc != 0)
    {
        /* pthread_create() failed */
        while (1)
        {
        }
    }
}
//function as a flag showing that the system doesn't crack
void *LED_Task(void *arg0)
{
    while (1)
    {
        GPIO_toggle(LED1);

        if (flag_convesing)
        {
            GPIO_write(LED_UART, CONFIG_GPIO_LED_ON);
        }
        else
        {
            GPIO_write(LED_UART, CONFIG_GPIO_LED_OFF);
        }

        sleep(1);
    }
}

void *ADC_Task(void *arg0)
{
    flag_convesing = 1; //防止 干扰

    uint16_t seq_size = adc_sequence_size / current_freq; // 一个周期内的采样数
    uint16_t ii;

    //卡尔曼滤波的参数设定
    double _err_measure = mea_e;
    double _err_estimate = est_e;
    double _q = q;
    double _current_estimate = 0;
    double _last_estimate = 0;
    double _kalman_gain = 0;

    //主要采集部分 由于采样频率不变 所以对于低频的耗时会更久
    for (ii = 0; ii < seq_size; ii++)
    {
        adc_sequence[ii] = Get_Micro_Volts(&hadc1); //循环采样ADC
        // UART_printf(huart1, "%d\r\n", adc_sequence[ii]);

        //something with Kalman
        _kalman_gain = _err_estimate / (_err_estimate + _err_measure);
        _current_estimate = _last_estimate + _kalman_gain * (adc_sequence[ii] - _last_estimate);
        _err_estimate = (1.0 - _kalman_gain) * _err_estimate + fabs(_last_estimate - _current_estimate) * _q;
        _last_estimate = _current_estimate;

        if (ii > 10)
            adc_kal_sequence[ii] = (int)_current_estimate; //前期波动较大
        else
            adc_kal_sequence[ii] = adc_sequence[ii];

        // UART_printf(huart1, "%d\n", adc_kal_sequence[ii]);

        usleep(1); //weird
    }

#ifdef TEST
    //主要数据的筛选 化长数列为有效的短数列进行运算
    //提高运算长度 可以提高精确度
    //最好长度也是可以调动的
    for (ii = 0; ii < pixel_width; ii++)
    {
        adc_pixel_sequence[ii] = adc_kal_sequence[ii * seq_size / pixel_width]; //用来传输给显示部分
        if (ii != 1)                                                            //第一项有干扰
            UART_printf(huart1, "%d\n", adc_pixel_sequence[ii]);                //print exclude the first point
    }

    // THD = my_adc_get(5, adc_kal_sequence, seq_size); //计算5次谐波的THD

    A1 = my_adc_ft(1, adc_pixel_sequence + 1, pixel_width - 1);
    A2 = my_adc_ft(2, adc_pixel_sequence + 1, pixel_width - 1);
    A3 = my_adc_ft(3, adc_pixel_sequence + 1, pixel_width - 1);
    A4 = my_adc_ft(4, adc_pixel_sequence + 1, pixel_width - 1);
    A5 = my_adc_ft(5, adc_pixel_sequence + 1, pixel_width - 1);
    U1 = sqrt(A1 / A1);
    U2 = sqrt(A2 / A1);
    U3 = sqrt(A3 / A1);
    U4 = sqrt(A4 / A1);
    U5 = sqrt(A5 / A1);
    THD = sqrt((A2 + A3 + A4 + A5) / A1);

    // UART_printf(huart1, "%.2f\n", U1);
    // UART_printf(huart1, "%.2f\n", U2);
    // UART_printf(huart1, "%.2f\n", U3);
    // UART_printf(huart1, "%.2f\n", U4);
    // UART_printf(huart1, "%.2f\n", U5);

    UART_printf(huart1, "THD=%.3lf\n", THD * 100);
    //滤波后的THD 一般性而言更加精确
#endif

    //点数越多越精确
    //暂且选择THD7输出
    if (flag_date_raw)
        my_adc_get(7, adc_sequence, seq_size);
    else
        my_adc_get(7, adc_kal_sequence, seq_size);

    param[thd5] = 100 * sqrt((A[1] + A[2] + A[3] + A[4]) / A[0]);
    param[thd6] = 100 * sqrt((A[1] + A[2] + A[3] + A[4] + A[5]) / A[0]);
    param[thd7] = 100 * sqrt((A[1] + A[2] + A[3] + A[4] + A[5] + A[6]) / A[0]);
    param[freq] = current_freq;

    UART_printf(huart1, "===\n");

    for (ii = 0; ii < param_size; ii++)
    {
        UART_printf(huart1, "%.3f\n", param[ii]);
    }

    flag_convesing = 0; //检测结束

    return (NULL);
}


//到5Khz左右的时候 测量加运算的时间就已经差不多10s了
void *ADC_Task_1KHz(void *arg0)
{
    //好像print函数会影响采样率
    flag_convesing = 1; //防止 干扰

    uint16_t seq_size = 1400; // 一个周期内的采样数
    uint16_t ii;

    //卡尔曼滤波的参数设定
    double _err_measure = mea_e;
    double _err_estimate = est_e;
    double _q = q;
    double _current_estimate = 0;
    double _last_estimate = 0;
    double _kalman_gain = 0;

    //主要采集部分 由于采样频率不变 所以对于低频的耗时会更久
    for (ii = 0; ii < seq_size; ii++)
    {
        adc_sequence[ii] = Get_Micro_Volts(&hadc1); //循环采样ADC
        // UART_printf(huart1, "%d\r\n", adc_sequence[ii]);

 


        usleep(1);
    }

    //点数越多越精确
    //暂且选择THD7输出
    // if (flag_date_raw)
    //     my_adc_get(7, adc_sequence, seq_size);
    // else
    //     my_adc_get(7, adc_kal_sequence, seq_size);

    // param[thd5] = 100 * sqrt((A[1] + A[2] + A[3] + A[4]) / A[0]);
    // param[thd6] = 100 * sqrt((A[1] + A[2] + A[3] + A[4] + A[5]) / A[0]);
    // param[thd7] = 100 * sqrt((A[1] + A[2] + A[3] + A[4] + A[5] + A[6]) / A[0]);
    // param[freq] = current_freq;

    // UART_printf(huart1, "===\n");

    // for (ii = 0; ii < param_size; ii++)
    // {
    //     UART_printf(huart1, "%.3f\n", param[ii]);
    // }

    flag_convesing = 0; //检测结束

    return (NULL);
}

//这段可以用作发展部分 14M采集
//当基频超过100KHz时 准确度陡降
//不带滤波算法的值 进行计算结果也是比较准确的
