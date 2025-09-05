#include <Arduino.h>

#include "Services/FanService.h"
#include "Services/FanServiceInterruptHelper.h"
#include "Utils/TemperatureCurve.h"

FanService *FanService::instances[40] = {nullptr};

FanService::FanService(const FanConfig &config) : _config(config)
{
    if (config.tachPin >= 0 && config.tachPin < 40)
    {
        instances[config.tachPin] = this;
    }
}

void FanService::begin()
{
    // Setup PWM
    ledcSetup(_config.pwmChannel, _config.maxRpm, _PWM_RESOLUTION);
    ledcAttachPin(_config.pwmPin, _config.pwmChannel);
    ledcWrite(_config.pwmChannel, 0);

    // Setup Tachometer
    pinMode(_config.tachPin, INPUT_PULLUP);
    if (_config.tachPin >= 0 && _config.tachPin < 40)
    {
        attachInterrupt(digitalPinToInterrupt(_config.tachPin), gpioISR[_config.tachPin], RISING);
    }
}

void FanService::setFanSpeedByTemp(int temperatureC)
{
    int fanSpeedPercent = getFanSpeedPercent(temperatureC);

    int duty_cycle = 0;
    duty_cycle = map(fanSpeedPercent, 0, 100, 0, (1 << _PWM_RESOLUTION) - 1);

    ledcWrite(_config.pwmChannel, duty_cycle);
}

void FanService::SetFanSpeedByPercent(int fanSpeedPercent)
{
    int duty_cycle = 0;
    duty_cycle = map(fanSpeedPercent, 0, 100, 0, (1 << _PWM_RESOLUTION) - 1);

    ledcWrite(_config.pwmChannel, duty_cycle);
}

void FanService::_calculateRpm()
{
    uint32_t periodUs;
    noInterrupts();
    periodUs = _lastPulsePeriodUs;
    interrupts();

    if (periodUs > 0)
    {
        float instantRpm = 60000000.0f / (periodUs * 2.0f);

        // low-pass smoothing
        _currentRpm = _currentRpm * 0.7f + instantRpm * 0.3f;
    }
}

int FanService::getRpm()
{
    return static_cast<int>(_currentRpm);
}

#define DEBOUNCE_DELAY_US 800

void IRAM_ATTR FanService::handlePulse()
{
    uint32_t now = micros();
    uint32_t delta = now - _lastPulseTimeUs;
    if (delta >= DEBOUNCE_DELAY_US)
    {
        _lastPulsePeriodUs = delta;
        _lastPulseTimeUs = now;
    }
}

void FanService::handle()
{
    _calculateRpm();
}