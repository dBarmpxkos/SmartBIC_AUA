
#ifndef SERVER_FUNCTIONS_H
#define SERVER_FUNCTIONS_H

#include <WiFi.h>
#include <WebServer.h>

/* WiFi */
extern char ssid[];
extern char password[];

extern IPAddress localIP;
extern IPAddress gateway;
extern IPAddress subnet;
extern WebServer RESTServer;

/* API */
extern char startSampling[];
extern char stopSampling[];

/* PFP */
void setup_AP(char *SSID, char *PWD,
              const IPAddress &softlocalIP, const IPAddress &softGateway, const IPAddress &softSubnet);
void options_response();
void handle_notFound();
void setup_endpoints();

#endif //FIRMWARE_SERVER_FUNCTIONS_H
