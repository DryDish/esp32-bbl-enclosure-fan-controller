#pragma once

#include <ArduinoOTA.h>
#include "services.h"

struct OTAConfig
{
    const char *hostname;
    const char *password;
};

class OTAService : public Service
{
public:
    OTAService(const OTAConfig &config);
    void begin();
    void handle() override;

private:
    const OTAConfig &_config;
};