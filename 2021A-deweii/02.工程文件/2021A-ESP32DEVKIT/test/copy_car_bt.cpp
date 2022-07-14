#include <Arduino.h>
// //遥感
// #define ADC_RX 35
// #define ADC_RY 34
// #define SW 32
//八路循迹
#define FORWARD 0x01
#define BACKWARD 0x02
#define LEFT 0x04
#define RIGHT 0x08
#define FORWARD_R 0x10
#define BACKWARD_R 0x20
#define LEFT_R 0x40
#define RIGHT_R 0x80
uint16_t MODE = 0;

#define speedMAX 1023
#define speedMIN 256
#define speedInteral 64

//-------------------------//
uint16_t freq1 = 2000;     // 频率
uint8_t channelA = 0;      // 通道0，共16个通道，0~15
uint16_t resolution1 = 10; // 分辨率，取值0~20，duty最大取值为2^resolution-1

uint16_t freq2 = 2000;     // 频率
uint8_t channelB = 1;      // 通道1，共16个通道，0~15
uint16_t resolution2 = 10; // 分辨率，取值0~20，duty最大取值为2^resolution-1
//-------------------------//

//-------------------------//
int speedA = 0; //left
int speedB = 0; //right
//-------------------------//

#define d1 36
#define d2 39
#define d3 34
#define d4 35
#define d5 32
#define d6 33
#define d7 25
#define d8 26
#define AIN1 12
#define AIN2 13
#define BIN1 14
#define BIN2 27
#define PWMA 17
#define PWMB 16

void modeChange(void);

void setup()
{
  Serial.begin(115200);
  ledcSetup(channelA, freq1, resolution1); // 设置通道0
  ledcAttachPin(PWMA, channelA);           // 将通道0与引脚26连接
  ledcSetup(channelB, freq2, resolution2); // 设置通道1
  ledcAttachPin(PWMB, channelB);           // 将通道1与引脚25连接
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  adcAttachPin(d1);
  adcAttachPin(d2);
  adcAttachPin(d3);
  adcAttachPin(d4);
  adcAttachPin(d5);
  adcAttachPin(d6);
  adcAttachPin(d7);
  adcAttachPin(d8);

  MODE = FORWARD;
  modeChange();
}

uint8_t count = 0;
void loop()
{
  Serial.print(analogRead(d1));
  Serial.print(",");
  Serial.print(analogRead(d2));
  Serial.print(",");
  Serial.print(analogRead(d3));
  Serial.print(",");
  Serial.print(analogRead(d4));
  Serial.print(",");
  Serial.print(analogRead(d5));
  Serial.print(",");
  Serial.print(analogRead(d6));
  Serial.print(",");
  Serial.print(analogRead(d7));
  Serial.print(",");
  Serial.print(analogRead(d8));
  Serial.println("");
  delay(100);
  if (count == 100)
  {
    MODE = FORWARD_R;
    modeChange();
    count++;
  }
  else
  {
    count++;
  }
}

void modeChange()
{
  switch (MODE)
  {
  case 0x01:
  {
    speedA = speedMIN;
    speedB = speedMIN;
    break;
  }
  case 0x02:
  {
    speedA = -speedMIN;
    speedB = -speedMIN;
    break;
  }
  case 0x04:
  {
    speedA = -speedMIN;
    speedB = speedMIN;
    break;
  }
  case 0x08:
  {
    speedA = speedMIN;
    speedB = -speedMIN;
    break;
  }
  case 0x05:
  {
    speedA = speedMIN;
    speedB = speedMAX;
    break;
  }
  case 0x09:
  {
    speedA = speedMAX;
    speedB = speedMIN;
    break;
  }
  case 0x06:
  {
    speedA = -speedMIN;
    speedB = -speedMAX;
    break;
  }
  case 0x0A:
  {
    speedA = -speedMAX;
    speedB = -speedMIN;
    break;
  }
  default:
  {
    speedA = 0;
    speedB = 0;
    break;
  }
  }

  if (speedA >= speedMIN) //A forward
  {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    if (speedA > speedMAX)
    {
      speedA = speedMAX;
    }
  }
  else if (speedA <= -speedMIN) //A backward
  {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    if (speedA < -speedMAX)
    {
      speedA = -speedMAX;
    }
  }
  else //stop
  {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, HIGH);
    speedA = 0;
  }
  // case MODE_RELEASE:
  // {
  //   digitalWrite(AIN1, LOW);
  //   digitalWrite(AIN2, LOW);
  //   break;
  // }

  if (speedB >= speedMIN)
  {
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    if (speedB >= speedMAX)
    {
      speedB = speedMAX;
    }
  }
  else if (speedB <= -speedMIN)
  {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    if (speedB < -speedMAX)
    {
      speedB = speedMAX;
    }
  }
  else
  {
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, HIGH);
    speedB = 0;
  }
  // case MODE_RELEASE:
  // {
  //   digitalWrite(BIN1, LOW);
  //   digitalWrite(BIN2, LOW);
  //   break;
  // }

  ledcWrite(channelA, abs(speedA));
  ledcWrite(channelB, abs(speedB));
}
