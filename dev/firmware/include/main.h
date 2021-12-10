#ifndef FIRMWARE_MAIN_H
#define FIRMWARE_MAIN_H

/* includes */
#include "Arduino.h"
#include <MCP3912.h>
#include "sauce.h"
#include "driver/ledc.h"
#include "../lib/generic_functions/generic_functions.h"
#include "server_functions.h"
/* end of includes */

/* variables */
MCP3912 MCP3912(MCP_CS_PIN, MCP_DR_PIN);
ADCConfig activeConf;                  /* this can later be exposed to user settings via GUI */

unsigned long measureDelay = 1000;     /* default sample rate to 1 s */
unsigned long previousMillis = 0;
const int16_t RAW_SAMPLE_SIZE = 20;
int32_t samples[RAW_SAMPLE_SIZE][4] = {};
int32_t filtered[4];
volatile bool activeADC = true;

/* PFP */
void start_clock(uint8_t clk_pin, uint32_t frequency);
/* end of PFP */

#endif //FIRMWARE_MAIN_H
