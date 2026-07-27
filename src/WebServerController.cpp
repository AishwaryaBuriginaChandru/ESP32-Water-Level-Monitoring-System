#ifndef _WEB_SERVER_CONTROLLER_C_
#define _WEB_SERVER_CONTROLLER_C_

#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include "SPIFFS.h"
#include <ArduinoJson.h>
// #include "ConfigManager.cpp"
#include "Setting.cpp"
#include "WaterLevel.cpp"
class WebServerController
{

public:
    AsyncWebServer instance;
    WebServerController() : instance(80)
    {
    }

    void initialize(String hostName, String defaultPage)
    {
        if (MDNS.begin(hostName))
        {
            Serial.println("DNS Server Started at http://" + hostName + ".local");
        }
        instance.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                    { request->send(SPIFFS, "/index.html", "text/html", false); });
        instance.on("/index", HTTP_GET, [](AsyncWebServerRequest *request)
                    { request->send(SPIFFS, "/index.html", "text/html", false); });
        instance.on("/wifi", HTTP_GET, [](AsyncWebServerRequest *request)
                    { request->send(SPIFFS, "/wifi.html", "text/html", false); });

        instance.serveStatic("/", SPIFFS, "/");
        DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
        DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET,POST,PUT,OPTIONS");
        DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type");
        // Start the server
        instance.begin();
        Serial.println("HTTP server started");
    }

    String getPage(String content)
    {
        String html = "<!DOCTYPE html><html><head>";
        html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
        html += "<title>CBK - IOT</title>";
        html += "<style>";
        html += "body{font-family:Arial,sans-serif;margin:0;padding:20px;background:#f0f2f5;}";
        html += "nav{background:#333;padding:10px;margin-bottom:20px;}";
        html += "nav a{color:white;text-decoration:none;margin-right:15px;}";
        html += ".container{max-width:800px;margin:0 auto;background:white;padding:20px;border-radius:8px;box-shadow:0 2px 4px rgba(0,0,0,0.1);}";
        html += "@keyframes fadeIn{from{opacity:0;}to{opacity:1;}}";
        html += ".animate{animation:fadeIn 1s;}";
        html += "</style></head><body>";
        html += "<nav>";
        html += "<a href='/index'>Dashbaord</a>";
        html += "<a href='/network-ino'>Network Info</a>";
        html += "<a href='/timer'>Timer</a>";
        html += "</nav>";
        html += "<div class='container animate'>";
        html += content;
        html += "</div></body></html>";
        return html;
    }

    void handleNetworkInfo(AsyncWebServerRequest *request)
    {
        if (!isAuthenticated(request))
        {
            return;
        }

        String content = "<h1>Detailed Network Information</h1>";
        content += "<div style='background:#e3f2fd;padding:15px;border-radius:5px;'>";
        content += "<h2>Station Mode (Client)</h2>";
        content += "<p><strong>Status:</strong> ";
        content += (WiFi.status() == WL_CONNECTED) ? "Connected" : "Disconnected";
        content += "</p>";
        content += "<p><strong>IP Address:</strong> ";
        content += (WiFi.status() == WL_CONNECTED) ? WiFi.localIP().toString() : "N/A";
        content += "</p>";
        content += "<p><strong>Subnet Mask:</strong> ";
        content += (WiFi.status() == WL_CONNECTED) ? WiFi.subnetMask().toString() : "N/A";
        content += "</p>";
        content += "<p><strong>Gateway IP:</strong> ";
        content += (WiFi.status() == WL_CONNECTED) ? WiFi.gatewayIP().toString() : "N/A";
        content += "</p>";
        content += "<p><strong>MAC Address:</strong> ";
        content += WiFi.macAddress();
        content += "</p>";
        content += "<p><strong>Connected to SSID:</strong> ";
        content += WiFi.SSID();
        content += "</p>";
        content += "<p><strong>Wifi Signal Strength (dbm):</strong> ";
        content += WiFi.RSSI();
        content += "</p>";
        content += "<h2>Access Point Mode</h2>";
        content += "<p><strong>AP IP:</strong> ";
        content += WiFi.softAPIP().toString();
        content += "</p>";
        content += "<p><strong>AP MAC Address:</strong> ";
        content += WiFi.softAPmacAddress();
        content += "</p>";
        content += "<p><strong>AP SSID:</strong> ";
        content += WiFi.softAPSSID();
        content += "</p>";
        content += "<p><strong>Connected Stations:</strong> ";
        content += WiFi.softAPgetStationNum();
        content += "</p>";
        content += "</div>";
        request->send(200, "text/html", getPage(content));
    }

    void handleNotFound(AsyncWebServerRequest *request)
    {
        String content = "<h1>404: Not Found</h1><p>The requested page was not found.</p>";
        request->send(404, "text/html", getPage(content));
    }

    void handleTimer(AsyncWebServerRequest *request)
    {
        String content = "<h1>Current Time</h1>";
        content += "<div id='clock' style='font-size:48px;text-align:center;'>Loading...</div>";
        content += "<script>";
        content += "function updateClock(){";
        content += "const now=new Date();";
        content += "document.getElementById('clock').textContent=now.toLocaleTimeString();}";
        content += "setInterval(updateClock,1000);updateClock();";
        content += "</script>";
        request->send(200, "text/html", getPage(content));
    }

    bool isAuthenticated(AsyncWebServerRequest *request)
    {
        if (!request->authenticate("USERNAME", "PASSWORD"))
        {
            request->requestAuthentication();
            return false;
        }
        return true;
    }

    void handleWifiDataGetRequest(AsyncWebServerRequest *request, Setting &_setting)
    {
        JsonDocument doc;
        doc["WifiSsid"] = _setting.WifiSsid;
        doc["WifiPassword"] = _setting.WifiPassword;
        String jsonString;
        serializeJson(doc, jsonString);
        request->send(200, "application/json", jsonString);
    }

    void handleSettingGetRequest(AsyncWebServerRequest *request, Setting &_setting)
    {
        JsonDocument doc;

        doc["WifiAccessPointName"] = _setting.WifiAccessPointName;
        doc["WifiAccessPointPassword"] = _setting.WifiAccessPointPassword;
        doc["WifiAccessPointIp"] = _setting.WifiAccessPointIp;
        doc["WifiSsid"] = _setting.WifiSsid;
        doc["WifiPassword"] = _setting.WifiPassword;

        doc["UpperTankEmptyDistance"] = _setting.UpperTankEmptyDistance;
        doc["UpperTankMinTriggerPercent"] = _setting.UpperTankMinTriggerPercent;
        doc["UpperTankMaxTriggerPercent"] = _setting.UpperTankMaxTriggerPercent;

        doc["LowerTankEmptyDistance"] = _setting.LowerTankEmptyDistance;
        doc["LowerTankMinTriggerPercent"] = _setting.LowerTankMinTriggerPercent;
        doc["LowerTankMaxTriggerPercent"] = _setting.LowerTankMaxTriggerPercent;

        doc["Motor1RunHour"] = _setting.Motor1RunHour;
        doc["Motor2RunHour"] = _setting.Motor2RunHour;

        doc["Motor1Auto"] = _setting.Motor1Auto;
        doc["Motor2Auto"] = _setting.Motor2Auto;
        doc["LightAuto"] = _setting.LightAuto;
        doc["UpsAuto"] = _setting.UpsAuto;
        doc["PageRefreshSeconds"] = _setting.PageRefreshSeconds;

        String jsonString;
        serializeJson(doc, jsonString);
        request->send(200, "application/json", jsonString);
    }

    // void handleSettingPostRequest(AsyncWebServerRequest *request, Setting &_setting)
    // {
    //     JsonDocument doc;

    //     _setting.WifiAccessPointName = doc["WifiAccessPointName"].as<String>();
    //     _setting.WifiAccessPointPassword = doc["WifiAccessPointPassword"].as<String>();
    //     _setting.WifiAccessPointIp = doc["WifiAccessPointIp"].as<String>();
    //     _setting.WifiSsid = doc["WifiSsid"].as<String>();
    //     _setting.WifiPassword = doc["WifiPassword"].as<String>();

    //     _setting.UpperTankEmptyDistance = doc["UpperTankEmptyDistance"];
    //     _setting.UpperTankMinTriggerPercent = doc["UpperTankMinTriggerPercent"];
    //     _setting.UpperTankMaxTriggerPercent = doc["UpperTankMaxTriggerPercent"];

    //     _setting.LowerTankEmptyDistance = doc["LowerTankEmptyDistance"];
    //     _setting.LowerTankMinTriggerPercent = doc["LowerTankMinTriggerPercent"];
    //     _setting.LowerTankMaxTriggerPercent = doc["LowerTankMaxTriggerPercent"];

    //     _setting.Motor1RunHour = doc["Motor1RunHour"];
    //     _setting.Motor2RunHour = doc["Motor2RunHour"];
    //     _setting.Motor1Auto = doc["Motor1Auto"];
    //     _setting.Motor2Auto = doc["Motor2Auto"];
    //     _setting.LightAuto = doc["LightAuto"];
    //     _setting.UpsAuto = doc["UpsAuto"];

    //     _setting.PageRefreshSeconds = doc["PageRefreshSeconds"];
    //     String jsonString;
    //     serializeJson(doc, jsonString);
    //     request->send(200, "application/json", jsonString);
    // }
};

#endif
