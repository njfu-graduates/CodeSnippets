/*
  Aliyun_mqtt.h - Library for connect to Aliyun MQTT server.
*/

#include "huaweiyun_mqtt.h"

#include "SHA256.h"

#define MQTT_PORT 1883
#define SHA256HMAC_SIZE 32

static String mqttBroker;
static String mqttClientID;
static String mqttUserName;
static String mqttPassword;

// Verify tool: http://tool.oschina.net/encrypt?type=2
static String hmac256(const String &signcontent, const String &ds)
{
  byte hashCode[SHA256HMAC_SIZE];
  SHA256 sha256;

  const char *key = ds.c_str();
  size_t keySize = ds.length();  

  sha256.resetHMAC(key, keySize);
  sha256.update((const byte *)signcontent.c_str(), signcontent.length());               
  sha256.finalizeHMAC(key, keySize, hashCode, sizeof(hashCode));

  String sign = "";
  for (byte i = 0; i < SHA256HMAC_SIZE; ++i)
  {
    sign += "0123456789abcdef"[hashCode[i] >> 4];
    sign += "0123456789abcdef"[hashCode[i] & 0xf];
  }

  return sign;
}

bool connectHuaweiyunMQTTWithPassword(PubSubClient &mqttClient, const char *password)
{
  mqttClient.setServer(mqttBroker.c_str(), MQTT_PORT);

  byte mqttConnectTryCnt = 5;
  while (!mqttClient.connected() && mqttConnectTryCnt > 0)
  {
    Serial.println("Connecting to MQTT Server ...");
    if (mqttClient.connect(mqttClientID.c_str(), mqttUserName.c_str(), password))
    {

      Serial.println("MQTT Connected!");
      return true;
    }
    else
    {
      byte errCode = mqttClient.state();
      Serial.print("MQTT connect failed, error code:");
      Serial.println(errCode);
      if (errCode == MQTT_CONNECT_BAD_PROTOCOL || errCode == MQTT_CONNECT_BAD_CLIENT_ID || errCode == MQTT_CONNECT_BAD_CREDENTIALS || errCode == MQTT_CONNECT_UNAUTHORIZED)
      {
        Serial.print("No need to try again.");
        switch (errCode)
        {
        case MQTT_CONNECT_BAD_PROTOCOL:
          Serial.println("MQTT_CONNECT_BAD_PROTOCOL");
          break;
        case MQTT_CONNECT_BAD_CLIENT_ID:
          Serial.println("MQTT_CONNECT_BAD_CLIENT_ID");
          break;
        case MQTT_CONNECT_BAD_CREDENTIALS:
          Serial.println("MQTT_CONNECT_BAD_CREDENTIALS");
          break;
        case MQTT_CONNECT_UNAUTHORIZED:
          Serial.println("MQTT_CONNECT_UNAUTHORIZED");
          break;
        default:
          break;
        }
        break;
      }
      delay(5000);
    }
    mqttConnectTryCnt -= 1;
  }

  return false;
}

bool connectHuaweiyunMQTT(
    PubSubClient &mqttClient,
    const char *serverip,
    const char *device_id,
    const char *devicesecret,
    const char *thistime)
{
  mqttBroker = serverip;
  mqttClientID = String(device_id) + "_0_0_" + String(thistime);
  mqttUserName = device_id;
  mqttPassword = hmac256(devicesecret, String(thistime));

  return connectHuaweiyunMQTTWithPassword(mqttClient, mqttPassword.c_str());
}
