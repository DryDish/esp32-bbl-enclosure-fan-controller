#include <Arduino.h>

#include "Services/PrinterStateService.h"

PrinterStateService::PrinterStateService(bool debug)
{
    _state = PrinterState::IDLE;
    _previousState = PrinterState::UNKNOWN;
}

const PrinterState &PrinterStateService::getState()
{
    return _state;
}

const char *PrinterStateService::getStateString()
{
    return printerStateToString(_state);
};

void PrinterStateService::setState(PrinterState newState)
{
    _lastStateChangeTimeMs = millis();
    _previousState = _state;

    if (newState == PrinterState::IDLE && _previousState == PrinterState::PRINTING)
    {
        if (_debug)
        {
            Serial.println("Printer has finished printing. Overriding state from 'IDLE' to 'RECENTLY_PRINTED'.");
        }
        _targetStateAfterCooldown = PrinterState::IDLE;
        _state = PrinterState::RECENTLY_PRINTED;
        return;
    }

    if (newState == PrinterState::OFFLINE && _previousState == PrinterState::PRINTING)
    {
        if (_debug)
        {
            Serial.println("Printer lost connection during print. Overriding state from 'OFFLINE' to 'RECENTLY_PRINTED'.");
        }
        _targetStateAfterCooldown = PrinterState::OFFLINE;
        _state = PrinterState::RECENTLY_PRINTED;
        return;
    }

    _state = newState;

    if (_debug)
    {
        Serial.printf("Printer state set from '%s' to '%s'\n",
                      printerStateToString(_previousState), printerStateToString(newState));
    }
}

void PrinterStateService::handle()
{
    if (_state == PrinterState::RECENTLY_PRINTED && millis() - _lastStateChangeTimeMs > _ENCLOSURE_VENT_TIME_MS)
    {
        if (_debug)
        {
            Serial.printf("Timer expired. Changing state from 'RECENTLY_PRINTED' to '%s'.\n", _targetStateAfterCooldown);
        }
        setState(_targetStateAfterCooldown);
    }
}

const char *PrinterStateService::printerStateToString(const PrinterState &state)
{
    switch (state)
    {
    case PrinterState::IDLE:
        return "IDLE";
    case PrinterState::PRINTING:
        return "PRINTING";
    case PrinterState::RECENTLY_PRINTED:
        return "RECENTLY_PRINTED";
    case PrinterState::OFFLINE:
        return "OFFLINE";
    case PrinterState::UNKNOWN:
        return "UNKNOWN";
    default:
        return "INVALID_STATE";
    }
}