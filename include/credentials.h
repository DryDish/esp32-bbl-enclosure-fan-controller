#pragma once

// Wifi
const char *WIFI_SSID = "your-wifi-ssid";     // Your wifi network name
const char *WIFI_PASS = "your-wifi-password"; // Your wifi password

// OTA
const char *OTA_HOSTNAME = "hostname-for-ota"; // Hostname for OTA updates
const char *OTA_PASSWORD = "password-for-ota"; // Password for OTA updates

// MQTT
const char *MQTT_SERVER = "192.168.1.1";                     // Printer's local IP address
const char *MQTT_USER = "bblp";                              // The default Bambulab mqtt username
const char *MQTT_PASS = "its-your-printers-lan-access-code"; // The printer's LAN access code
const int MQTT_PORT = 8883;                                  // The default Bambulab mqtt port

// Bambulab info
const char *PRINTER_SERIAL_NUMBER = "03919C452113315"; // The printer's serial number, needed for MQTT
const char *BBL_CERT = "-----BEGIN CERTIFICATE-----\n"
                       "not sure if i can share this,\n"
                       "but it's the cert for bambulab\n"
                       "printers. You can find it online\n"
                       "-----END CERTIFICATE-----";
