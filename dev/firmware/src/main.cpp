#include "main.h"

MCP3912 MCP3912(MCP_CS_PIN, MCP_DR_PIN);
ADCConfig activeConf; /* this can later be exposed to user settings */

void setup() {
    Serial.begin(115200);
    start_clock(MCP_CLK_PIN, MCP_FREQ); /* clock pulse to MCP CLKIN pin */

    Serial.print(logo);
    Serial.print(firmwareNFO);

    MCP3912.setup_MCP_configuration(activeConf);
    MCP3912.write_MCP_configuration(activeConf);
    MCP3912.read_MCP_configuration();

}

void loop() {
    bool hasNewData = MCP3912.mcp_data_ready(0x0C);  /* 0x0C STATUSCOM register */
    while (!hasNewData) { /* waits */ }
    if (hasNewData) {
        Serial.printf("\r\nCH0: %i\tCH1: %i\tCH2: %i\tCH3: %i",
                      MCP3912.read_single_value(0),
                      MCP3912.read_single_value(1),
                      MCP3912.read_single_value(2),
                      MCP3912.read_single_value(3));
    }

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