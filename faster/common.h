#ifndef _H_COMMON_
#define _H_COMMON_

#include <stdlib.h>
#include <stdbool.h>

float randomRatio();
int randomRangeInt(int, int);
bool probablityIsHit(float);
int intAbs(int);

float randomRatio() {
    return rand() / (RAND_MAX + 1.0f);
}

int randomRangeInt(int src, int dst) {
    return src + (dst - src) * randomRatio();
}

bool probabilityIsHit(float probability) {
    return randomRatio() < probability;
}

int intAbs(int value) {
    return value < 0 ? -value : value;
}

#endif
