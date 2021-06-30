#include "main.h"

void IRAM_ATTR measure_ISR() {
    portENTER_CRITICAL_ISR(&timerMUX);
    measure = true;
    portEXIT_CRITICAL_ISR(&timerMUX);
}

void setup() {
    Serial.begin(115200);
    start_clock(MCP_CLK_PIN, MCP_FREQ); /* clock pulse to MCP CLKIN pin */

    Serial.print(logo);
    Serial.print(firmwareNFO);

    if (state == sConfig) {
        MCP3912.setup_MCP_configuration(activeConf);
        MCP3912.write_MCP_configuration(activeConf);
        MCP3912.read_MCP_configuration();
        state = sSample;
    }

    start_timer(0, 80, true, 2000000, true, true);
}

void loop() {
    if (measure && state == sSample) {
        portENTER_CRITICAL(&timerMUX);
        for (auto &sample : samples) {
            sample[0] = MCP3912.read_single_value(0);
            sample[1] = MCP3912.read_single_value(1);
            sample[2] = MCP3912.read_single_value(2);
            sample[3] = MCP3912.read_single_value(3);
        }
        measure = false;
        portEXIT_CRITICAL(&timerMUX);
        state = sProcess;
    }

    if (state == sProcess) {
        for (auto &sample : samples) {
            Serial.println(sample[0]);
        }
        int32_t testArray[] = {-5,-4,-3,-2,-1,1,2,3,4,5};
        MedianFilter<int32_t, 10> filter;
        for (auto &testSample : testArray){
            filter.addSample(testSample);
        }
        Serial.println(filter.getMedian());
        state = sTransmit;
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

void start_timer(uint8_t timerNum, uint16_t divider, bool countUp, uint64_t interruptAt, bool autoReload, bool enable) {
    if (enable) {
        timer = timerBegin(timerNum, divider, countUp);
        timerAttachInterrupt(timer, &measure_ISR, true);
        timerAlarmWrite(timer, interruptAt, autoReload);
        timerAlarmEnable(timer);
    } else if (timerAlarmEnabled(timer)) timerAlarmDisable(timer);

}