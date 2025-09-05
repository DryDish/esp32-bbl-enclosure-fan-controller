#pragma once

#include "services.h"

enum PrinterState
{
    IDLE,
    PRINTING,
    RECENTLY_PRINTED,
    OFFLINE,
    UNKNOWN,
};

class PrinterStateService : public Service
{
public:
    PrinterStateService(bool debug = false);

    const PrinterState &getState();
    const char *getStateString();
    void setState(PrinterState newState);
    void handle() override;

    static const char *printerStateToString(const PrinterState &state);

private:
    PrinterState _state;
    PrinterState _previousState;
    PrinterState _targetStateAfterCooldown;
    bool _debug;

    unsigned long _lastStateChangeTimeMs = 0;
    static const unsigned long _ENCLOSURE_VENT_TIME_MS = 5 * 60 * 1000; // 5 minutes
};