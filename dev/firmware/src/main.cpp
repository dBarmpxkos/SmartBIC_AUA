#include "main.h"

/* FreeRTOS tasks */
void taskMeasure(void *parameters){ /* samples ADC */
    for (;;){
        vTaskDelay(measureDelay / portTICK_PERIOD_MS);
        while (!MCP3912.mcp_data_ready(0x0C)) { /* waits */ }
        for (auto & sample : samples) {
            sample[0] = MCP3912.read_single_value(0);
            sample[1] = MCP3912.read_single_value(1);
            sample[2] = MCP3912.read_single_value(2);
            sample[3] = MCP3912.read_single_value(3);
        }
        return_median_to_var(samples, filtered);
        Serial.printf("\r\n%i,%i,%i,%i", filtered[0], filtered[1], filtered[2], filtered[3]);
    }
}

void taskAPI(void *parameters){ /* handles server requests */
    for (;;){ RESTServer.handleClient(); }
}

void setup() {
    Serial.begin(115200);
    start_clock(MCP_CLK_PIN, MCP_FREQ); /* clock pulse to MCP CLKIN pin */

    Serial.print(logo);
    Serial.print(firmwareNFO);
    MCP3912.initialize_with_conf(activeConf);   /* felt cute might delete later */

    setup_AP(ssid, password, localIP, gateway, subnet);
    setup_endpoints();

    Serial.print(F("\r\n[SYS]\tSetting up tasks"));
    xTaskCreatePinnedToCore(
            taskMeasure,             /* Function that should be called */
            "Measure",       /* Name of the task (for debugging) */
            1000,        /* Stack size (bytes) */
            NULL,        /* Parameter to pass */
            1,              /* Task priority */
            NULL,       /* Task handle */
            app_cpu                  /* core to run */
    );
    xTaskCreatePinnedToCore(
            taskAPI,                 /* Function that should be called */
            "API",           /* Name of the task (for debugging) */
            1000,        /* Stack size (bytes) */
            NULL,        /* Parameter to pass */
            1,              /* Task priority */
            NULL,       /* Task handle */
            app_cpu                  /* core to run */
    );
    Serial.print(F("\r\n[SYS]\tTask setup done"));

}

void loop() {

}

void start_clock(uint8_t clk_pin, uint32_t frequency) {
    ledc_timer_config_t ledc_timer;
    ledc_channel_config_t ledc_channel;

    ledc_timer.speed_mode = LEDC_HIGH_SPEED_MODE;
    ledc_timer.timer_num = LEDC_TIMER_0;
    ledc_timer.duty_resolution = (ledc_timer_bit_t) 1;
    ledc_timer.freq_hz = frequency;

    ledc_channel.channel = LEDC_CHANNEL_0;
    ledc_channel.gpio_num = clk_pin;
    ledc_channel.speed_mode = LEDC_HIGH_SPEED_MODE;
    ledc_channel.timer_sel = LEDC_TIMER_0;
    ledc_channel.duty = 1;

    ledc_timer_config(&ledc_timer);
    ledc_channel_config(&ledc_channel);
}

void setup_AP(char *SSID, char *PWD, const IPAddress& softlocalIP, IPAddress softGateway, IPAddress softSubnet){
    do { /* setup AP */ } while (!WiFi.softAP(SSID, PWD));
    do { /* setup AP */ } while (!WiFi.softAPConfig(softlocalIP, softGateway, softSubnet));
}

void handle_OnConnect() {
    Serial.println("onConnect");
    RESTServer.send(200, "text/html", "yaw");
}

void setup_endpoints(){
    RESTServer.on("/", handle_OnConnect);
}