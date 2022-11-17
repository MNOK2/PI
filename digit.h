#ifndef _H_DIGIT_
#define _H_DIGIT_

#include "system.h"

typedef struct _Digit {
    char _value;
} Digit;

Digit newDigit(char value);
Digit digitAdd(Digit a, Digit b, int *carry);
Digit digitSub(Digit a, Digit b, int *carry);
Digit digitMul(Digit a, Digit b, int *carry);
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

Digit digitAdd(Digit a, Digit b, int *carry) {
    int sum = a._value + b._value + *carry;
    *carry = sum / 10 - (sum < 0 ? 1 : 0);
    return newDigit(sum % 10 < 0 ? sum % 10 + 10 : sum % 10);
}

Digit digitSub(Digit a, Digit b, int *carry) {
    int diff = a._value - b._value + *carry;
    *carry = diff / 10 - (diff < 0 ? 1 : 0);
    return newDigit(diff % 10 < 0 ? diff % 10 + 10 : diff % 10);
}

Digit digitMul(Digit a, Digit b, int *carry) {
    int pro = a._value * b._value + *carry;
    *carry = pro / 10 - (pro < 0 ? 1 : 0);
    return newDigit(pro % 10 < 0 ? pro % 10 + 10 : pro % 10);
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
