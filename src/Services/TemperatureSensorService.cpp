#include <Arduino.h>

#include "Services/TemperatureSensorService.h"

TemperatureSensorService::TemperatureSensorService(const TemperatureSensorConfig &config, bool debug) : _config(config), _debug(debug)
{
    _sampleIntervalMs = config.sampleIntervalMs;

    _oneWire = OneWire(config.gpioPin);
    _sensors = DallasTemperature(&_oneWire);
}

void TemperatureSensorService::begin()
{
    _sensors.begin();
    _handleSensorConnection();
}

void TemperatureSensorService::_handleSensorConnection()
{
    _numberOfDevices = _sensors.getDeviceCount();

    if (_numberOfDevices == 0)
    {
        if (_debug)
        {
            Serial.println("No temperature sensors found");
        }
    }
    else
    {
        if (_debug)
        {
            Serial.print("Found ");
            Serial.print(_numberOfDevices);
            Serial.println(" temperature sensors");
        }

        _sensors.getAddress(_deviceAddress, 0);
    }
}

float TemperatureSensorService::getTemperature()
{
    return _currentTemperature;
}

void TemperatureSensorService::handle()
{

    unsigned long currentSampleTime = millis();
    if (currentSampleTime - _lastSampleTime > _sampleIntervalMs)
    {
        _lastSampleTime = currentSampleTime;
        _sensors.requestTemperatures();

        _currentTemperature = _sensors.getTempC(_deviceAddress);
        if (_currentTemperature == DEVICE_DISCONNECTED_C)
        {
            if (_debug)
            {
                Serial.println("Error: Could not read temperature data");
            }
            _handleSensorConnection();
        }
    }
}