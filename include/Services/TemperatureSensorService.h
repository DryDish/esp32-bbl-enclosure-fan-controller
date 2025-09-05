#pragma once

#include <OneWire.h>
#include <DallasTemperature.h>

#include "services.h"

struct TemperatureSensorConfig
{
    int gpioPin;
    unsigned int sampleIntervalMs;
};

class TemperatureSensorService : public Service
{
public:
    TemperatureSensorService(const TemperatureSensorConfig &config, bool debug = false);

    float getTemperature();
    void handle() override;
    void begin();

private:
    void _handleSensorConnection();

    const TemperatureSensorConfig &_config;

    bool _debug;
    float _currentTemperature;
    unsigned int _sampleIntervalMs;
    OneWire _oneWire;
    DallasTemperature _sensors;
    DeviceAddress _deviceAddress;
    unsigned long _lastSampleTime = 0;
    unsigned int _numberOfDevices = 0;
};