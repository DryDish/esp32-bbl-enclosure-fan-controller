#pragma once

#include "Services/services.h"

struct WifiConfig
{
    const char *ssid;
    const char *password;
    const int wifiTimeoutMs;
};

typedef void (*connectionCallback)();

void connectToWifi(const WifiConfig &config, connectionCallback onConnect, const bool debugLogs);
void handleWifiServices(const WifiConfig &config, Service *services[], unsigned int size, connectionCallback onConnect, const bool debugLogs = false);
