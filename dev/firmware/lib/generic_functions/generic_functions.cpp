#include "generic_functions.h"

extern const int16_t RAW_SAMPLE_SIZE = 20;
MedianFilter<int32_t, RAW_SAMPLE_SIZE> filter;

void return_median_to_var(int32_t rawSamples[RAW_SAMPLE_SIZE][4], int32_t filteredOut[4]) {
    for (uint8_t col = 0; col<4; col++) {
        for (uint16_t sample = 0; sample < RAW_SAMPLE_SIZE; sample++) {
            filter.addSample(rawSamples[sample][col]);
        }
        filteredOut[col] = filter.getMedian();
    }
}
