#ifndef FIRMWARE_MAIN_H
#define FIRMWARE_MAIN_H

/* includes */
#include "Arduino.h"
#include <WiFi.h>
#include <WebServer.h>
#include <MCP3912.h>
#include "sauce.h"
#include "driver/ledc.h"
#include "../lib/generic_functions/generic_functions.h"
/* end of includes */

#if CONFIG_FREERTOS_UNICORE
    static const BaseType_t app_cpu = 0;
#else
    static const BaseType_t app_cpu = 1;
#endif

/* variables */
MCP3912 MCP3912(MCP_CS_PIN, MCP_DR_PIN);
ADCConfig activeConf;       /* this can later be exposed to user settings via GUI */

static int measureDelay = 1000;     /* default sample rate to 1 s */
const int16_t RAW_SAMPLE_SIZE = 20;
int32_t samples[RAW_SAMPLE_SIZE][4] = {};
int32_t filtered[4];

char* ssid = "SmartBIC";
char* password = "zmartBIKZ";
IPAddress localIP(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
WebServer RESTServer(80);

/* PFP */
void start_clock(uint8_t clk_pin, uint32_t frequency);
void setup_AP(char *SSID, char *PWD, const IPAddress& softlocalIP, IPAddress softGateway, IPAddress softSubnet);
void setup_endpoints();
/* end of PFP */

#endif //FIRMWARE_MAIN_H
