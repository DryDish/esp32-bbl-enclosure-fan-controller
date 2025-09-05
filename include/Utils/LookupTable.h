#pragma once

struct DataPoint
{
    float temperatureC;
    int adcValue;
};

class LookupTable
{
public:
    LookupTable();

    static float getTemperature(int adcValue);

private:
    const static int _LOOKUP_TABLE_SIZE = 32;
    const static DataPoint _data[_LOOKUP_TABLE_SIZE];
};