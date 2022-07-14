# Arduino - 华为云物联网平台MQTT连接库

## 简介

为简化使用Arduino Framwork的开发者连接华为云物联网平台时的工作，本项目封装了开源的
[PubSubClient库](https://github.com/knolleary/pubsubclient)和
[Arduino Cryptography Library](https://github.com/rweather/arduinolibs)代码库，
帮助开发者完成连接华为云物联网平台需要的参数设置以及准备工作。开发者只要提供从阿里云物联网平台上获得的
（server_ip, device_id, device_secret）就可以迅速使用支持Arduino Framework的开发板完
成MQTT连云和数据上报。

关于本库实现的说明：

1. 本代码库使用HMAC256算法完成连接时的签名验证。
2. 本代码库并不进一步封装`PubSubClient`库本身的接口，使用者可以仍然以熟悉的方式使用`PubSubClient`库接口，
   这个库仅仅帮助开发者完成连接华为云MQTT Broker的繁琐部分。
3. 默认使用阿里云华东2区作为连接region，使用其他region的用户调用时可以显式指定region名。

目前已验证平台和硬件包括:

* Espressif 8266: Node MCU v3.

其他经验证的平台和开发板信息可以逐步更新到上面列表中。