#pragma once

struct TemperaturePoint
{
    float temperatureC;
    unsigned int fanSpeedPercentage;
};

const TemperaturePoint MbTemperatureCurve[] = {
    {.temperatureC = 0.00, .fanSpeedPercentage = 15}, // For my fans, this makes less noise than the lowest RPM, thus it's my minimum
    {.temperatureC = 30.0, .fanSpeedPercentage = 15},
    {.temperatureC = 50.0, .fanSpeedPercentage = 20},
    {.temperatureC = 55.0, .fanSpeedPercentage = 40},
    {.temperatureC = 60.0, .fanSpeedPercentage = 50},
    {.temperatureC = 70.0, .fanSpeedPercentage = 60},
    {.temperatureC = 90.0, .fanSpeedPercentage = 80},
};

// This  is for testing, hand warmable
// const TemperaturePoint temperatureCurve[] = {
//     {.temperatureC = 0.00, .fanSpeedPercentage = 15},
//     {.temperatureC = 20.0, .fanSpeedPercentage = 15},
//     {.temperatureC = 25.0, .fanSpeedPercentage = 20},
//     {.temperatureC = 27.0, .fanSpeedPercentage = 40},
//     {.temperatureC = 28.0, .fanSpeedPercentage = 50},
//     {.temperatureC = 30.0, .fanSpeedPercentage = 60},
//     {.temperatureC = 35.0, .fanSpeedPercentage = 100},
// };

const unsigned int TEMPERATURE_CURVE_SIZE = sizeof(MbTemperatureCurve) / sizeof(MbTemperatureCurve[0]);

unsigned int getFanSpeedPercent(float currentTemperatureInC);
