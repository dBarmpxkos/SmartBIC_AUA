
#include <MCP3912.h>
#include "Arduino.h"
#include "driver/ledc.h"

#define PULSE_OUTPUT_PIN 16
MCP3912 POWERADC(5, 17);


void setup() {
    Serial.begin(115200);

    ledc_timer_config_t ledc_timer;
    ledc_channel_config_t ledc_channel;

    ledc_timer.speed_mode = LEDC_HIGH_SPEED_MODE;
    ledc_timer.timer_num = LEDC_TIMER_0;
    ledc_timer.duty_resolution = (ledc_timer_bit_t) 1;
    ledc_timer.freq_hz = 16000000;

    ledc_channel.channel = LEDC_CHANNEL_0;
    ledc_channel.gpio_num = PULSE_OUTPUT_PIN;
    ledc_channel.speed_mode = LEDC_HIGH_SPEED_MODE;
    ledc_channel.timer_sel = LEDC_TIMER_0;
    ledc_channel.duty = 1;

    ledc_timer_config(&ledc_timer);
    ledc_channel_config(&ledc_channel);

    POWERADC.Configuration(3,0,255,2,0,0,0,0);
    delay(5000);
    POWERADC.ReadConfiguration();
}

void loop() {
    Serial.printf("CH0: %i\tCH1: %i\tCH2: %i\tCH3: %i\r\n",
                  POWERADC.ReadSingleValue(0),
                  POWERADC.ReadSingleValue(1),
                  POWERADC.ReadSingleValue(2),
                  POWERADC.ReadSingleValue(3));
}