#include <Arduino.h>
hw_timer_t *timer = NULL;
static void IRAM_ATTR Timer0_CallBack(void);

void setup()
{
    Serial.begin(115200);
    timer = timerBegin(0, 80, true);                     // 选择timer0，分频系数为80，向上计数
    timerAttachInterrupt(timer, &Timer0_CallBack, true); // 绑定中断函数
    timerAlarmWrite(timer, wdtTimeout * 1000, true);     // 设置报警保护函数
    timerAlarmEnable(timer);                             // 使能报警器
}

void loop()
{
    
}

static void IRAM_ATTR Timer0_CallBack(void)
{
}