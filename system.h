#ifndef _H_SYSTEM_
#define _H_SYSTEM_

#include <stdio.h>
#include <stdlib.h>

void throwException(const char *memo);
void throwExceptionIllegalArgument(const char *memo);
int intAbs(int value);
float randomRatio();
int randomRangeInt(int min, int max);
char probabilityIsHit(float probability);

void throwException(const char *memo) {
    printf("%s: 例外が発生しました。\n", memo);
    exit(1);
}

void throwExceptionIllegalArgument(const char *memo) {
    printf("%s: 許容範囲外の引数が渡されました。\n", memo);
    exit(1);
}

int intAbs(int value) {
    return value < 0 ? -value : value;
}

float randomRatio() {
    return rand() / (RAND_MAX + 1.0f);
}

int randomRangeInt(int min, int max) {
    if (min >= max) throwExceptionIllegalArgument("randomRangeInt");
    return min + (max - min) * randomRatio();
}

char probabilityIsHit(float probability) {
    return randomRatio() < probability;
}

#endif
