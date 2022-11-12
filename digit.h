#ifndef _H_DIGIT_
#define _H_DIGIT_

#include "system.h"

typedef struct _Digit {
    unsigned char _value;
} Digit;

Digit newDigit(char value);
Digit digitAdd(Digit a, Digit b);
Digit digitSub(Digit a, Digit b);
char digitAddCarry(Digit a, Digit b);
char digitSubCarry(Digit a, Digit b);
char digitEquals(Digit a, Digit b);
char digitIsLess(Digit a, Digit b);
char digitIsGreater(Digit a, Digit b);
char digitIsLessEquals(Digit a, Digit b);
char digitIsGreaterEquals(Digit a, Digit b);
char digitToChar(Digit digit);

Digit newDigit(char value) {
    if (value < 0 || value > 9) throwExceptionIllegalArgument("newDigit");

    Digit result = { ._value = value };
    return result;
}

Digit digitAdd(Digit a, Digit b) {
    return newDigit((a._value + b._value) % 10);
}

Digit digitSub(Digit a, Digit b) {
    return newDigit(((a._value - b._value) < 0 ? (a._value - b._value + 10) : (a._value - b._value)) % 10);
}

char digitAddCarry(Digit a, Digit b) {
    return (a._value + b._value) / 10;
}

char digitSubCarry(Digit a, Digit b) {
    return ((a._value - b._value) < 0 ? (a._value - b._value - 10) : (a._value - b._value)) / 10;
}

char digitEquals(Digit a, Digit b) {
    return a._value == b._value;
}

char digitIsLess(Digit a, Digit b) {
    return a._value < b._value;
}

char digitIsGreater(Digit a, Digit b) {
    return a._value > b._value;
}

char digitIsLessEquals(Digit a, Digit b) {
    return a._value <= b._value;
}

char digitIsGreaterEquals(Digit a, Digit b) {
    return a._value >= b._value;
}

char digitToChar(Digit digit) {
    return digit._value;
}

#endif
