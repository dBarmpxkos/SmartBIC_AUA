#include "generic_functions.h"

bool sort(int32_t *inputArr, uint8_t cnt) {
    for (uint8_t i = 0; i < cnt-1; i++) {
        bool flag = true;
        for (uint8_t j = 1; j < cnt-i; j++) {
            if (inputArr[j-1] > inputArr[j]) {
                int32_t t = inputArr[j-1];
                inputArr[j-1] = inputArr[j];
                inputArr[j] = t;
                flag = false;
            }
        }
        if (flag) break;
    }
    return true;
}

int32_t get_median(int32_t *inputArr, uint8_t cnt) {
    sort(inputArr, cnt);
    if (cnt > 0){
        if (cnt & 0x01) return inputArr[cnt/2];
        else            return (inputArr[cnt/2] + inputArr[cnt/2 - 1]) / 2;
    } else return 0;
}
