#include <Arduino.h>
#include <painlessMesh.h>
#include <U8g2lib.h>
#include "Package.h"

#define LED 25

MyJson pkg(NODE_TYPE_ROOT);
DynamicJsonDocument JDoc(128);  //send message
DynamicJsonDocument JPho(1024); //report message

uint32_t NODE_TO_SEND;
commandType COMMAND_TO_SEND;
multimap<uint32_t, uint16_t> TO_SEND_MAP;
std::map<uint32_t, uint8_t> DELAY_MAP;

uint8_t interval_count = 100;
const uint8_t interval = 60;

Scheduler userScheduler; // to control your personal task
painlessMesh mesh;

bool calc_delay = false;
SimpleList<uint32_t> nodes;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, 16, 15, 4);
char screenbuffer[256];
char row = 0, col = 0; // 64\128
bool OLED_FLAG = false;

void OLED_Init();
void OLED_Disp();
Task taskScreenDisplay(TASK_MILLISECOND * 250, TASK_FOREVER, &OLED_Disp);

void sendMessage();
Task taskSendMessage(TASK_SECOND * 1, TASK_FOREVER, &sendMessage);

void TO_SEND_STRUCT();
void sendCalcDelay();

void handleMQTTCall();
void handleReturn(uint32_t from, JsonObject &msg);

void receivedCallback(uint32_t from, String &msg);
void newConnectionCallback(uint32_t nodeId);
void changedConnectionCallback();
void nodeTimeAdjustedCallback(int32_t offset);
void delayReceivedCallback(uint32_t from, int32_t delay);

void setup()
{
  Serial.begin(115200, SERIAL_8N1);
  OLED_Init();

  mesh.setDebugMsgTypes(ERROR | DEBUG); // set before init() so that you can see error messages

  mesh.init(MESH_SSID, MESH_PASSWORD, &userScheduler, MESH_PORT);

  mesh.setRoot();
  mesh.setContainsRoot();
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  mesh.onNodeDelayReceived(&delayReceivedCallback);

  userScheduler.addTask(taskSendMessage);
  taskSendMessage.enable();
  userScheduler.addTask(taskScreenDisplay);
  taskScreenDisplay.enable();

  randomSeed(analogRead(A0));
}

void loop()
{
  mesh.update();
  if (Serial.available())
    handleMQTTCall();
}

void sendMessage()
{
  JsonObject msg = JDoc.to<JsonObject>();
  if (TO_SEND_MAP.empty()) //if the map is empty do find
  {
    interval_count++;
    if (interval_count > interval)
    {
      interval_count = 0;
      pkg.cmd = COMMAND_FIND;
      msg["text"] = "HELLO";
      pkg.msg = msg;
      mesh.sendBroadcast(pkg.toString());
    }
    else
      return;
  }
  else //send the command in the map
  {
    NODE_TO_SEND = TO_SEND_MAP.begin()->first;
    COMMAND_TO_SEND = (commandType)TO_SEND_MAP.begin()->second;
    TO_SEND_MAP.erase(TO_SEND_MAP.begin());

    pkg.cmd = COMMAND_TO_SEND;
    switch (COMMAND_TO_SEND)
    {
    case COMMAND_GET_TEMPER:
    {
      msg["text"] = "_GET_TEMPER";
      break;
    }
    case COMMAND_GET_VALUE:
    {
      msg["text"] = "_GET_VALUE";
      break;
    }
    default:
    {
      break;
    }
    }
    pkg.msg = msg;
    mesh.sendSingle(NODE_TO_SEND, pkg.toString());
    sendCalcDelay();

    const char *text = msg["text"];
#if _DEBUG == 1
    Serial.printf("%s\n", text);
#endif
    sprintf(screenbuffer, "%s", text);
    OLED_FLAG = true;
  }
}

