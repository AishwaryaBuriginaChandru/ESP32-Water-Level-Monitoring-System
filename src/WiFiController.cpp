#ifndef _WIFI_CONTROLLER_C_
#define _WIFI_CONTROLLER_C_
#include <Arduino.h>
#include <WiFi.h>
#include "Logger.cpp"
using namespace std;

// https://randomnerdtutorials.com/esp32-wi-fi-manager-asyncwebserver/
// https://randomnerdtutorials.com/esp32-client-server-wi-fi/
// https://dronebotworkshop.com/wifimanager/
class WiFiController
{
public:
    bool isConnected()
    {
        return WiFi.status() == WL_CONNECTED;
    }

    int signalStrength()
    {
        return WiFi.RSSI();
    }

    void initialize(String &wifiNetworkSsid, String &wifiNetworkPassword,
                    String &softAccessPointSsid,
                    String &softAccessPointPassword)
    {
        WiFi.mode(WIFI_AP_STA);                                    // Enable both AP and Station modes
        WiFi.softAP(softAccessPointSsid, softAccessPointPassword); // Start the access point
        IPAddress ip(192, 168, 3, 23);
        IPAddress gateway(192, 168, 3, 1);
        IPAddress subnet(255, 255, 255, 0);
        WiFi.softAPConfig(ip, gateway, subnet);
        Logger::logInfo("Connected to Access Point with IP: ");
        Serial.println(WiFi.softAPIP());

        WiFi.begin(wifiNetworkSsid, wifiNetworkPassword); // Connect to existing network
        Logger::logInfo("Connecting to WiFi Network");
        int attempts = 0;
        while (!isConnected() && attempts < 20)
        {
            delay(1000);
            Serial.print(".");
            attempts++;
        }
        if (isConnected())
        {
            Logger::logInfo("Connected to WiFi Network with IP ");
            Serial.println(WiFi.localIP());
            Logger::logInfo("Received Signal Strength: ");
            Serial.print(WiFi.RSSI());
            Serial.print(" dBm (decibel-milliwatts)");
        }
        else
        {
            Logger::logError("Failed to connect to WiFi Network");
        }
    }
};

#endif