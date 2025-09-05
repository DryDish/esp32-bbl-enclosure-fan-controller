#include <Arduino.h>
#include <WiFi.h>

#include "Utils/Wireless.h"

void handleWifiServices(const WifiConfig &config, Service *services[], unsigned int size, connectionCallback onConnect, const bool debugLogs)
{
    static unsigned long lastConnectionAttempt = 0;

    if (WiFi.status() == WL_CONNECTED)
    {
        handleServices(services, size);
    }
    else
    {
        unsigned long currentConnectionAttempt = millis();

        if (currentConnectionAttempt - lastConnectionAttempt > config.wifiTimeoutMs)
        {

            if (debugLogs)
            {
                Serial.println("Wifi connection lost. Attempting to reconnect...");
            }

            lastConnectionAttempt = currentConnectionAttempt;
            connectToWifi(config, onConnect, debugLogs);
        }
    }
};

void connectToWifi(const WifiConfig &config, connectionCallback onConnect, const bool debugLogs)
{
    if (debugLogs)
    {
        Serial.print("Connecting to wifi");
    }

    WiFi.begin(config.ssid, config.password);
    unsigned long wifiStartTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - wifiStartTime < config.wifiTimeoutMs)
    {
        delay(500);
        if (debugLogs)
        {
            Serial.print(".");
        }
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        if (debugLogs)
        {
            Serial.printf("\nConnected!\nIP Address: %s\n", WiFi.localIP().toString().c_str());
        }
        onConnect();
    }
};
