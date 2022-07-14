/*
*功能：ESP8266连接阿里云服务，并简单的点个灯！
*作者：刘泽文
*时间：2020/4/10
*/
 
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <huaweiyun_mqtt.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <SHA256.h>
 
//定义OLED型号
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
 
//定义LED灯
#define LED 2

//初始化次态
bool LedState = 0;
 
//初始化现态
int last0 = 0;
 
//上云时间现态
unsigned long lastSend = 0;
 
//你的wifi
#define WIFI_SSID "liuzewen"
#define WIFI_PASSWD "17609245102liu"

//华为云上云条件
#define server_ip "iot-mqtts.cn-north-4.myhuaweicloud.com"
#define device_id "5f98f71d21284902d049ca58_watch001"
#define device_secret "45534412b07f5697052ae135b9266a01"
#define this_time "2021020403"

//属性上报Topic---发布
#define HUAWEI_TOPIC_PROP_POST "$oc/devices/" device_id "/sys/properties/report"
//属性设置Topic---订阅
#define HUAWEI_TOPIC_PROP_SET "$oc/devices/" device_id "/sys/properties/set"
//设备报警信息---发布与订阅
#define HUAWEI_TOPIC_PROP_ERROR "$oc/devices/" device_id "/user/error"

//创建WiFiClient实例
WiFiClient espClient;
//创建MqttClient实例
PubSubClient mqttClient(espClient);

//连接Wifi
void initWifi(const char *ssid, const char *password)
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi does not connect, try again ...");
    delay(1000);
  }
 
  Serial.println("Wifi is connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
 
 
//连接Mqtt并订阅属性设置Topic
void mqttCheckConnect()
{
  bool connected = connectHuaweiyunMQTT(mqttClient,server_ip,device_id,device_secret,this_time);

  if (connected)
  {
    Serial.println("MQTT connect succeed!");
    //订阅属性设置Topic
    mqttClient.subscribe(HUAWEI_TOPIC_PROP_SET); 
    Serial.println("subscribe done");
  }
}

// 上报属性Topic数据
void mqttIntervalPost() 
{
  String jsonBuf = "{\"services\":[{\"service_id\":\"MPU6050\",\"properties\":{\"AngleX\":0,\"AngleY\":0,\"AngleZ\":1}},{\"service_id\":\"DHT30\",\"properties\":{\"Temp\":12.34,\"Hum\":33.33}}]}";
  Serial.println("");
  Serial.print(HUAWEI_TOPIC_PROP_POST);
  Serial.println(jsonBuf);
  mqttClient.publish(HUAWEI_TOPIC_PROP_POST, jsonBuf.c_str()); 
}

void u8g2_faution(bool state)
{
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_wqy16_t_gb2312);
  u8g2.setCursor(0, 16);
  u8g2.print("   华为云测试   ");//一行共8个汉字
  u8g2.setCursor(10, 55);
  u8g2.print("灯1状态：");
  u8g2.setFont(u8g2_font_open_iconic_all_4x_t);
  if(state)
    u8g2.drawGlyph(85,64,120);//开灯
  else
    u8g2.drawGlyph(85,64,121);//关灯
  u8g2.sendBuffer();
}
 
//监听云端下发指令并处理 
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
  StaticJsonBuffer<512> jsonBuffer;
  JsonObject &root = jsonBuffer.parseObject(payload);

  //判断json解析是否成功
  if (root.success()){

    if(root["command_name"]=="LED_Power")
    {
      LedState = root["paras"]["LED_Statu"];
      u8g2_faution(LedState);
      digitalWrite(LED, !LedState);
    }
    else if(root["command_name"]=="LED_RGB")
    {

    }
  }

}
 
void setup() 
{
  Serial.begin(115200);

  u8g2.begin();
  u8g2.enableUTF8Print();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_wqy16_t_gb2312);
  u8g2.setCursor(40, 40);
  u8g2.print("华为云");
  u8g2.sendBuffer();

  Serial.print("\n\n\n");
  Serial.print("固件编译时间：");
  Serial.print(__DATE__);
  Serial.print("   ");
  Serial.println(__TIME__);

  pinMode(LED, OUTPUT); 
  digitalWrite(LED, HIGH); /*LOW*/

  initWifi(WIFI_SSID, WIFI_PASSWD); 

  Serial.println();

  mqttCheckConnect(); //初始化首次链接
  mqttIntervalPost(); //上报初始化数据

  Serial.println();
  mqttClient.setCallback(callback); // 回调，监听云端下发指令，当ESP8266收到订阅Topic后调用callback函数
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
 