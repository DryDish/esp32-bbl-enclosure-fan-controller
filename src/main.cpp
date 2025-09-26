#include <Arduino.h>
#include <WiFiClientSecure.h>

#include "credentials.h"

#include "Services/MQTTService.h"
#include "Services/OTAService.h"
#include "Services/ThermistorService.h"
#include "Services/FanService.h"
#include "Services/SiteService.h"
#include "Services/RelayService.h"
#include "Services/PrinterStateService.h"
#include "Services/TemperatureSensorService.h"
#include "Utils/Wireless.h"
#include "Utils/TemperatureCurve.h"

const bool SHOW_LOGS = true;

// ---------------------- CONFIGS ----------------------

// WIFI
static const WifiConfig wifiConfig = {
    .ssid = WIFI_SSID,
    .password = WIFI_PASS,
    .wifiTimeoutMs = 10000, // 10 seconds
};

// OTA
static const OTAConfig otaConfig = {
    .hostname = OTA_HOSTNAME,
    .password = OTA_PASSWORD,
};

// MQTT
static const MQTTConfig mqttConfig = {
    .server = MQTT_SERVER,
    .port = MQTT_PORT,
    .user = MQTT_USER,
    .pass = MQTT_PASS,
    .printerSerialNumber = PRINTER_SERIAL_NUMBER,
    .reconnectDelayMs = 10000,
    .bblCert = BBL_CERT,
};

// Thermistor
static const ThermistorConfig thermistorConfig = {
    .gpioPin = GPIO_NUM_34,
    .samplesToAverage = 10,
    .sampleIntervalMs = 100,
    ._thermistorVoltageDividerResistorSize = 5000,
};

// Temperature Sensor
static const TemperatureSensorConfig temperatureSensorConfig = {
    .gpioPin = GPIO_NUM_32,
    .sampleIntervalMs = 500,
};

// Fan Motherboard In
static const FanConfig fanMotherboardInConfig = {
    .pwmPin = GPIO_NUM_15,
    .tachPin = GPIO_NUM_13,
    .pwmChannel = 0,
    .maxRpm = 15000,
};

// Fan Motherboard Out
static const FanConfig fanMotherboardOutConfig = {
    .pwmPin = GPIO_NUM_16,
    .tachPin = GPIO_NUM_14,
    .pwmChannel = 1,
    .maxRpm = 15000,
};

// Fan Enclosure
static const FanConfig fanEnclosureConfig = {
    .pwmPin = GPIO_NUM_17,
    .tachPin = GPIO_NUM_26,
    .pwmChannel = 2,
    .maxRpm = 10000,
};

// Relay
static const RelayConfig relayConfig = {
    .gpioPin = GPIO_NUM_33,
};

// Site
static SiteSensorData sensors = {
    .MBTemperature = 0,
    .EnclosureTemperature = 0,
    .MBInFanSpeedPercent = 0,
    .MBInFanRPM = 0,
    .MBOutFanSpeedPercent = 0,
    .MBOutFanRPM = 0,
    .EnclosureFanSpeedPercent = 0,
    .EnclosureFanRPM = 0,
    .MBFanExpectedRPM = 0,
    .EnclosureFanExpectedRPM = 0,
};

// ---------------------- GLOBALS ----------------------

static PrinterStateService printerStateService;

static OTAService otaService(otaConfig);

static MQTTService mqttService(mqttConfig, printerStateService, SHOW_LOGS);

static ThermistorService motherboardTermistor(thermistorConfig);

static TemperatureSensorService enclosureTemperatureProbe(temperatureSensorConfig, SHOW_LOGS);

static FanService fanMotherboardInService(fanMotherboardInConfig);
static FanService fanMotherboardOutService(fanMotherboardOutConfig);
static FanService fanEnclosureService(fanEnclosureConfig);

static RelayService relayService(relayConfig);

static SiteService siteService(80, sensors);

// ---------------------- FUNCTIONS ----------------------

void startOfflineServices()
{
    fanMotherboardInService.begin();
    fanMotherboardOutService.begin();
    fanEnclosureService.begin();
    relayService.begin();
    enclosureTemperatureProbe.begin();
}

void wifiConnectedCallback()
{
    otaService.begin();
    mqttService.begin();
    siteService.begin();
}

// ---------------------- SERVICE ARRAYS ----------------------

static Service *wirelessServices[] = {
    &otaService,
    &mqttService,
    &siteService,
};
static const unsigned int wirelessServicesSize = sizeof(wirelessServices) / sizeof(wirelessServices[0]);

static Service *offlineServices[] = {
    &motherboardTermistor,
    &enclosureTemperatureProbe,
    &fanMotherboardInService,
    &fanMotherboardOutService,
    &fanEnclosureService,
    &printerStateService,
};
static const unsigned int offlineServicesSize = sizeof(offlineServices) / sizeof(offlineServices[0]);

