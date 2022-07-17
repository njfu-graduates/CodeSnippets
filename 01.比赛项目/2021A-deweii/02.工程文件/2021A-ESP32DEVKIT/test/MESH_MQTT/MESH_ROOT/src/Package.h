#ifndef __PACKAGE_H__
#define __PACKAGE_H__

#include <bits/stdc++.h>
#include "painlessmesh/plugin.hpp"

using namespace painlessmesh;
using namespace std;

// #define JSON_PACKAGE 1 //wether using JSON message
#define _DEBUG 0

#define MESH_SSID "whateverYouLike"
#define MESH_PASSWORD "somethingSneaky"
#define MESH_PORT 5555

/*
node_type - name / type
     command - cmd
     message - msg
 - DHT - text \ Humidity \ Temperature \ Heat index
 - HC-SR04 - value

*/

typedef enum
{
    NODE_TYPE_ROOT,
    NODE_TYPE_DHT11,
    NODE_TYPE_HCSR04,
    NODE_TYPE_DEVKIT,
    NODE_TYPE_PHONE,
    NODE_END
} nodeType;

typedef enum
{
    COMMAND_FIND,
    COMMAND_GET,
    COMMAND_GET_TEMPER,
    COMMAND_GET_VALUE,
    COMMAND_GET_DEVKIT,
    COMMAND_RETURN,
    COMMAND_RETURN_DHT,
    COMMAND_RETURN_HCSR04,
    COMMAND_RETURN_DEVKIT,
    COMMAND_FEEDBACK,
    COMMAND_PHONE_GET_DATA,
    COMMAND_PHONE_GET_STRUCT,
    COMMAND_PHONE_GET_DELAY,
    COMMAND_PHONE_UPDATE,
    COMMAND_PHONE_CONTROL,
    COMMAND_ALLDATA,
    COMMAND_UPDATE,
    COMMAND_RESTART,
    COMMAND_RESTART_ALL,
    COMMAND_SHUTDOWN,
    COMMAND_END
} commandType;

class MyPkg : public plugin::BroadcastPackage
{
public:
    nodeType type;
    commandType cmd;
    JsonObject msg;

    MyPkg() : plugin::BroadcastPackage(31) {}

    MyPkg(DynamicJsonDocument doc) : plugin::BroadcastPackage(doc.to<JsonObject>())
    {
        type = doc["type"];
        cmd = doc["cmd"];
        msg = doc["msg"];
    }

    JsonObject addTo(JsonObject &&jsonObj) const
    {
        jsonObj = plugin::BroadcastPackage::addTo(std::move(jsonObj));
        jsonObj["type"] = type;
        jsonObj["cmd"] = cmd;
        jsonObj["msg"] = msg;
        return jsonObj;
    }

    size_t jsonObjectSize() const
    {
        return JSON_OBJECT_SIZE(noJsonFields + 2) + round(8 + 1.1 * msg.size());
    }
};

class MyJson
{
public:
    nodeType type;
    commandType cmd;
    JsonObject msg;

public:
    MyJson() {}
    MyJson(nodeType type) : type(type), cmd(COMMAND_END) {}
    MyJson(nodeType type, commandType cmd, JsonObject msg) : type(type), cmd(cmd), msg(msg) {}
    ~MyJson() {}
    MyJson &operator=(MyJson &json)
    {
        this->type = json.type;
        this->cmd = json.cmd;
        this->msg = json.msg;
        return *this;
    }
    String toString() const
    {
        DynamicJsonDocument doc(256);
        doc["type"] = type;
        doc["cmd"] = cmd;
        doc["msg"] = msg;

        String str;
        serializeJson(doc, str);
        return str;
    }
    const char *toCharArray() const
    {
        return toString().c_str();
    }
};

#endif
