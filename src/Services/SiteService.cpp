#include <ArduinoJson.h>

#include "Services/PrinterStateService.h"
#include "Services/SiteService.h"
#include "HTML/index.h"

SiteService::SiteService(uint16_t port, SiteSensorData const &data) : WebServer(port), _data(data) {}

void SiteService::begin()
{
    this->on("/", [&]()
             { _handleRoot(); });
    this->on("/data", [&]()
             { _handleData(); });
    this->on("/favicon.ico", [&]()
             { this->send(204); });
    WebServer::begin();
}

void SiteService::_handleData()
{
    JsonDocument json;
    json["motherboardTemperature"] = _data.MBTemperature;
    json["enclosureTemperature"] = _data.EnclosureTemperature;
    json["printerState"] = _data.PrinterState;
    json["motherboardInFanSpeedPercent"] = _data.MBInFanSpeedPercent;
    json["motherboardInFanRPM"] = _data.MBInFanRPM;
    json["motherboardOutFanSpeedPercent"] = _data.MBOutFanSpeedPercent;
    json["MotherboardOutFanRPM"] = _data.MBOutFanRPM;
    json["EnclosureFanSpeedPercent"] = _data.EnclosureFanSpeedPercent;
    json["EnclosureFanRPM"] = _data.EnclosureFanRPM;
    json["smallFanExpectedRPM"] = _data.MBFanExpectedRPM;
    json["largeFanExpectedRPM"] = _data.EnclosureFanExpectedRPM;
    String output;

    serializeJson(json, output);

    this->send(200, "application/json", output);
}

void SiteService::_handleRoot()
{
    this->send(200, "text/html", indexPage);
}

void SiteService::handle()
{
    this->handleClient();
}