#pragma once

#include <Arduino.h>

#include "services.h"

struct FanConfig
{
    int pwmPin;
    int tachPin;
    int pwmChannel;
    int maxRpm;
};

class FanService : public Service
{
public:
    FanService(const FanConfig &config);
    void begin();
    void handle() override;
    int getRpm();
    void setFanSpeedByTemp(int temperatureC);
    void SetFanSpeedByPercent(int fanSpeedPercent);
    void IRAM_ATTR handlePulse();

    static FanService *instances[40];

private:
    void _calculateRpm();

    const FanConfig &_config;
    const int _PWM_RESOLUTION = 8;
    volatile uint32_t _lastPulseTimeUs = 0;
    volatile uint32_t _lastPulsePeriodUs = 0;
    float _currentRpm = 0;
};