#include "server_functions.h"

extern volatile bool activeADC;
extern int32_t filtered[4];
/* WiFi */
char ssid[] = "SmartBIC";
char password[] = "zmartBIKZ";

IPAddress localIP(192,168,1,99);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
AsyncWebServer RESTServer(80);

/* API */
char startSampling[]    = "/v1/start";
char stopSampling[]     = "/v1/stop";
char singleShot[]       = "/v1/single";
char range[]            = "/v1/all";

void setup_AP(char *SSID, char *PWD,
              const IPAddress &softlocalIP, const IPAddress &softGateway, const IPAddress &softSubnet){
    do { /* setup AP */ } while (!WiFi.softAP(SSID, PWD));
    do { /* setup AP */ } while (!WiFi.softAPConfig(softlocalIP, softGateway, softSubnet));
}

void not_found(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void start_sampling(AsyncWebServerRequest *request){
    activeADC = true;
    AsyncResponseStream *response = request->beginResponseStream("text/html");
    response->addHeader("Server","SmartBiC");
    response->printf("started sampling");
    request->send(response);
}

void stop_sampling(AsyncWebServerRequest *request){
    activeADC = false;
    AsyncResponseStream *response = request->beginResponseStream("text/html");
    response->addHeader("Server","SmartBiC");
    response->printf("stopped sampling");
    request->send(response);
}

void single_shot(AsyncWebServerRequest *request){
    AsyncResponseStream *response = request->beginResponseStream("text/html");
    response->addHeader("Server","SmartBiC");
    response->printf("%d, %d, %d, %d", filtered[0], filtered[1], filtered[2], filtered[3]);
    request->send(response);
}

void batch(AsyncWebServerRequest *request){
    AsyncResponseStream *response = request->beginResponseStream("text/html");
    response->addHeader("Server","SmartBiC");
    response->printf("%d, %d, %d, %d", filtered[0], filtered[1], filtered[2], filtered[3]);
    request->send(response);
}


void setup_endpoints(){

    RESTServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "SmartBiC RESTful API v1");
    });

    RESTServer.on(singleShot,HTTP_GET, single_shot);
    RESTServer.on(range,HTTP_GET, batch);

    RESTServer.onNotFound(not_found);
    RESTServer.begin();
}