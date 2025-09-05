#pragma once

#include <PubSubClient.h>
#include <WiFiClientSecure.h>

#include "services.h"
#include "PrinterStateService.h"

struct MQTTConfig
{
    const char *server;
    const int port;
    const char *user;
    const char *pass;
    const char *printerSerialNumber;
    const int reconnectDelayMs;
    const char *bblCert;
};

class MQTTService : public Service
{
public:
    MQTTService(const MQTTConfig &config, PrinterStateService &printerStateService, bool debug = false, bool verbose = false);
    void begin();
    void handle() override;

private:
    PrinterStateService &_printerStateService;
    static MQTTService *_instance;
    const MQTTConfig &_config;
    WiFiClientSecure _espClient;
    PubSubClient _client;
    bool _debug;
    bool _verbose;
    unsigned long _lastReconnectAttempt = 0;

    void _reconnect();
    void _onMessageReceived(char *topic, byte *payload, unsigned int length);
    static void _staticCallback(char *topic, byte *payload, unsigned int length);
    void _requestPrinterStatus();
};