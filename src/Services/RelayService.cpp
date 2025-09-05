#include <Arduino.h>

#include "Services/RelayService.h"

RelayService::RelayService(const RelayConfig &config, bool debug) : _config(config) {}

void RelayService::begin()
{
    pinMode(_config.gpioPin, OUTPUT);
    turnOff();
}

void RelayService::turnOn()
{
    if (_debug)
    {
        Serial.println("Relay on");
    }
    digitalWrite(_config.gpioPin, HIGH);
}

void RelayService::turnOff()
{
    if (_debug)
    {
        Serial.println("Relay off");
    }
    digitalWrite(_config.gpioPin, LOW);
}