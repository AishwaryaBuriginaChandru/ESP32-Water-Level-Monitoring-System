#ifndef _ESP_NOW_CONTROLLER_C_
#define _ESP_NOW_CONTROLLER_C_

// #include <Arduino.h>
#include <esp_now.h>
#include "WiFi.h"

class EspNowController
{
    // REPLACE WITH YOUR RECEIVER MAC Address
    // uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    // uint8_t broadcastAddress;

    // Structure example to send data
    // Must match the receiver structure
    typedef struct struct_message
    {
        char a[32];
        float distance;
        bool d;
    } struct_message;

    // Create a struct_message called myData
    struct_message myData;

    esp_now_peer_info_t peerInfo;

public:
    // callback when data is sent
    void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
    {
        Serial.print("\r\nLast Packet Send Status:\t");
        Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
    }

    void initialize()
    {
        WiFi.mode(WIFI_MODE_STA);
        Serial.println(WiFi.macAddress());
        // Init ESP-NOW
        if (esp_now_init() != ESP_OK)
        {
            Serial.println("Error initializing ESP-NOW");
            return;
        }

        // Once ESPNow is successfully Init, we will register for Send CB to
        // get the status of Transmitted packet
        // esp_now_register_send_cb(onDataSent);

        // Register peer
        // memcpy(peerInfo.peer_addr, broadcastAddress, 6);
        peerInfo.channel = 0;
        peerInfo.encrypt = false;

        // Add peer
        if (esp_now_add_peer(&peerInfo) != ESP_OK)
        {
            Serial.println("Failed to add peer");
            return;
        }
    }

    bool sendMessage(String message)
    {
        strcpy(myData.a, "THIS IS A CHAR");
        myData.distance = 1.2;
        myData.d = false;
        // esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
        // return result == ESP_OK;
        return true;
    }
};

#endif