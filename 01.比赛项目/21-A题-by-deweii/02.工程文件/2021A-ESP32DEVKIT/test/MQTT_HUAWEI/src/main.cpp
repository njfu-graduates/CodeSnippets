#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <huaweiyun_mqtt.h>

#include <Wire.h>
float AHT10_T, AHT10_RH;

#define LED 2

bool LedState = 0;

int last0 = 0;

unsigned long lastSend = 0;

#define WIFI_SSID "Xiaomi_CD4B"
#define WIFI_PASSWD "ACG12345"

#define server_ip "iot-mqtts.cn-north-4.myhuaweicloud.com"
#define device_id "60a8b40606dc9602c03b27b9_123456qQ"
#define device_secret "9f2f476f9600a308f3b00dd949a37aae"
#define this_time "2021052215"

//属性上报Topic---发布
#define HUAWEI_TOPIC_PROP_POST "$oc/devices/" device_id "/sys/properties/report"
//属性设置Topic---订阅
#define HUAWEI_TOPIC_PROP_SET "$oc/devices/" device_id "/sys/properties/set"
//设备报警信息---发布与订阅
#define HUAWEI_TOPIC_PROP_ERROR "$oc/devices/" device_id "/user/error"

WiFiClient client;
PubSubClient mqttClient(client);

void AHT10Post();
void AHT10_update();
void initWifi(const char *ssid, const char *password);
void mqttCheckConnect();
void mqttIntervalPost();
void callback(char *topic, byte *payload, unsigned int length);

void setup()
{
  Wire.begin();
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW); /*LOW*/

  initWifi(WIFI_SSID, WIFI_PASSWD);

  Serial.println();

  mqttCheckConnect(); //初始化首次链接
  mqttIntervalPost(); //上报初始化数据

  Serial.println();
  mqttClient.setCallback(callback);
  // 回调，监听云端下发指令，当ESP8266收到订阅Topic后调用callback函数
}

void loop()
{
  //每隔1s尝试连接一次云
  if (millis() - lastSend >= 50000)
  {
    mqttCheckConnect();
    lastSend = millis();
  }

  //判断灯现态与次态的关系，减少无意义的上报
  if (last0 != LedState)
  {
    last0 = LedState;
    mqttIntervalPost();
    Serial.println();
  }

  mqttClient.loop();
}

void initWifi(const char *ssid, const char *password)
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi does not connect,try again ..");
    delay(1000);
  }

  Serial.printf("WiFi is connected.\nIP address: ");
  Serial.println(WiFi.localIP());
}

void mqttCheckConnect()
{
  bool connected = connectHuaweiyunMQTT(mqttClient, server_ip, device_id, device_secret, this_time);

  if (connected)
  {
    Serial.println("MQTT connect succeed!");

    mqttClient.subscribe(HUAWEI_TOPIC_PROP_SET);
    Serial.println("subscribe done");
  }
}

void mqttIntervalPost()
{
  DynamicJsonDocument jsonSend(1024);
  DynamicJsonDocument LEDdoc(128), LED_SW(32);

  LED_SW["LED_SW"] = LedState;

  LEDdoc["service_id"] = "LED";
  LEDdoc["properties"] = LED_SW;

  jsonSend["services"][0] = LEDdoc;

  String jsonBuf;
  serializeJson(jsonSend, jsonBuf);

  Serial.println();
  Serial.print(HUAWEI_TOPIC_PROP_POST);
  Serial.println(jsonBuf);

  mqttClient.publish(HUAWEI_TOPIC_PROP_POST, jsonBuf.c_str());
}

void callback(char *topic, byte *payload, unsigned int length)
{

  Serial.println();
  Serial.println();
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.println();

  payload[length] = '\0';
  Serial.println((char *)payload);

  //json解析payload
  StaticJsonDocument<1024> root;
  deserializeJson(root, payload);

  if (root["command_name"] == "LED_SW")
  {
    LedState = root["paras"]["LED_Statu"];
    digitalWrite(LED, LedState);
  }
  if (root["command_name"] == "GET")
  {
    if (root["paras"]["GET"])
    {
      AHT10Post();
    }
  }
}

void AHT10Post()
{
  AHT10_update();
  Serial.printf("updated AHT T:%f  RH:%f\n", AHT10_T, AHT10_RH);

  DynamicJsonDocument jsonSend(1024);
  DynamicJsonDocument AHTdoc(64), AHTdata(48);
  AHTdata["AHT10_T"] = AHT10_T;
  AHTdata["AHT10_RH"] = AHT10_RH;

  AHTdoc["service_id"] = "AHT10";
  AHTdoc["properties"] = AHTdata;

  jsonSend["services"][0] = AHTdoc;

  String jsonBuf;
  serializeJson(jsonSend, jsonBuf);

  Serial.println();
  Serial.print(HUAWEI_TOPIC_PROP_POST);
  Serial.println(jsonBuf);

  mqttClient.publish(HUAWEI_TOPIC_PROP_POST, jsonBuf.c_str());
}

void AHT10_update()
{
  int data[6];
  Wire.beginTransmission(0x38); //选择AHT的i2c位置
  Wire.requestFrom(0x38, 6);    //接受其回复
  while (Wire.available())
  {
    for (int i = 0; i < 6; i++)
    {
      data[i] = Wire.read();
    }

    if ((data[0] & 0x08) == 0) //初始化
    {
      Wire.write(0xE1);
      Wire.write(0x08);
      Wire.write(0x00);
    }
    else //不需要初始化
    {
      Wire.write(0xAC);
      Wire.write(0x33);
      Wire.write(0x00);

      delayMicroseconds(75);

      if ((data[0] & 0x80) == 0) //需要等待完成
      {
      }
      else //不需要等待
      {
      }
    }
  }
  Wire.endTransmission();

  AHT10_RH = ((data[1] << 12) | (data[2] << 4)) | (data[3] >> 4);
  AHT10_RH = (AHT10_RH / pow(2, 20)) * 100; //湿度

  AHT10_T = ((data[3] & 0x0f) << 16) | (data[4] << 8) | data[5];
  AHT10_T = (AHT10_T / pow(2, 20)) * 200 - 50; //温度
}
