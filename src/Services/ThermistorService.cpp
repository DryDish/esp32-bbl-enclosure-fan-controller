#include "Services/ThermistorService.h"

ThermistorService::ThermistorService(const ThermistorConfig &config) : _config(config),
                                                                       _thermistorReadings(config.samplesToAverage, UINT16_MAX)
{
}

void ThermistorService::_updateThermistorReadings()
{
    static unsigned int bufferIndex = 0;
    static unsigned long lastSampleTime = 0;

    unsigned long currentSampleTime = millis();

    if (currentSampleTime - lastSampleTime >= _config.sampleIntervalMs)
    {

        lastSampleTime = currentSampleTime;

        _thermistorReadings[bufferIndex] = analogRead(_config.gpioPin);

        bufferIndex = (bufferIndex + 1) % _config.samplesToAverage;
    }
}

float ThermistorService::getTemperature()
{
    unsigned long sum = 0;
    unsigned int samplesCounted = 0;

    for (const auto &reading : _thermistorReadings)
    {
        if (reading != UINT16_MAX)
        {
            sum += reading;
            samplesCounted++;
        }
    }

    if (samplesCounted > 0)
    {
        unsigned long averageADC = sum / samplesCounted;
        return LookupTable::getTemperature(averageADC);
    }
    else
    {
        return 0.0;
    }
}

void ThermistorService::handle()
{
    _updateThermistorReadings();
}
