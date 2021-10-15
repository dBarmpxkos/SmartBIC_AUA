#include "server_functions.h"

extern volatile bool activeADC;

/* WiFi */
char ssid[] = "SmartBIC";
char password[] = "zmartBIKZ";

IPAddress localIP(192,168,1,99);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
AsyncWebServer RESTServer(80);

/* API */
char startSampling[] = "/v1/start";
char stopSampling[] = "/v1/stop";

void setup_AP(char *SSID, char *PWD,
              const IPAddress &softlocalIP, const IPAddress &softGateway, const IPAddress &softSubnet){
    do { /* setup AP */ } while (!WiFi.softAP(SSID, PWD));
    do { /* setup AP */ } while (!WiFi.softAPConfig(softlocalIP, softGateway, softSubnet));
}

void not_found(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void setup_endpoints(){

    RESTServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "Hello, world");
    });

    RESTServer.onNotFound(not_found);
    RESTServer.begin();
}