// ---------------------- SETUP ----------------------

void setup()
{
    Serial.begin(115200);
    analogSetAttenuation(ADC_11db);

    startOfflineServices();
    connectToWifi(wifiConfig, wifiConnectedCallback, SHOW_LOGS);
}

// ---------------------- LOOP ----------------------

void loop()
{
    handleWifiServices(wifiConfig, wirelessServices, wirelessServicesSize, wifiConnectedCallback, SHOW_LOGS);
    handleServices(offlineServices, offlineServicesSize);

    static unsigned long lastLoopLogic = 0;
    if (millis() - lastLoopLogic >= 1000)
    {
        lastLoopLogic = millis();

        const PrinterState &currentState = printerStateService.getState();
        sensors.MBTemperature = motherboardTermistor.getTemperature();
        sensors.EnclosureTemperature = enclosureTemperatureProbe.getTemperature();
        sensors.PrinterState = PrinterStateService::printerStateToString(currentState);

        if (SHOW_LOGS)
        {
            Serial.printf("Printer state: %s | MB Temperature: %.2f°C | Enclosure Temperature: %.2f°C\n",
                          PrinterStateService::printerStateToString(currentState), sensors.MBTemperature, sensors.EnclosureTemperature);
        }

        int fanSpeedPercent = getFanSpeedPercent(sensors.MBTemperature);
        sensors.MBFanExpectedRPM = fanSpeedPercent * fanMotherboardInConfig.maxRpm / 100;
        sensors.MBInFanSpeedPercent = fanSpeedPercent;
        sensors.MBOutFanSpeedPercent = fanSpeedPercent;

        switch (currentState)
        {
        case PrinterState::OFFLINE:
            fanMotherboardInService.SetFanSpeedByPercent(0);
            fanMotherboardOutService.SetFanSpeedByPercent(0);
            fanEnclosureService.SetFanSpeedByPercent(0);

            relayService.turnOff();

            sensors.MBInFanSpeedPercent = 0;
            sensors.MBOutFanSpeedPercent = 0;
            sensors.EnclosureFanSpeedPercent = 0;
            sensors.EnclosureFanExpectedRPM = 0;

            break;
        case PrinterState::IDLE:
            fanMotherboardInService.SetFanSpeedByPercent(0);
            fanMotherboardOutService.SetFanSpeedByPercent(0);
            fanEnclosureService.SetFanSpeedByPercent(0);

            relayService.turnOff();

            sensors.MBInFanSpeedPercent = 0;
            sensors.MBOutFanSpeedPercent = 0;
            sensors.EnclosureFanSpeedPercent = 0;
            sensors.EnclosureFanExpectedRPM = 0;

            break;
        case PrinterState::PRINTING:
            fanMotherboardInService.setFanSpeedByTemp(sensors.MBTemperature);
            fanMotherboardOutService.setFanSpeedByTemp(sensors.MBTemperature);
            fanEnclosureService.SetFanSpeedByPercent(10);

            relayService.turnOn();

            sensors.MBInFanSpeedPercent = fanSpeedPercent;
            sensors.MBOutFanSpeedPercent = fanSpeedPercent;
            sensors.EnclosureFanSpeedPercent = 10;
            sensors.EnclosureFanExpectedRPM = 10 * fanEnclosureConfig.maxRpm / 100;
            break;
        case PrinterState::RECENTLY_PRINTED:
            fanMotherboardInService.setFanSpeedByTemp(sensors.MBTemperature);
            fanMotherboardOutService.setFanSpeedByTemp(sensors.MBTemperature);
            fanEnclosureService.SetFanSpeedByPercent(70);

            relayService.turnOn();

            sensors.MBInFanSpeedPercent = fanSpeedPercent;
            sensors.MBOutFanSpeedPercent = fanSpeedPercent;
            sensors.EnclosureFanSpeedPercent = 70;
            sensors.EnclosureFanExpectedRPM = 70 * fanEnclosureConfig.maxRpm / 100;
            break;

        case PrinterState::UNKNOWN:
            // Change nothing
            break;
        default:
            // Change nothing
            break;
        }

        sensors.MBInFanRPM = fanMotherboardInService.getRpm();
        sensors.MBOutFanRPM = fanMotherboardOutService.getRpm();
        sensors.EnclosureFanRPM = fanEnclosureService.getRpm();

        if (SHOW_LOGS)
        {
            Serial.printf("Expected MB fan RPM: %d | Expected enclosure fan RPM: %d | MB in fan RPM: %d | MB out fan RPM: %d | Enclosure fan RPM: %d\n",
                          sensors.MBFanExpectedRPM, sensors.EnclosureFanExpectedRPM, sensors.MBInFanRPM, sensors.MBOutFanRPM, sensors.EnclosureFanRPM);
        }
    }
}
