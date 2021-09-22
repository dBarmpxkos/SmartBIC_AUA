#include "server_functions.h"

extern volatile bool activeADC;

/* WiFi */
char ssid[] = "SmartBIC";
char password[] = "zmartBIKZ";

IPAddress localIP(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
WebServer RESTServer(80);

/* API */
char startSampling[] = "/v1/start";
char stopSampling[] = "/v1/stop";

void setup_AP(char *SSID, char *PWD,
              const IPAddress &softlocalIP, const IPAddress &softGateway, const IPAddress &softSubnet){
    do { /* setup AP */ } while (!WiFi.softAP(SSID, PWD));
    do { /* setup AP */ } while (!WiFi.softAPConfig(softlocalIP, softGateway, softSubnet));
}

void options_response(){
    RESTServer.sendHeader("Allow","GET,POST,PUT,PATCH,DELETE,HEAD,OPTIONS");
    RESTServer.sendHeader("Access-Control-Allow-Origin", "*");
    RESTServer.sendHeader("Access-Control-Allow-Methods", "GET,POST,PUT,PATCH,DELETE,HEAD,OPTIONS");
    RESTServer.sendHeader("Access-Control-Allow-Headers", "*");
    RESTServer.send(200, "text/plain");
}

void handle_notFound(){
    RESTServer.sendHeader("Access-Control-Allow-Origin", "*");
    RESTServer.send(404, "text/plain", "OHMAGAWRD");
}

void handle_onStartMeasure(){
    activeADC = !activeADC;
}

void setup_endpoints(){
    RESTServer.on("/", HTTP_GET, []() {
        RESTServer.sendHeader("Access-Control-Allow-Origin", "*");
        RESTServer.send(200, "text/html",
                        "SmartBiC");
    });

    RESTServer.onNotFound(handle_notFound);

    RESTServer.on(startSampling, HTTP_OPTIONS, options_response);
    RESTServer.on(startSampling, HTTP_GET, handle_onStartMeasure);

    RESTServer.begin();
}