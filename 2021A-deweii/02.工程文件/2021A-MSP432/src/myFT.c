#include "./inc/myFT.h"
#include "./inc/main.h"

/**
 * param
 * n            需要计算的谐波的阶数
 * dstBuffer    ADC采样数列
 * seq_size     需要计算的一个周期的点数
 * 
 * return       n次谐波的赋值的平方
*/
double my_adc_ft(int n, uint32_t dstBuffer[adc_sequence_size], int seq_size)
{
    uint32_t ii;
    double res;
    double ft_an = 0, ft_bn = 0;
    for (ii = 0; ii < seq_size; ii++)
    {
        ft_an += 2 * (double)dstBuffer[ii] * cos(2 * PI * n * ii / seq_size) / seq_size;
        ft_bn += 2 * (double)dstBuffer[ii] * sin(2 * PI * n * ii / seq_size) / seq_size;
    }

    param[n - 1] = ft_an;
    param[n + 6] = ft_bn;

    //the result is the square of the coefficient
    res = (double)ft_an * ft_an + (double)ft_bn * ft_bn;
    return res;
}

/**
 * param
 * n            需要计算的THD的阶数
 * dstBuffer    ADC采样数列
 * seq_size     需要计算的一个周期的点数
 * 
 * return       THDn
*/
double my_adc_get(int n, uint32_t dstBuffer[adc_sequence_size], int seq_size)
{
    double res = 0;
    uint32_t ii;
    for (ii = 0; ii < n; ii++)
    {
        A[ii] = my_adc_ft(ii + 1, dstBuffer, seq_size);
        param[U1 + ii] = sqrt(A[ii] / A[0]); //Un
    }

    return res;
}

//更新归一化幅值 Ux
void refresh_param(void)
{
}
