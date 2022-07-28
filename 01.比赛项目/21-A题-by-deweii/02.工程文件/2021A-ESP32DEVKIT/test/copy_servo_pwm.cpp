#include <Arduino.h>

uint16_t freq = 50;       // 频率
uint8_t channel_A = 0;    // 通道0，共16个通道，0~15
uint16_t resolution = 10; // 分辨率，取值0~20，duty最大取值为2^resolution-1
#define PWM_BOUND ((1 << resolution) - 1)
#define PWM_VALUE(ms) (ms * freq / 1000.0 * PWM_BOUND)

#define SERVO_MS_MAX 2.5
#define SERVO_MS_MIN 0.35

#define SERVO_PWM_PIN_A 2

void setup()
{
    Serial.begin(115200);

    pinMode(SERVO_PWM_PIN_A, OUTPUT);
    ledcSetup(channel_A, freq, resolution);
    ledcAttachPin(SERVO_PWM_PIN_A, channel_A);

    for (float i = SERVO_MS_MAX; i > SERVO_MS_MIN; i -= 0.01)
    {
        ledcWrite(channel_A, PWM_VALUE(i));
        Serial.println(i);
        delay(20);
    }
    for (float i = SERVO_MS_MIN; i < SERVO_MS_MAX; i += 0.01)
    {
        ledcWrite(channel_A, PWM_VALUE(i));
        Serial.println(i);
        delay(20);
    }
}

void loop()
{
}