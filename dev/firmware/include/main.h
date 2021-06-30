#ifndef FIRMWARE_MAIN_H
#define FIRMWARE_MAIN_H

/* includes */
#include "Arduino.h"
#include <MCP3912.h>
#include "sauce.h"
#include "driver/ledc.h"
#include "../lib/generic_functions/generic_functions.h"
/* end of includes */

/* PFP */
void start_clock(uint8_t clkPin, uint32_t frequency);\
void start_timer(uint8_t timerNum, uint16_t divider, bool countUp, uint64_t interruptAt, bool autoReload, bool enable);
/* end of PFP */

/* variables */
typedef enum {
    sConfig,
    sSample,
    sProcess,
    sTransmit
} userFSM ;

userFSM state = sConfig;    /* initial state is to configure ADC */
volatile bool measure;      /* toggled via ISR */
hw_timer_t * timer = NULL;
portMUX_TYPE timerMUX = portMUX_INITIALIZER_UNLOCKED;

MCP3912 MCP3912(MCP_CS_PIN, MCP_DR_PIN);
ADCConfig activeConf;       /* this can later be exposed to user settings via GUI */

int32_t samples[1000][4] = {};
uint16_t sampleCounter = 0;

#endif //FIRMWARE_MAIN_H
