uint16_t freq1 = 2000;     // 频率
uint8_t channelA = 0;      // 通道0，共16个通道，0~15
uint16_t resolution1 = 10; // 分辨率，取值0~20，duty最大取值为2^resolution-1

uint16_t freq2 = 2000;     // 频率
uint8_t channelB = 1;      // 通道1，共16个通道，0~15
uint16_t resolution2 = 10; // 分辨率，取值0~20，duty最大取值为2^resolution-1

ledcSetup(channelA, freq1, resolution1); // 设置通道0
ledcAttachPin(PWMA, channelA);           // 将通道0与引脚26连接
ledcSetup(channelB, freq2, resolution2); // 设置通道1
ledcAttachPin(PWMB, channelB);           // 将通道1与引脚25连接
ledcWrite(channelA, abs(speedA));
ledcWrite(channelB, abs(speedB));