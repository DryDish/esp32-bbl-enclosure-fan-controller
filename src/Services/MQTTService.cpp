#include <ArduinoJson.h>

#include "Services/MQTTService.h"

MQTTService *MQTTService::_instance = nullptr;

MQTTService::MQTTService(const MQTTConfig &config, PrinterStateService &printerStateService, bool debug, bool verbose)
    : _config(config), _printerStateService(printerStateService), _client(_espClient), _verbose(verbose)
{
    _instance = this;
}

void MQTTService::begin()
{
    _espClient.setCACert(_config.bblCert);

    _client.setBufferSize(3072);

    _client.setServer(_config.server, _config.port);
    _client.setCallback(MQTTService::_staticCallback);
}

void MQTTService::_reconnect()
{
    if (!_client.connected())
    {
        unsigned long currentMillis = millis();

        if (currentMillis - _lastReconnectAttempt > _config.reconnectDelayMs)
        {
            _lastReconnectAttempt = currentMillis;
            if (_debug)
            {
                Serial.println("Attempting MQTT connection...");
            }

            // Convert IP string to object
            int octet1, octet2, octet3, octet4;
            sscanf(_config.server, "%d.%d.%d.%d", &octet1, &octet2, &octet3, &octet4);
            IPAddress printerIP = IPAddress(octet1, octet2, octet3, octet4);

            // Manually connect to the printer using the serialNumber and the BBL certificate.
            // This allows the PubSub client to use connect with the certificate using the printer's serial number
            // instead of the IP, allowing for secure connection.
            bool secureConnected = _espClient.connect(printerIP, _config.port, _config.printerSerialNumber, _config.bblCert, NULL, NULL);
            if (!secureConnected)
            {
                Serial.println("Secure Connection failed");
            }

            bool connected = _client.connect(_config.printerSerialNumber, _config.user, _config.pass);

            if (!connected)
            {
                if (_debug)
                {
                    Serial.print("MQTT Conneciton failed, rc=");
                    Serial.println(_client.state());
                }
            }
            else
            {
                String mqttReportTopic = "device/" + String(_config.printerSerialNumber) + "/report";
                if (_debug)
                {
                    Serial.println("MQTT Connected!");
                    Serial.print("Subscribing to topics: '");
                    Serial.println("'");
                }
                bool subscribed = _client.subscribe(mqttReportTopic.c_str(), 1);
                if (subscribed)
                {
                    _requestPrinterStatus();
                }
                else
                {
                    if (_debug)
                    {
                        Serial.println("MQTT Subscription failed!");
                    }
                }
            }
        }
    }
}

void MQTTService::_requestPrinterStatus()
{
    static const int MESSAGE_SIZE = 96;

    String mqttRequestTopic = "device/";
    mqttRequestTopic += String(_config.printerSerialNumber);
    mqttRequestTopic += "/request";

    JsonDocument jsonPayload;
    JsonObject pushing = jsonPayload["pushing"].to<JsonObject>();

    pushing["command"] = "pushall";
    pushing["sequence_id"] = "0";

    char buffer[MESSAGE_SIZE];
    size_t size = serializeJson(jsonPayload, buffer);

    if (_debug)
    {
        Serial.print("MQTT Request: ");
        Serial.println(buffer);
    }

    _client.publish(mqttRequestTopic.c_str(), buffer);
}

void MQTTService::_staticCallback(char *topic, byte *payload, unsigned int length)
{
    if (_instance)
    {
        _instance->_onMessageReceived(topic, payload, length);
    }
}

void MQTTService::_onMessageReceived(char *topic, byte *payload, unsigned int length)
{

    const static int MESSAGE_SIZE = 4024;
    JsonDocument json;
    DeserializationError error = deserializeJson(json, payload, length);

    if (_verbose)
    {
        Serial.print("Message arrived [");
        Serial.print(topic);
        Serial.println("] ");

        Serial.print("JSON: ");
        // This is the correct way to print
        serializeJson(json, Serial);
        Serial.println();
    }

    if (error)
    {
        if (_debug)
        {
            Serial.print("Failed to deserialize the JSON payload: ");
            Serial.println(error.c_str());
        }
        return;
    }

    if (!json["print"].is<JsonVariant>())
    {
        if (_debug)
        {
            Serial.println("JSON does not contain the 'print' key.");
        }
        return;
    }

    const char *rawPrinterState = json["print"]["gcode_state"];

    if (!rawPrinterState)
    {
        if (_verbose) // This is going to  happen A LOT. therfore delegated to verbose.
        {
            Serial.println("JSON does not contain the 'print.gcode_state' key.");
        }
        return;
    }

    if (_debug)
    {
        Serial.print("Received print.gcode_state: ");
        Serial.println(rawPrinterState);
    }

    PrinterState newState;

    if (strcmp(rawPrinterState, "IDLE") == 0)
    {
        newState = PrinterState::IDLE;
    }

    else if (strcmp(rawPrinterState, "PREPARE") == 0)
    {
        newState = PrinterState::PRINTING;
    }
    else if (strcmp(rawPrinterState, "RUNNING") == 0)
    {
        newState = PrinterState::PRINTING;
    }
    else if (strcmp(rawPrinterState, "PAUSE") == 0)
    {
        newState = PrinterState::PRINTING;
    }

    else if (strcmp(rawPrinterState, "FAILED") == 0)
    {
        newState = PrinterState::IDLE;
    }
    else if (strcmp(rawPrinterState, "FINISH") == 0)
    {
        newState = PrinterState::IDLE;
    }
    else
    {
        if (_debug)
        {
            Serial.print("Unknown printer state: ");
            Serial.println(rawPrinterState);
        }
        return;
    }

    if (_debug)
    {
        Serial.print("Setting new state: ");
        Serial.println(PrinterStateService::printerStateToString(newState));
    }

    _printerStateService.setState(newState);
}

void MQTTService::handle()
{
    _client.loop();
    _reconnect();
}
