
#ifndef SERVER_FUNCTIONS_H
#define SERVER_FUNCTIONS_H

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

/* WiFi */
extern char ssid[];
extern char password[];

extern IPAddress localIP;
extern IPAddress gateway;
extern IPAddress subnet;
extern AsyncWebServer RESTServer;

/* PFP */
void setup_AP(char *SSID, char *PWD,
              const IPAddress &softlocalIP, const IPAddress &softGateway, const IPAddress &softSubnet);

void not_found(AsyncWebServerRequest *request);
void single_shot(AsyncWebServerRequest *request);
void experiment_program(AsyncWebServerRequest *request);

void setup_endpoints();

#endif //FIRMWARE_SERVER_FUNCTIONS_H
