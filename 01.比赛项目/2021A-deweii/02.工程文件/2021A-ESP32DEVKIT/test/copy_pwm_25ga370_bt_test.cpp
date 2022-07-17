#include <Arduino.h>
#include <BluetoothSerial.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial BS;
uint16_t freq1 = 50;       // 频率 20ms
uint8_t channelA = 0;      // 通道0，共16个通道，0~15
uint16_t resolution1 = 20; // 分辨率，取值0~20，duty最大取值为2^resolution-1
float duty = 1.5;
uint8_t MODE = 0;
#define PWM1 13

void handleBuletooth()
{
  while (BS.available())
  {
    uint16_t cmd = BS.read();
    if (cmd == 0x01 || cmd == 0x04)
    {
      MODE |= cmd;
    }
    else
    {
      MODE &= (0x0f - (cmd >> 1));
    }
  }
}

void setup()
{
  Serial.begin(115200);
  BS.begin("test_devkit");
  ledcSetup(channelA, freq1, resolution1);
  ledcAttachPin(PWM1, channelA);
}

void loop()
{
  if (BS.available())
  {
    handleBuletooth();
  }
  switch (MODE)
  {
  case 0x01:
  {
    duty += 0.01;
    break;
  }

  case 0x04:
  {
    duty -= 0.01;
    break;
  }

  default:
  {
    break;
  }
  }

  if (duty > 5)
  {
    duty = 5;
  }
  else if (duty < 0.04)
  {
    duty = 0.04;
  }
  Serial.println(duty);
  ledcWrite(channelA, uint32_t(duty * 1024 * 1024 / 20));
  delay(100);
}