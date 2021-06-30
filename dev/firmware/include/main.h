#include "Arduino.h"
#include <MCP3912.h>
#include "sauce.h"
#include "driver/ledc.h"
#include "../lib/generic_functions/generic_functions.h"

#ifndef FIRMWARE_MAIN_H
#define FIRMWARE_MAIN_H

void start_clock(uint8_t clkPin, uint32_t frequency);

#endif //FIRMWARE_MAIN_H
