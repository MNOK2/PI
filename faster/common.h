#ifndef _H_COMMON_
#define _H_COMMON_

#include <stdlib.h>

int intAbs(int);
int randomRangeInt(int, int);
int probabilityIsHit(float);
float randomRatio();

int intAbs(int value) {
    return value < 0 ? -value : value;
}

int randomRangeInt(int this, int other) {
    return this + (other - this) * randomRatio();
}

int probabilityIsHit(float probability) {
    return randomRatio() < probability;
}

float randomRatio() {
    return rand() / (RAND_MAX + 1.0f);
}

#endif
