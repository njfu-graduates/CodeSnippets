/*
  Aliyun_mqtt.h - Library for connect to Aliyun MQTT server with authentication by
  product key, device name and device secret.

  https://www.alibabacloud.com/help/product/30520.htm
*/

#ifndef _ALIYUN_MATT_H
#define _ALIYUN_MATT_H

#include <Arduino.h>
#include <PubSubClient.h>

extern "C" bool connectHuaweiyunMQTT(
    PubSubClient &mqttClient,
    const char *serverip,
    const char *device_id,
    const char *devicesecret,
    const char *thistime);

extern "C" bool connectHuaweiyunMQTTWithPassword(PubSubClient &mqttClient, const char *password);

#endif
