#ifndef _CONFIG_MANAGER_C_
#define _CONFIG_MANAGER_C_

#include <Arduino.h>
#include <ArduinoJson.h>
#include "SPIFFS.h"
#include "Logger.cpp"
#include "Setting.cpp"

class ConfigManager
{
public:
    void initialize()
    {
        if (!SPIFFS.begin(true))
        {
            Serial.println("An error has occurred while mounting SPIFFS");
        }
        Serial.println("LittleFS mounted successfully");
    }

    static Setting readConfig(String fileName = "/appSetting.json")
    {
        Setting setting;

        File file = SPIFFS.open(fileName);
        if (!file)
        {
            Serial.println("Failed to open file for reading");
            return setting;
        }
        while (file.available())
        {
            String content = file.readString();
            Serial.println(content);
            JsonDocument doc;
            deserializeJson(doc, content);
            // Serial.println("Doc Size::" + String(doc.size()));
            setting.WifiAccessPointName = doc["WifiAccessPointName"].as<String>();
            setting.WifiAccessPointPassword = doc["WifiAccessPointPassword"].as<String>();
            setting.WifiAccessPointIp = doc["WifiAccessPointIp"].as<String>();

            setting.WifiSsid = doc["WifiSsid"].as<String>();
            setting.WifiPassword = doc["WifiPassword"].as<String>();

            setting.UpperTankEmptyDistance = doc["UpperTankEmptyDistance"];
            setting.UpperTankMinTriggerPercent = doc["UpperTankMinTriggerPercent"];
            setting.UpperTankMaxTriggerPercent = doc["UpperTankMaxTriggerPercent"];

            setting.LowerTankEmptyDistance = doc["LowerTankEmptyDistance"];
            setting.LowerTankMinTriggerPercent = doc["LowerTankMinTriggerPercent"];
            setting.LowerTankMaxTriggerPercent = doc["LowerTankMaxTriggerPercent"];

            setting.Motor1RunHour = doc["Motor1RunHour"];
            setting.Motor2RunHour = doc["Motor2RunHour"];
            setting.Motor1Auto = doc["Motor1Auto"];
            setting.Motor2Auto = doc["Motor2Auto"];
            setting.LightAuto = doc["LightAuto"];
            setting.UpsAuto = doc["UpsAuto"];

            setting.PageRefreshSeconds = doc["PageRefreshSeconds"];
        }
        file.close();
        return setting;
    }

    static void saveConfig(Setting setting, String fileName = "/appSetting.json")
    {
        JsonDocument doc;

        doc["WifiAccessPointName"] = setting.WifiAccessPointName;
        doc["WifiAccessPointPassword"] = setting.WifiAccessPointPassword;
        doc["WifiAccessPointIp"] = setting.WifiAccessPointIp;
        doc["WifiSsid"] = setting.WifiSsid;
        doc["WifiPassword"] = setting.WifiPassword;

        doc["UpperTankEmptyDistance"] = setting.UpperTankEmptyDistance;
        doc["UpperTankMinTriggerPercent"] = setting.UpperTankMinTriggerPercent;
        doc["UpperTankMaxTriggerPercent"] = setting.UpperTankMaxTriggerPercent;

        doc["LowerTankEmptyDistance"] = setting.LowerTankEmptyDistance;
        doc["LowerTankMinTriggerPercent"] = setting.LowerTankMinTriggerPercent;
        doc["LowerTankMaxTriggerPercent"] = setting.LowerTankMaxTriggerPercent;

        doc["Motor1RunHour"] = setting.Motor1RunHour;
        doc["Motor2RunHour"] = setting.Motor2RunHour;
        doc["Motor1Auto"] = setting.Motor1Auto;
        doc["Motor2Auto"] = setting.Motor2Auto;
        doc["LightAuto"] = setting.LightAuto;
        doc["UpsAuto"] = setting.UpsAuto;

        doc["PageRefreshSeconds"] = setting.PageRefreshSeconds;

        String content;
        serializeJson(doc, content);
        File file = SPIFFS.open(fileName, "w");
        if (file.print(content))
        {
            Serial.println("File was written with below Content");
            Serial.println(content);
        }
        else
        {
            Serial.println("File write failed");
        }
        file.close();
    }

    static void eraceConfig(String fileName = "/appSetting.json")
    {
        JsonDocument doc;
        doc["WifiAccessPointName"] = "";
        doc["WifiAccessPointIp"] = "";
        doc["WifiSsid"] = "";
        doc["WifiPassword"] = "";
        doc["UpperTankEmptyDistance"] = 0;
        doc["LowerTankEmptyDistance"] = 0;
        doc["Motor1RunHour"] = 0;
        doc["Motor2RunHour"] = 0;
        doc["Motor1Auto"] = false;
        doc["Motor2Auto"] = false;
        doc["LightAuto"] = false;
        doc["UpsAuto"] = false;

        String content;
        serializeJson(doc, content);
        File file = SPIFFS.open(fileName, "w");
        if (file.print(content))
        {
            Serial.println("File was written");
        }
        else
        {
            Serial.println("File write failed");
        }
        file.close();
    }
};

#endif