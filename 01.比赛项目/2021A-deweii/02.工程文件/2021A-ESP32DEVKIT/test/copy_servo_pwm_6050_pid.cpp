#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <PID_v1.h>
#include <SimpleKalmanFilter.h>
uint16_t freq = 50;       // 频率
uint8_t channel_A = 0;    // 通道0，共16个通道，0~15
uint8_t channel_B = 1;    // 通道0，共16个通道，0~15
uint16_t resolution = 10; // 分辨率，取值0~20，duty最大取值为2^resolution-1
#define PWM_BOUND ((1 << resolution) - 1)
#define PWM_VALUE(ms) (ms * freq / 1000.0 * PWM_BOUND)

#define SERVO_MS_MAX 2.5
#define SERVO_MS_MIN 0.35

#define SERVO_PWM_PIN_A 2
#define SERVO_PWM_PIN_B 4

Adafruit_MPU6050 mpu;
//
SimpleKalmanFilter MPU_AY(.03, 0.05, 0.01);
SimpleKalmanFilter MPU_AX(.03, 0.05, 0.01);

double Setpoint_A, Input_A, Output_A;
double Setpoint_B, Input_B, Output_B;
double consKp = 5, consKi = 2, consKd = 1;
PID pid_A(&Input_A, &Output_A, &Setpoint_A, consKp, consKi, consKd, DIRECT);
PID pid_B(&Input_B, &Output_B, &Setpoint_B, consKp, consKi, consKd, DIRECT);
void setup()
{
    Serial.begin(115200);

    mpu.begin();
    mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
    mpu.setGyroRange(MPU6050_RANGE_250_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_94_HZ);

    pinMode(SERVO_PWM_PIN_A, OUTPUT);
    ledcSetup(channel_A, freq, resolution);
    ledcAttachPin(SERVO_PWM_PIN_A, channel_A);
    pinMode(SERVO_PWM_PIN_B, OUTPUT);
    ledcSetup(channel_B, freq, resolution);
    ledcAttachPin(SERVO_PWM_PIN_B, channel_B);

    Setpoint_A = 0;
    Setpoint_B = 0;
    pid_A.SetMode(AUTOMATIC);
    pid_B.SetMode(AUTOMATIC);
    pid_A.SetOutputLimits(SERVO_MS_MIN, SERVO_MS_MAX);
    pid_B.SetOutputLimits(SERVO_MS_MIN, SERVO_MS_MAX);
}

void loop()
{
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    float ay = MPU_AY.updateEstimate(a.acceleration.y);
    float ax = MPU_AX.updateEstimate(a.acceleration.x);

    Input_A = (Output_A + SERVO_MS_MIN + (SERVO_MS_MAX - SERVO_MS_MIN) / 20 * (10 + ay)) / 2;
    Input_B = (Output_B + SERVO_MS_MIN + (SERVO_MS_MAX - SERVO_MS_MIN) / 20 * (10 - ax)) / 2;

    Setpoint_A = SERVO_MS_MIN + (SERVO_MS_MAX - SERVO_MS_MIN) / 20 * (10 + ay);
    Setpoint_B = SERVO_MS_MIN + (SERVO_MS_MAX - SERVO_MS_MIN) / 20 * (10 - ax);

    pid_A.Compute();
    pid_B.Compute();

    ledcWrite(channel_A, PWM_VALUE(Output_A));
    ledcWrite(channel_B, PWM_VALUE(Output_B));

    Serial.printf("%.2f,%.2f\n", Output_A, Output_B);

    delay(100);
}

void pwm_test(void)
{
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