// Basic demo for accelerometer readings from Adafruit MPU6050

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <PID_v1.h>
#include <SimpleKalmanFilter.h>

/*
平衡小车
结果并不是很理想
*/

SimpleKalmanFilter output_f(2, 2, 0.01);
Adafruit_MPU6050 mpu;

float ay;

uint16_t freq1 = 2000;     // 频率
uint8_t channelA = 0;      // 通道0，共16个通道，0~15
uint16_t resolution1 = 10; // 分辨率，取值0~20，duty最大取值为2^resolution-1

uint16_t freq2 = 2000;     // 频率
uint8_t channelB = 1;      // 通道1，共16个通道，0~15
uint16_t resolution2 = 10; // 分辨率，取值0~20，duty最大取值为2^resolution-1
//-------------------------//

#define AIN1 12
#define AIN2 13
#define BIN1 14
#define BIN2 27
#define PWMA 17
#define PWMB 16

#define speedMAX 1023
#define speedMIN 100

#define Output_Limits 25
#define Output_Div 40

int speedA = 0; //left
int speedB = 0; //right

double Setpoint, Input, Output;
double aggKp = 10, aggKi = 0.5, aggKd = 0.1;
double midKp = 2, midKi = 2, midKd = 1;
double consKp = 3, consKi = 3, consKd = 3;
PID myPID(&Input, &Output, &Setpoint, consKp, consKi, consKd, DIRECT);

void setup(void)
{
    Serial.begin(115200);
    mpu.begin();
    mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
    mpu.setGyroRange(MPU6050_RANGE_250_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_94_HZ);
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(BIN1, OUTPUT);
    pinMode(BIN2, OUTPUT);
    ledcSetup(channelA, freq1, resolution1); // 设置通道0
    ledcAttachPin(PWMA, channelA);           // 将通道0与引脚26连接
    ledcSetup(channelB, freq2, resolution2); // 设置通道1
    ledcAttachPin(PWMB, channelB);           // 将通道1与引脚25连接

    Input = 0;
    Setpoint = 0;

    myPID.SetMode(AUTOMATIC);
    myPID.SetOutputLimits(-175, 175);
}

void loop()
{

    /* Get new sensor events with the readings */
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    Input = a.acceleration.y * 10;
    double gap = abs(Setpoint - Input);

    if (gap < 15)
    { //we're close to setpoint, use conservative tuning parameters
        myPID.SetTunings(consKp, consKi, consKd);
    }
    else if (gap < 50)
    {
        myPID.SetTunings(midKp, midKi, midKd);
    }
    else
    {
        //we're far from setpoint, use aggressive tuning parameters
        myPID.SetTunings(aggKp, aggKi, aggKd);
    }

    myPID.Compute();
    float est = output_f.updateEstimate(Output * 1.5);
    if ((abs(est - speedA) < Output_Div) || abs(Input) > 50)
    {
        speedA = est;
    }
    else
    {
        speedA = (est - speedA) > 0 ? speedA + Output_Div : speedA - Output_Div;
    }

    if (speedA > Output_Limits) //A forward
    {
        digitalWrite(AIN1, HIGH);
        digitalWrite(AIN2, LOW);
        digitalWrite(BIN1, HIGH);
        digitalWrite(BIN2, LOW);

        ledcWrite(channelA, speedA);
        ledcWrite(channelB, speedA);
    }
    else if (speedA < -Output_Limits)
    {
        digitalWrite(AIN1, LOW);
        digitalWrite(AIN2, HIGH);
        digitalWrite(BIN1, LOW);
        digitalWrite(BIN2, HIGH);

        ledcWrite(channelA, abs(speedA));
        ledcWrite(channelB, abs(speedA));
    }

    // ledcWrite(channelB, abs(speedB));
    Serial.printf("%d %.2f\n", speedA, a.acceleration.y);

    delay(50);
}