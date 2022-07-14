#ifndef _BT_H_
#define _BT_H_

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// extern BluetoothSerial BS;

// void loop()
// {
//   if (BS.available())
//   {
//     Serial.write(BS.read());
//   }
//   if (Serial.available())
//   {
//     BS.write(Serial.read());
//   }
// }

#endif