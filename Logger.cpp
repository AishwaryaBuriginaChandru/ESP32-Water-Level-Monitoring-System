#ifndef _LOGGER_C_
#define _LOGGER_C_
#include <Arduino.h>
#define DEBUG 1
class Logger
{
public:
    static void initLogger(int baudRate = 115200)
    {
        Serial.begin(baudRate);
        Serial.setDebugOutput(true);
        // Serial.println("Initializing Logger...");
        logInfo("Initialized Logger");
    }
    static void logInfo(String message)
    {
        log("INFO :: " + message);
    }
    static void logWarn(String message)
    {
        log("WARN :: " + message);
    }
    static void logError(String message)
    {
        log("ERROR :: " + message);
    }
    static void log(String &message)
    {
#if DEBUG == 1
        Serial.println(message);
#endif
    }
    static void logI2CDeviceInfo()
    {
        // byte error, address;
        // int nDevices;
        // Serial.println("Scanning...");
        // nDevices = 0;
        // for (address = 1; address < 127; address++)
        // {
        //     Wire.beginTransmission(address);
        //     error = Wire.endTransmission();
        //     if (error == 0)
        //     {
        //         logInfo("I2C device found at address 0x");
        //         if (address < 16)
        //         {
        //             logInfo("0");
        //         }
        //         Serial.println(address, HEX);
        //         nDevices++;
        //     }
        //     else if (error == 4)
        //     {
        //         logError("Unknow error at address 0x");
        //         if (address < 16)
        //         {
        //             logInfo("0");
        //         }
        //         Serial.println(address, HEX);
        //     }
        // }
        // if (nDevices == 0)
        // {
        //     logWarn("No I2C devices found\n");
        // }
        // else
        // {
        //     logInfo("done\n");
        // }
    }
};

#endif