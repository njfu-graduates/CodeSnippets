#ifndef INC_MAIN_H_
#define INC_MAIN_H_

/* For usleep() */
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>

/* Driver configuration */
#include "ti_drivers_config.h"
/* Driver Header files */
#include <ti/drivers/GPIO.h>
//#include "arm_math.h"

// #define TEST
//经过多次测试得到的最精确结果
#define adc_sequence_size 14000
#define seq_size_1KHz 140
// #define pixel_width 140
#define default_freq 10

//Kalman 参数
#define mea_e 3000
#define est_e 1
#define q 0.01

extern uint32_t adc_sequence[adc_sequence_size];
extern uint32_t adc_kal_sequence[adc_sequence_size];
// extern uint32_t adc_pixel_sequence[pixel_width];

extern uint8_t flag_date_raw;
extern double A[7];
// extern double THD;
extern uint16_t current_freq; //KHz
enum
{
    a1,
    a2,
    a3,
    a4,
    a5,
    a6,
    a7,
    b1,
    b2,
    b3,
    b4,
    b5,
    b6,
    b7,
    U1,
    U2,
    U3,
    U4,
    U5,
    U6,
    U7,
    thd5,
    thd6,
    thd7,
    freq
};
#define param_size 25
extern double param[param_size];

extern uint8_t flag_convesing;

#endif /* INC_MAIN_H_ */
