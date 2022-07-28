#ifndef _mcu_touch_magic_h_
#define _mcu_touch_magic_h_

#define TCLK_LOW digitalWrite(tclkPin, LOW)
#define TCLK_HIGH digitalWrite(tclkPin, HIGH)
#define TCS_LOW digitalWrite(tcsPin, LOW)
#define TCS_HIGH digitalWrite(tcsPin, HIGH)
#define TDIN_LOW digitalWrite(tdinPin, LOW)
#define TDIN_HIGH digitalWrite(tdinPin, HIGH)
#define TDOUT_STATE digitalRead(tdoutPin)
#define TIRQ_STATE digitalRead(tirqPin)

#endif