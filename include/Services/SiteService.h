#pragma once

#include "services.h"
#include <WebServer.h>

struct SiteSensorData
{
    float MBTemperature;
    int MBInFanSpeedPercent;
    int MBInFanRPM;
    int MBOutFanSpeedPercent;
    int MBOutFanRPM;
    int EnclosureFanSpeedPercent;
    int EnclosureFanRPM;
    int MBFanExpectedRPM;
    int EnclosureFanExpectedRPM;
};

class SiteService : public WebServer,
                    public Service
{
public:
    SiteService(uint16_t port, SiteSensorData const &data);
    void begin();
    void handle() override;

private:
    SiteSensorData const &_data;

    void _handleRoot();
    void _handleData();
};