void receivedCallback(uint32_t from, String &msg)
{
  StaticJsonDocument<256> doc;
  deserializeJson(doc, msg);
  nodeType type = doc["type"];
  commandType cmd = doc["cmd"];
  JsonObject _msg = doc["msg"];

  // switch (cmd)
  // {
  // case COMMAND_FEEDBACK:
  // {
  //   handleFeedback(from, type);
  //   break;
  // }
  // case COMMAND_RETURN_DHT:
  // {
  //   handleDHT11Return(from, _msg);
  //   break;
  // }
  // case COMMAND_RETURN_HCSR04:
  // {
  //   handleHCSR04Return(from, _msg);
  //   break;
  // }
  // case COMMAND_RETURN_DEVKIT:
  // {
  //   handleDEVKITReturn(from, _msg);
  //   break;
  // }
  // default:
  //   break;
  // }

  if (cmd == COMMAND_FEEDBACK)
  {
    TO_SEND_MAP.insert(make_pair(from, COMMAND_GET));
  }
  else
  {
    handleReturn(from, _msg);
  }

#if _DEBUG == 1
  Serial.printf("%u->%s\n", from, msg.c_str());
#endif
}

void handleMQTTCall()
{
  while (Serial.available())
  {
    String str = Serial.readString();
  }
}

void handleReturn(uint32_t from, JsonObject &msg)
{
  

}

//------------------------------------------------//

//------------------------------------------------//
//---------- no need to change -------------------//
//------------------------------------------------//

void newConnectionCallback(uint32_t nodeId)
{
#if _DEBUG == 1
  Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
  Serial.printf("--> startHere: New Connection, %s\n", mesh.subConnectionJson(true).c_str());
#endif
  sprintf(screenbuffer, "New Connection");
  OLED_FLAG = true;
}

void changedConnectionCallback()
{
#if _DEBUG == 1
  Serial.printf("Changed connections\n");
  nodes = mesh.getNodeList();
  Serial.printf("Num nodes: %d\n", nodes.size());
  Serial.printf("Connection list:");
  for (auto node : nodes)
  {
    Serial.printf(" %u", node);
  }
  Serial.println();
#endif

  calc_delay = true;

  nodes = mesh.getNodeList();
  sprintf(screenbuffer, "Num nodes: %d", nodes.size());
  OLED_FLAG = true;
}

void nodeTimeAdjustedCallback(int32_t offset)
{
#if _DEBUG == 1
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
#endif
}

void delayReceivedCallback(uint32_t from, int32_t delay)
{
#if _DEBUG == 1
  Serial.printf("Delay to node %u is %d us\n", from, delay);
#endif
  DELAY_MAP.insert(make_pair(from, delay));
  sprintf(screenbuffer, "Delay %d us\n", delay);
  OLED_FLAG = true;
}

void OLED_Init()
{
  u8g2.begin();
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
  u8g2.clearBuffer();
}

void OLED_Disp()
{
  if (!OLED_FLAG)
    return;
  else
  {
    OLED_FLAG = false;
    col = 0;
  }
  if (row >= 60)
  {
    row = 0;
    u8g2.clearBuffer();
  }
  u8g2.drawStr(col, row, screenbuffer);
  u8g2.sendBuffer();

  row += 10;
}

void TO_SEND_STRUCT()
{
  Serial.println("----TO_SEND_STRUCT----");
  for (auto i : TO_SEND_MAP)
  {
    Serial.printf("%u\t%d\n", i.first, i.second);
  }
  Serial.println("----------------------");
}

void sendCalcDelay()
{
  if (calc_delay)
  {
    SimpleList<uint32_t>::iterator node = nodes.begin();
    while (node != nodes.end())
    {
      mesh.startDelayMeas(*node);
      node++;
    }
    calc_delay = false;
  }
}

void handlePhoneCall()
{
  switch (Serial.read())
  {
  case COMMAND_PHONE_GET_DATA:
  {
    serializeJsonPretty(JPho, Serial);
    Serial.println();
    break;
  }
  case COMMAND_PHONE_GET_STRUCT:
  {
    Serial.println(mesh.subConnectionJson(true));
    break;
  }
  case COMMAND_PHONE_GET_DELAY:
  {
    for (auto i : DELAY_MAP)
    {
      Serial.printf("Delay to node %u is %.2f ms\n", i.first, i.second / 1000.0);
    }
    break;
  }
  case COMMAND_PHONE_UPDATE:
  {
    interval_count = interval + 1;
    Serial.println("UPDATED!");
    break;
  }
  case COMMAND_PHONE_CONTROL:
  {
    break;
  }
  case 0xFF:
  {
    pkg.cmd = COMMAND_RESTART;
    mesh.sendBroadcast(pkg.toString());

    ESP.restart();
    break;
  }

  default:
    break;
  }
}
