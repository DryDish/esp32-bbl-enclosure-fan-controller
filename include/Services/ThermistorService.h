#pragma once

#include <vector>
#include <Arduino.h>

#include "services.h"
#include "Utils/LookupTable.h"

struct ThermistorConfig
{
    const int gpioPin;
    const int samplesToAverage;
    const int sampleIntervalMs;
    const int _thermistorVoltageDividerResistorSize;
};

class ThermistorService : public Service
{
public:
    ThermistorService(const ThermistorConfig &config);
    void handle() override;
    float getTemperature();

private:
    const ThermistorConfig &_config;
    std::vector<uint16_t> _thermistorReadings;
    LookupTable _lookupTable;

    void _updateThermistorReadings();
};