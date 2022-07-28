#include <Arduino.h>
#include <painlessMesh.h>
#include "DHT.h"
#include "..\..\mesh_root\src\Package.h"

#define DHTTYPE DHT11
#define DHTPIN 2
DHT dht(DHTPIN, DHTTYPE, 11); // 11 works fine for ESP8266

MyJson pkg(NODE_TYPE_DHT11);
DynamicJsonDocument JDoc(128);

Scheduler userScheduler;
painlessMesh mesh;

void sendValue(uint32_t from);
void sendFeedback(uint32_t from);
void receivedCallback(uint32_t from, String &msg);

void setup()
{
  Serial.begin(115200);
  dht.begin();

  mesh.setDebugMsgTypes(ERROR | DEBUG);
  mesh.setContainsRoot();
  mesh.init(MESH_SSID, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onReceive(&receivedCallback);
}

void loop()
{
  mesh.update();
}

void receivedCallback(uint32_t from, String &msg)
{
#if _DEBUG == 1
  Serial.printf("%s", msg.c_str());
#endif

  DynamicJsonDocument doc(256);
  deserializeJson(doc, msg);
  commandType cmd = doc["cmd"];

  switch (cmd)
  {
  case COMMAND_FIND:
  {
    sendFeedback(from);
    break;
  }

  case COMMAND_GET_TEMPER:
  {
    sendValue(from);
    break;
  }

  case COMMAND_RESTART:
  {
    ESP.restart();
    break;
  }

  default:
    break;
  }
}

void sendValue(uint32_t from)
{
  pkg.cmd = COMMAND_RETURN_DHT;
  JsonObject msg = JDoc.to<JsonObject>();

  float h, t, hic;
  h = dht.readHumidity();
  t = dht.readTemperature();
  if (isnan(h) || isnan(t))
  {
    msg["text"] = "DHT ERROR:fail to read!";
  }
  else
  {
    hic = dht.computeHeatIndex(t, h, false);
    msg["Humidity"] = h;
    msg["Temperature"] = t;
    msg["Heat index"] = hic;
  }
  pkg.msg = msg;

  mesh.sendSingle(from, pkg.toString());

#if _DEBUG == 1
  serializeJson(msg, Serial);
  Serial.printf("\n");
#endif
}

void sendFeedback(uint32_t from)
{
  pkg.cmd = COMMAND_FEEDBACK;
  JsonObject msg = JDoc.to<JsonObject>();
  msg["text"] = "Feedback from dht11";
  pkg.msg = msg;

  mesh.sendSingle(from, pkg.toString());

#if _DEBUG == 1
  serializeJson(msg, Serial);
  Serial.printf("\n");
#endif
}