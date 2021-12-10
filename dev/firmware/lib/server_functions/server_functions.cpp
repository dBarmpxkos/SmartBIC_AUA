#include "server_functions.h"

extern volatile bool activeADC;
extern unsigned long measureDelay;
extern int32_t filtered[4];
/* WiFi */
char ssid[] = "SmartBIC";
char password[] = "smartbic";

IPAddress localIP(192,168,1,99);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
AsyncWebServer RESTServer(80);

/* API */
const char startSampling[]    = "/v1/start";
const char stopSampling[]     = "/v1/stop";
const char singleShot[]       = "/v1/single";
const char range[]            = "/v1/range";
const char experiment[]       = "/v1/exp";
const char PARAM_DUR[]        = "duration";
const char PARAM_INTV[]       = "interval";
const char PARAM_RAW[]        = "matrix";

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
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    DynamicJsonDocument json(1024);

    json["ch1"] = filtered[0];
    json["ch2"] = filtered[1];
    json["ch3"] = filtered[2];
    json["ch4"] = filtered[3];
    serializeJson(json, *response);

    request->send(response);
}
void experiment_program(AsyncWebServerRequest *request){
    int paramsNr = request->params();

    for(int i=0;i<paramsNr;i++){
        AsyncWebParameter* p = request->getParam(i);
        if (strcmp(p->name().c_str(), PARAM_INTV) == 0) {
            measureDelay = p->value().toInt();
        }
        else if (strcmp(p->name().c_str(), PARAM_INTV) == 0) {
            measureDelay = p->value().toInt();
        }
    }
    request->send(200, "text/plain", "message received");
}

void setup_endpoints(){

    RESTServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "SmartBiC RESTful API v1");
    });

    RESTServer.on(startSampling,HTTP_GET, start_sampling);
    RESTServer.on(stopSampling,HTTP_GET, stop_sampling);
    RESTServer.on(singleShot,HTTP_GET, single_shot);
    RESTServer.on(experiment, HTTP_GET, experiment_program);

    RESTServer.onNotFound(not_found);
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    RESTServer.begin();
}