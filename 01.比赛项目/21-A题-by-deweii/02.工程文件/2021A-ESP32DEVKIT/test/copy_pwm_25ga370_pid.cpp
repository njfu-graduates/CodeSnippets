#include <Arduino.h>
#include <esp_timer.h>
#include <PID_v1.h>
#include <SimpleKalmanFilter.h>

#define AIN1 12
#define AIN2 13
#define PWMA //上拉

uint16_t freq = 2000;  // 频率
uint8_t channal_L = 0; // 通道0，共16个通道，0~15

uint16_t resolution = 7; // 分辨率，取值0~20，duty最大取值为2^resolution-1
#define PWM_BOUND ((1 << resolution) - 1)
hw_timer_t *timer = NULL;

float speed_L_cal;
int pwm_value_L = 0;
uint32_t cnt_L = 0; //AB相插了同一个外部中断

bool flag_timer = false;
bool flag_pwm = true; //up
#define phase_L 18

#define pluse_once 22 * 12000 / 620 //426
#define test_time_interval 50       //ms

void phase_L_CallBack(void)
{
    cnt_L++;
}
//定时器中断
void IRAM_ATTR timer_CallBack(void)
{
    flag_timer = true;
    speed_L_cal = 3.25 * 3.14 * cnt_L / pluse_once * 1000 / test_time_interval / 100; //计算一圈的距离 * 转速

    cnt_L = 0;
}

SimpleKalmanFilter output_f(2, 4, 0.01);
double Setpoint, Input, Output;
double consKp = 5, consKi = 2, consKd = 1;
PID myPID(&Input, &Output, &Setpoint, consKp, consKi, consKd, DIRECT);
void setup()
{
    Serial.begin(115200);

    pinMode(phase_L, INPUT);
    attachInterrupt(phase_L, phase_L_CallBack, RISING);

    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);

    ledcSetup(channal_L, freq, resolution);

    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &timer_CallBack, true);
    timerAlarmWrite(timer, test_time_interval * 1000, true);
    timerAlarmEnable(timer);

    Setpoint = 30;
    Input = 0;
    myPID.SetMode(AUTOMATIC);
    myPID.SetOutputLimits(-PWM_BOUND, PWM_BOUND);
}

unsigned long freshtime = 0;
#define fresh_interval 4000
void loop()
{
    if (flag_timer)
    {
        flag_timer = false;

        Serial.printf("%.2f,%.2f,%.2f\n", Output, speed_L_cal, Setpoint); // cm/s
    }
    Input = Output > 0 ? speed_L_cal : -speed_L_cal;

    if (millis() - freshtime >= fresh_interval)
    {
        if (Setpoint >= 90)
        {
            Setpoint -= 180;
            delay(2000);
        }
        else
            Setpoint += 10;

        freshtime = millis();
    }

    myPID.Compute();
    Output = output_f.updateEstimate(Output);
    if (Output < 0)
    {
        ledcDetachPin(AIN2);
        ledcAttachPin(AIN1, channal_L);
        digitalWrite(AIN2, HIGH);
    }
    else
    {
        ledcDetachPin(AIN1);
        ledcAttachPin(AIN2, channal_L);
        digitalWrite(AIN1, HIGH);
    }

    ledcWrite(channal_L, PWM_BOUND - abs(Output));

    delay(50);
}