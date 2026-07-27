#ifndef _SETTING_C_
#define _SETTING_C_
#include <Arduino.h>

struct Setting
{
    String WifiAccessPointName;
    String WifiAccessPointPassword;
    String WifiAccessPointIp;
    String WifiSsid;
    String WifiPassword;
    float UpperTankEmptyDistance;
    float UpperTankMinTriggerPercent;
    float UpperTankMaxTriggerPercent;
    float LowerTankEmptyDistance;
    float LowerTankMinTriggerPercent;
    float LowerTankMaxTriggerPercent;
    float Motor1RunHour;
    float Motor2RunHour;
    bool Motor1Auto;
    bool Motor2Auto;
    bool LightAuto;
    bool UpsAuto;
    int PageRefreshSeconds;
};
#endif