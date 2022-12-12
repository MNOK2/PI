#ifndef _H_COMMON_
#define _H_COMMON_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

void throwException(const char *, ...);
bool probabilityIsHit(double);
int intAbs(int);
int randomRangeInt(int, int);
int euclideanDivInt(int, int);
int euclideanModInt(int, int);
double randomRatio();
double doubleAbs(double);
double randomRangeDouble(double, double);

void throwException(const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    vprintf(format, ap);
    va_end(ap);
    putchar('\n');
    exit(1);
}

bool probabilityIsHit(double probability) {
    return randomRatio() < probability;
}

int intAbs(int this) {
    return this < 0 ? -this : this;
}

int randomRangeInt(int min, int max) {
    if (min >= max) throwException("randomRangeIntの範囲が[%d, %d)をとりました。", min, max);
    return min + (max - min) * randomRatio();
}

int euclideanDivInt(int this, int other) {
    return (this < 0 ? this - other + 1 : this) / other;
}

int euclideanModInt(int this, int other) {
    int rem = this % other;
    return rem < 0 ? rem + other : rem;
}

double randomRatio() {
    return rand() / (RAND_MAX + 1.0);
}

double doubleAbs(double value) {
    return value < 0.0 ? -value : value;
}

double randomRangeDouble(double min, double max) {
    if (min >= max) throwException("randomRangeDoubleの範囲が[%.2e, %.2e)をとりました。", min, max);
    return min + (max - min) * randomRatio();
}

#endif