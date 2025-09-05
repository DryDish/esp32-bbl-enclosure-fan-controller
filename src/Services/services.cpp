#include "Services/services.h"

void handleServices(Service *services[], unsigned int size)
{
    for (unsigned int i = 0; i < size; i++)
    {
        services[i]->handle();
    }
};
