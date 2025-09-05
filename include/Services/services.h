#pragma once

class Service
{
public:
    virtual void handle() = 0;
};

void handleServices(Service *services[], unsigned int size);