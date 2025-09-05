#pragma once

#include "services.h"

struct RelayConfig
{
    int gpioPin;
};

class RelayService
{
public:
    RelayService(const RelayConfig &config, bool debug = false);

    void begin();
    void turnOn();
    void turnOff();

private:
    const RelayConfig &_config;
    bool _debug;
};