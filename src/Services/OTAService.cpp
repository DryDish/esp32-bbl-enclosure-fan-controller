#include "Services/OTAService.h"

OTAService::OTAService(const OTAConfig &config)
    : _config(config)
{
}

void OTAService::begin()
{
    ArduinoOTA.setHostname(_config.hostname);
    ArduinoOTA.setPassword(_config.password);
    ArduinoOTA.begin();
}

void OTAService::handle()
{
    ArduinoOTA.handle();
}