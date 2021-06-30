#ifndef GENERIC_FUNCTIONS_H
#define GENERIC_FUNCTIONS_H

#include <cstdint>

#define MEDIAN_FILTER_SIZE   3
bool sort(int32_t *inputArr, uint8_t cnt);
int32_t get_median(int32_t *inputArr, uint8_t cnt);

#endif //FIRMWARE_GENERIC_FUNCTIONS_H
