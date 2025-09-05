#include "Utils/LookupTable.h"

LookupTable::LookupTable()
{
}

// This is a carefully calibrated temperature lookup table for my unbranded thermistor.
// I used the Shelly DS18B20 Temperature Sensor to get accurate readings for calibration.
// If you intend to use a thermistor too, you will need to make your own lookup table.
// IMPORTANT: This is very specific to my thermistor.
// Since it's completely unbranded, these values will likely not work for you.
const DataPoint LookupTable::_data[LookupTable::_LOOKUP_TABLE_SIZE] = {
    {12.60, 2917},
    {13.50, 2890},
    {14.63, 2853},
    {15.76, 2816},
    {16.50, 2793},
    {17.75, 2754},
    {18.50, 2729},
    {19.50, 2692},
    {23.00, 2569},
    {24.40, 2523},
    {25.50, 2488},
    {26.00, 2474},
    {26.50, 2454},
    {27.12, 2435},
    {29.66, 2349},
    {31.37, 2290},
    {34.54, 2182},
    {37.31, 2087},
    {39.14, 2023},
    {41.82, 1928},
    {44.99, 1823},
    {46.66, 1783},
    {50.95, 1632},
    {52.19, 1531},
    {56.43, 1452},
    {67.87, 1140},
    {73.47, 1002},
    {77.00, 914},
    {80.01, 856},
    {81.73, 821},
    {88.03, 703},
    {93.50, 613},
};

float LookupTable::getTemperature(int adcValue)
{
    if (adcValue >= _data[0].adcValue)
    {
        return 11.999; // Known lowest value for temperatures below my calibrated range
    }
    if (adcValue <= _data[_LOOKUP_TABLE_SIZE - 1].adcValue)
    {
        return 93.999; // Known highest value for temperatures above my calibrated range
    }

    for (int i = 0; i < _LOOKUP_TABLE_SIZE - 1; ++i)
    {
        if (adcValue <= _data[i].adcValue && adcValue >= _data[i + 1].adcValue)
        {
            float adcRange = _data[i].adcValue - _data[i + 1].adcValue;
            float adcRatio = (float)(adcValue - _data[i + 1].adcValue) / adcRange;
            float tempRange = _data[i].temperatureC - _data[i + 1].temperatureC;
            float interpolatedTemp = _data[i + 1].temperatureC + (tempRange * adcRatio);
            return interpolatedTemp;
        }
    }
    return -999.0;
}