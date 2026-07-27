#ifndef _WATER_LEVEL_CONTROLLER_C_
#define _WATER_LEVEL_CONTROLLER_C_

#include <Arduino.h>
#include "Logger.cpp"
#include "WaterLevel.cpp"
// #include <stl_map.h>
// #include <map>
// using map = std ::map;
class WaterLevelController
{

private:
    int _triggerPin;
    int _echoPin;
    int _outputControlPin;
    float _sensorHeight;
    float _topWaterHeight;
    int _minTriggerPercent = 20;
    int _maxTriggerPercent = 100;
    boolean _isLowerTank;
    // boolean _motorStarted = false;

public:
    /*
    triggerPin: Trigger Pin Number of Ultrasonic Sensor
    echoPin: Echo Pin Number of Ultrasonic Sensor
    outputControlPin: This will be used to Control the Output like Motors, Relays.
    sensorHeight: Distance Between Bottom and Sensor Surface in CM
    minTriggerPercent: Distance Between Bottom and Bottom Cutoff in CM
    topWaterHeight: Distance Between Bottom and Top Cutoff in CM
    */
    void initialize(int triggerPin = 32, int echoPin = 33, float sensorHeight = 150,
                    float topWaterHeight = 130, int minTriggerPercent = 20, int maxTriggerPercent = 100, boolean isLowerTank = false)
    {
        _triggerPin = triggerPin;
        _echoPin = echoPin;
        // _outputControlPin = outputControlPin;
        _sensorHeight = sensorHeight;
        _topWaterHeight = topWaterHeight;
        _minTriggerPercent = minTriggerPercent;
        _maxTriggerPercent = maxTriggerPercent;
        _isLowerTank = isLowerTank;

        pinMode(_echoPin, INPUT);
        pinMode(_triggerPin, OUTPUT);
        // pinMode(outputControlPin, OUTPUT);
    }

    /*
    1. Turn on TRIG Pin to 5V for at least 10uS
    2. The module will then automatically send 8 40KHz tones and automatically detect when the signal return after reflecting back from the object.
    3. Upon detecting the signal, a high-level signal is outputted through the IO Pin ECHO.
    4. By keeping track of the time duration between transmitting and receiving the signal, the distance can be calculated.
    5. Distance = (Time to Receive Reflected Signal * speed of sound (340M/S)) /2
    */
    float getLevel()
    {
        // Set the trigger pin LOW for 2uS
        digitalWrite(_triggerPin, LOW);
        delayMicroseconds(2);

        // Set the trigger pin HIGH for 20us to send pulse
        digitalWrite(_triggerPin, HIGH);
        delayMicroseconds(20);

        // Return the trigger pin to LOW
        digitalWrite(_triggerPin, LOW);

        // Measure the width of the incoming pulse
        float duration = pulseIn(_echoPin, HIGH);
        // Serial.println("DUR: " + String(duration));
        // Determine distance from duration. Use 343 metres per second as speed of sound.
        // Divide by 10 as we want Centi Meters
        // float currentLevel = ((duration / 2) * 0.343) / 10;

        float currentLevel = (duration / 2) / 29.1;
        // float currentLevel = duration / 58;

        return currentLevel;
    }

    WaterLevel checkLevel()
    {
        WaterLevel level;
        level.Distance = getLevel();
        float actualHeight = _sensorHeight - _topWaterHeight;
        // map(value, fromLow, fromHigh, toLow, toHigh)
        int currentLevelPercent = ::map(level.Distance, actualHeight, _sensorHeight, 0, 100);
        if (currentLevelPercent < 0)
        {
            currentLevelPercent = 0;
        }
        if (currentLevelPercent > 100)
        {
            currentLevelPercent = 100;
        }
        level.LevelPercent = currentLevelPercent;
        level.OutputEnabled = false;

        if (_isLowerTank)
        {
            if (currentLevelPercent >= _minTriggerPercent)
            {
                level.OutputEnabled = true;
            }
            else if (currentLevelPercent > _minTriggerPercent && currentLevelPercent < _maxTriggerPercent)
            {
                level.OutputEnabled = true;
            }
        }
        else
        {
            if (currentLevelPercent <= _minTriggerPercent)
            {
                level.OutputEnabled = true;
            }
            else if (currentLevelPercent > _minTriggerPercent && currentLevelPercent < _maxTriggerPercent)
            {
                level.OutputEnabled = true;
            }
        }
        // digitalWrite(_outputControlPin, level.OutputEnabled ? HIGH : LOW);
        // Apply a 100ms delay to allow the sensor to stabilize and do it all over again
        delay(100);
        return level;
    }
};

#endif