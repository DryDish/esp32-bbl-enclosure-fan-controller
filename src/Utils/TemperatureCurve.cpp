#include "Utils/TemperatureCurve.h"

unsigned int getFanSpeedPercent(float currentTemp)
{
    const float cutoffTemp = MbTemperatureCurve[1].temperatureC;

    if (currentTemp < cutoffTemp)
    {
        return MbTemperatureCurve[0].fanSpeedPercentage;
    }

    if (currentTemp >= MbTemperatureCurve[TEMPERATURE_CURVE_SIZE - 1].temperatureC)
    {
        return MbTemperatureCurve[TEMPERATURE_CURVE_SIZE - 1].fanSpeedPercentage;
    }

    for (unsigned int i = 1; i < TEMPERATURE_CURVE_SIZE; ++i)
    {
        if (currentTemp <= MbTemperatureCurve[i].temperatureC)
        {
            float temp1 = MbTemperatureCurve[i - 1].temperatureC;
            float speed1 = MbTemperatureCurve[i - 1].fanSpeedPercentage;
            float temp2 = MbTemperatureCurve[i].temperatureC;
            float speed2 = MbTemperatureCurve[i].fanSpeedPercentage;

            unsigned int interpolatedSpeed = (unsigned int)(speed1 + (currentTemp - temp1) * (speed2 - speed1) / (temp2 - temp1));
            return interpolatedSpeed;
        }
    }

    return 0;
}