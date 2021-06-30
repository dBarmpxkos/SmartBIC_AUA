#include "generic_functions.h"

void return_median_to_var(int32_t rawSamples[][], int32_t filteredOut[4]) {
    MedianFilter<int32_t, 1000> filter;
    for (uint8_t col = 0; col<4; col++) {
        for (uint16_t sample = 0; sample < 1000; sample++) {
            filter.addSample(rawSamples[sample][col]);
        }
        filteredOut[col] = filter.getMedian();
    }
}
