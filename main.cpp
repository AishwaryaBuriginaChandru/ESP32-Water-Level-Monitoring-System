// Add Below for brownout detector was triggere
// #include "soc/soc.h"
// #include "soc/rtc_cntl_reg.h"

#define WIFI_ENABLED true
#define WEB_SERVER_ENABLED true
#define WATER_LEVEL_CONTROLLER_ENABLED true
#define BLUDETOOTH_ENABLED false
#define CURRENT_SENSOR_ENABLED false
#define VOLTAGE_SENSOR_ENABLED false
#define REAL_TIME_CLOCK_ENABLED false

using namespace std;

#include <Arduino.h>
#include "Logger.cpp"
#include "DisplayController.cpp"
DisplayController _displayController;

#if WIFI_ENABLED

#include "WiFiController.cpp"
WiFiController _wifi;

#endif

#if BLUDETOOTH_ENABLED

// #include "BluetoothController.cpp"
#include <BluetoothSerial.h>
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
// BluetoothController _bt;
BluetoothSerial _bt;
// https://randomnerdtutorials.com/esp-now-esp32-arduino-ide/

#endif

#if CURRENT_SENSOR_ENABLED

#define CURRENT_SENS1_IN_PIN 32 // ADC1
#define CURRENT_SENS2_IN_PIN 33 // ADC1
#include "CurrentSensor.cpp"
CurrentSensor _currentSensor;

#endif

#if VOLTAGE_SENSOR_ENABLED

#define VOLTAGE_SENS1_IN_PIN 34 // ADC1 Input Only
#define VOLTAGE_SENS2_IN_PIN 35 // ADC1 Input Only

#endif

#if REAL_TIME_CLOCK_ENABLED

#include "RtcController.cpp"
#define RTC_OLED_SDA_PIN 21 // 5 - For Onboard LED kit
#define RTC_OLED_SCL_PIN 22 // 4 - For Onboard LED kit
RtcController _rtcController;

#endif

#if WATER_LEVEL_CONTROLLER_ENABLED

#include "WaterLevelController.cpp"
#include "MotorController.cpp"

#define ULTRA_SENS1_TRIGGER_PIN 26
#define ULTRA_SENS1_ECHO_PIN 25

#define ULTRA_SENS2_TRIGGER_PIN 27
#define ULTRA_SENS2_ECHO_PIN 14

#define MOTOR1_OUT_PIN 16
#define MOTOR2_OUT_PIN 17
#define TIMER1_OUT_PIN 18
#define TIMER2_OUT_PIN 19

WaterLevelController _upperWlc;
WaterLevelController _lowerWlc;
MotorController _motorController;
WaterLevel _upperWaterLevel;
WaterLevel _lowerWaterLevel;

#endif

#include "ConfigManager.cpp"
ConfigManager _configManager;
Setting _setting;

#define BUZZER_PIN 4
#define LED_PIN 2

String _currentDateTime = __DATE__ __TIME__;

#if WEB_SERVER_ENABLED

