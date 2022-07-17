#include <Arduino.h>
#include <esp_timer.h>
/*
在定时器终端中找外部中断的次数
*/

hw_timer_t *Timer = NULL;

float speed_L_cal;
uint16_t cnt_L_1 = 0, cnt_L_2 = 0;
// float speed_R_cal;
// uint16_t cnt_R_1 = 0, cnt_R_2 = 0;

bool flag_timer = false;

#define pluse_time 11 * 12000 / 620 //213
#define test_time_interval 50       //ms
//外部中断
void int_L_A(void) { cnt_L_1++; }
void int_L_B(void) { cnt_L_2++; }

//定时器中断
void IRAM_ATTR timerInt(void)
{
    flag_timer = true;
    speed_L_cal = 6.5 * 3.14 * (cnt_L_1 + cnt_L_2) / 2 / 11;
    // Serial.printf("timerInt:%d %d %.2f\n", cnt_L_1, cnt_L_2, speed_L_cal);

    cnt_L_1 = 0;
    cnt_L_2 = 0;
}

#define L_A 13
#define L_B 12

void setup()
{
    Serial.begin(115200);

    pinMode(L_A, INPUT);
    pinMode(L_B, INPUT);

    attachInterrupt(L_A, int_L_A, RISING);
    attachInterrupt(L_B, int_L_B, RISING);
    Timer = timerBegin(0, 80, true); // 选择timer0，分频系数为80，向上计数
    timerAttachInterrupt(Timer, &timerInt, true);
    timerAlarmWrite(Timer, test_time_interval * 1000, true); // 设置报警保护函数
    timerAlarmEnable(Timer);
}

void loop()
{
    if (flag_timer)
    {
        flag_timer = false;
        delay(100);
        Serial.println(speed_L_cal);
    }
}
