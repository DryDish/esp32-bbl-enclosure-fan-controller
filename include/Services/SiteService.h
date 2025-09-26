#pragma once

#include "services.h"
#include "Services/PrinterStateService.h"
#include <WebServer.h>

struct SiteSensorData
{
    float MBTemperature;
    float EnclosureTemperature;
    const char *PrinterState;
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