#include "WebServerController.cpp"
WebServerController _webServer;
void handleDashboardDataGetRequest(AsyncWebServerRequest *request);
void handleWifiDataPostRequest(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
void handleRestartPostRequest(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
void handleSettingPostRequest(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);

#endif

void setup()
{
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    Logger::initLogger(115200);
    Logger::logInfo("Display Starting...");
    _displayController.initialize();
    Logger::logInfo("Display Started");
    _configManager.initialize();
    Logger::logInfo("Config Manager Initialized");
    _setting = _configManager.readConfig();
#if REAL_TIME_CLOCK_ENABLED
    _rtcController.initialize(RTC_OLED_SDA_PIN, RTC_OLED_SCL_PIN);
#endif

#if WIFI_ENABLED
    _wifi.initialize(_setting.WifiSsid, _setting.WifiPassword, _setting.WifiAccessPointName, _setting.WifiAccessPointPassword);
    // WiFi.disconnect(true, true);
#endif

#if WEB_SERVER_ENABLED

    // API Handlers
    _webServer.instance.on("/api/setting", HTTP_GET, [](AsyncWebServerRequest *request)
                           { _webServer.handleSettingGetRequest(request, _setting); });
    _webServer.instance.on("/api/setting", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, handleSettingPostRequest);

    _webServer.instance.on("/api/wifi-setting", HTTP_GET, [](AsyncWebServerRequest *request)
                           { _webServer.handleWifiDataGetRequest(request, _setting); });
    _webServer.instance.on("/api/wifi-setting", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, handleWifiDataPostRequest);
    _webServer.instance.on("/api/restart", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, handleRestartPostRequest);
    _webServer.instance.on("/api/dashboard-data", HTTP_GET, handleDashboardDataGetRequest);

    // Page Handlers
    _webServer.instance.on("/home", HTTP_GET, [](AsyncWebServerRequest *request)
                           { request->send(SPIFFS, "/home.html", "text/html", false); });
    _webServer.instance.on("/network-ino", HTTP_GET, [](AsyncWebServerRequest *request)
                           { _webServer.handleNetworkInfo(request); });
    _webServer.instance.on("/timer", HTTP_GET, [](AsyncWebServerRequest *request)
                           { _webServer.handleTimer(request); });

    _webServer.initialize("WaterLevelMonitor", "/index.html");

#endif

#if BLUDETOOTH_ENABLED
    Logger::logInfo("Bluetooth Starting");
    try
    {
        // _bt.initialize("ESP32-WATER-MONITOR");
        // _bt.discoverDevices();
        _bt.begin("ESP32-WATER-MONITOR");
        Logger::logInfo("Bluetooth Started");
    }
    catch (const std::exception &e)
    {
        Logger::logError("Error initializing Bluetooth: " + String(e.what()));
    }

#endif

#if WATER_LEVEL_CONTROLLER_ENABLED
    _upperWlc.initialize(ULTRA_SENS1_TRIGGER_PIN, ULTRA_SENS1_ECHO_PIN, 150, 130, 20, 100, false);
    _lowerWlc.initialize(ULTRA_SENS2_TRIGGER_PIN, ULTRA_SENS2_ECHO_PIN, 150, 130, 20, 100, false);
    _motorController.initialize(MOTOR1_OUT_PIN, MOTOR2_OUT_PIN);
#endif

#if CURRENT_SENSOR_ENABLED

    _currentSensor.initialize(CURRENT_SENS1_IN_PIN);

#endif
}

void loop()
{
    // digitalWrite(LED_PIN, HIGH);

    // // BUGGER - START
    // digitalWrite(BUZZER_PIN, HIGH);
    // delay(5000);
    // digitalWrite(BUZZER_PIN, LOW);
    // // BUGGER - END

    // _displayController.showDemo("ESP32");
    // _displayController.showInfo("DONE");
    // Logger::logInfo("DONE");
    digitalWrite(LED_PIN, LOW);
    if (_wifi.isConnected())
    {
        digitalWrite(LED_PIN, HIGH);
        // TODO: Call Web elated Tasks
    }
#if REAL_TIME_CLOCK_ENABLED
    DateTime currentDate = _rtcController.getCurrentDateTime();
    _currentDateTime = _rtcController.ToDateTimeString(currentDate);
    Logger::logInfo("Time: " + _currentDateTime);
#endif

#if BLUDETOOTH_ENABLED
    if (_bt.available())
    {
        Logger::logInfo(_bt.readString());
        if (_bt.availableForWrite())
        {
            String a = "page1ok";
            uint8_t buf[a.length()];
            memcpy(buf, a.c_str(), a.length());
            _bt.write(buf, a.length());
        }
    }

#endif

#if WATER_LEVEL_CONTROLLER_ENABLED

    _upperWaterLevel = _upperWlc.checkLevel();
    // Logger::logInfo("UPPER: " + String(_upperWaterLevel.Distance) + "::" + String(_upperWaterLevel.OutputEnabled) + "::" + String(_upperWaterLevel.LevelPercent) + "%");
    _lowerWaterLevel = _lowerWlc.checkLevel();
    // Logger::logInfo("LOWER: " + String(lowerWaterLevel.Distance) + "::" + String(lowerWaterLevel.OutputEnabled) + "::" + String(lowerWaterLevel.LevelPercent) + "%");
    _displayController.showProgress(_currentDateTime, _lowerWaterLevel.LevelPercent, String(_lowerWaterLevel.LevelPercent) + "%", _lowerWaterLevel.LevelPercent, String(_lowerWaterLevel.LevelPercent) + "%");
    // _displayController.showProgress(time, 0, String(0) + "%", lowerWaterLevel.LevelPercent, String(lowerWaterLevel.LevelPercent) + "%");
    // _displayController.showProgress(time, 0, String(0) + "%", lowerWaterLevel.LevelPercent, String(lowerWaterLevel.LevelPercent) + "%");
    _motorController.controlMotor(_upperWaterLevel, _lowerWaterLevel);

#endif

#if CURRENT_SENSOR_ENABLED

    float current = _currentSensor.getCurrent();

#endif
}

#if WEB_SERVER_ENABLED

void handleDashboardDataGetRequest(AsyncWebServerRequest *request)
{
    JsonDocument doc;
    doc["ServerTime"] = _currentDateTime.c_str();
    doc["Motor1RunHour"] = _setting.Motor1RunHour;
    doc["Motor2RunHour"] = _setting.Motor2RunHour;
    doc["Motor1On"] = false;
    doc["Motor2On"] = true;
    doc["LightOn"] = false;
    doc["UpsOn"] = true;
    doc["UpperWaterLevelPercent"] = _upperWaterLevel.LevelPercent;
    doc["UpperWaterLevelDistance"] = _upperWaterLevel.Distance;
    doc["LowerWaterLevelPercent"] = _lowerWaterLevel.LevelPercent;
    doc["LowerWaterLevelDistance"] = _lowerWaterLevel.Distance;
    doc["WifiStatus"] = _wifi.isConnected();
    doc["WifiSignalStrength"] = _wifi.signalStrength();

    String jsonString;
    serializeJson(doc, jsonString);

    request->send(200, "application/json", jsonString);
}

void handleWifiDataPostRequest(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    if (request->method() == HTTP_OPTIONS)
    {
        request->send(200);
    }
    if (request->contentType() != "application/json")
    {
        request->send(400, "text/plain", "Invalid Content-Type");
        return;
    }
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, data);
    if (error)
    {
        request->send(400, "text/plain", "Failed to parse JSON");
        return;
    }
    Logger::logInfo("WIFI SETTING SAVING...");
    _setting.WifiSsid = doc["WifiSsid"].as<String>();
    _setting.WifiPassword = doc["WifiPassword"].as<String>();
    _configManager.saveConfig(_setting);
    Logger::logInfo("WIFI SETTING SAVED");
    request->send(200, "application/json", "{\"status\": \"OK\"}");
}

void handleRestartPostRequest(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    if (request->method() == HTTP_OPTIONS)
    {
        request->send(200);
    }
    if (request->contentType() != "application/json")
    {
        request->send(400, "text/plain", "Invalid Content-Type");
        return;
    }
    Logger::logInfo("Restarting...");
    request->send(200, "application/json", "{\"status\": \"OK\"}");
    ESP.restart();
    Logger::logInfo("Restared");
}

void handleSettingPostRequest(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    if (request->method() == HTTP_OPTIONS)
    {
        request->send(200);
    }
    if (request->contentType() != "application/json")
    {
        request->send(400, "text/plain", "Invalid Content-Type");
        return;
    }
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, data);
    if (error)
    {
        request->send(400, "text/plain", "Failed to parse JSON");
        return;
    }
    _setting.WifiAccessPointName = doc["WifiAccessPointName"].as<String>();
    _setting.WifiAccessPointPassword = doc["WifiAccessPointPassword"].as<String>();
    _setting.WifiAccessPointIp = doc["WifiAccessPointIp"].as<String>();
    _setting.WifiSsid = doc["WifiSsid"].as<String>();
    _setting.WifiPassword = doc["WifiPassword"].as<String>();

    _setting.UpperTankEmptyDistance = doc["UpperTankEmptyDistance"];
    _setting.UpperTankMinTriggerPercent = doc["UpperTankMinTriggerPercent"];
    _setting.UpperTankMaxTriggerPercent = doc["UpperTankMaxTriggerPercent"];

    _setting.LowerTankEmptyDistance = doc["LowerTankEmptyDistance"];
    _setting.LowerTankMinTriggerPercent = doc["LowerTankMinTriggerPercent"];
    _setting.LowerTankMaxTriggerPercent = doc["LowerTankMaxTriggerPercent"];

    _setting.Motor1RunHour = doc["Motor1RunHour"];
    _setting.Motor2RunHour = doc["Motor2RunHour"];
    _setting.Motor1Auto = doc["Motor1Auto"];
    _setting.Motor2Auto = doc["Motor2Auto"];
    _setting.LightAuto = doc["LightAuto"];
    _setting.UpsAuto = doc["UpsAuto"];
    _setting.PageRefreshSeconds = doc["PageRefreshSeconds"];

    _configManager.saveConfig(_setting);
    request->send(200, "application/json", "{\"status\": \"OK\"}");
}
#endif

// https://iotcircuithub.com/iot-based-water-level-indicator/