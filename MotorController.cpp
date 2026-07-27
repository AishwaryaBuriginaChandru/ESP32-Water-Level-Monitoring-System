#ifndef _MOTOR_C_
#define _MOTOR_C_

#include <Arduino.h>
#include "WaterLevel.cpp"
#include "Logger.cpp"

class MotorController
{

private:
    int _outputControlPin1;
    int _outputControlPin2;
    // WaterLevel _upperWaterLevel;
    // WaterLevel _lowerWaterLevel;

public:
    /*
    triggerPin: Trigger Pin Number of Ultrasonic Sensor
    echoPin: Echo Pin Number of Ultrasonic Sensor
    outputControlPin: This will be used to Control the Output like Motors, Relays.
    sensorHeight: Distance Between Bottom and Sensor Surface in CM
    minTriggerPercent: Distance Between Bottom and Bottom Cutoff in CM
    topWaterHeight: Distance Between Bottom and Top Cutoff in CM
    */
    void initialize(int outputControlPin1 = 18, int outputControlPin2 = 19
                    // ,WaterLevel &upperWaterLevel, WaterLevel &lowerWaterLevel
    )
    {
        // _upperWaterLevel = upperWaterLevel;
        // _lowerWaterLevel = lowerWaterLevel;
        _outputControlPin1 = outputControlPin1;
        _outputControlPin2 = outputControlPin2;
        pinMode(outputControlPin1, OUTPUT);
        pinMode(outputControlPin2, OUTPUT);
    }

    /*
    1. Turn on TRIG Pin to 5V for at least 10uS
    2. The module will then automatically send 8 40KHz tones and automatically detect when the signal return after reflecting back from the object.
    3. Upon detecting the signal, a high-level signal is outputted through the IO Pin ECHO.
    4. By keeping track of the time duration between transmitting and receiving the signal, the distance can be calculated.
    5. Distance = (Time to Receive Reflected Signal * speed of sound (340M/S)) /2
    */

    void controlMotor(WaterLevel &upperLevel, WaterLevel &lowerLevel)
    {
        digitalWrite(_outputControlPin1, upperLevel.OutputEnabled ? HIGH : LOW);
        digitalWrite(_outputControlPin2, lowerLevel.OutputEnabled ? HIGH : LOW);
        // Apply a 100ms delay to allow the sensor to stabilize and do it all over again
        delay(100);
        // return level;
    }
};

#endif