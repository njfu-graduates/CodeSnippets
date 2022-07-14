#include <Arduino.h>
#include <painlessMesh.h>
#include "..\..\mesh_root\src\Package.h"

MyJson pkg(NODE_TYPE_HCSR04);
DynamicJsonDocument JDoc(128);

Scheduler userScheduler; //nothing to add
painlessMesh mesh;

//something to do with hc-sr04
const int trigPin = 2;
const int echoPin = 0;
double duration, cm;
double sensor_cm();

void sendValue(uint32_t from);
void sendFeedback(uint32_t from);
void receivedCallback(uint32_t from, String &msg);

void setup()
{
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  mesh.setDebugMsgTypes(ERROR | DEBUG);
  mesh.setContainsRoot();
  mesh.init(MESH_SSID, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onReceive(&receivedCallback);
}

void loop()
{
  mesh.update();
  Serial.println(sensor_cm());
}

void receivedCallback(uint32_t from, String &msg)
{
#if _DEBUG == 1
  Serial.printf("%s\n", msg.c_str());
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

  case COMMAND_GET_VALUE:
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
  pkg.cmd = COMMAND_RETURN_HCSR04;
  JsonObject msg = JDoc.to<JsonObject>();
  msg["value"] = sensor_cm();
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
  msg["text"] = "Feedback from hcsr04";
  pkg.msg = msg;

  mesh.sendSingle(from, pkg.toString());

#if _DEBUG == 1
  serializeJson(msg, Serial);
  Serial.printf("\n");
#endif
}

double sensor_cm()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  cm = (duration / 2) * 0.0341;
  return (cm > 2 && cm < 600) ? cm : 0;
}