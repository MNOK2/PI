#ifndef _H_DIGIT_
#define _H_DIGIT_

#include <stdbool.h>

typedef struct _Digit {
    char _value;
} Digit;

Digit digitZero();
char digitToChar(Digit);
bool tryCharToDigit(char, Digit *);
bool digitEquals(Digit, Digit);
bool digitIsLess(Digit, Digit);
bool digitIsGreater(Digit, Digit);
bool digitIsLessEquals(Digit, Digit);
bool digitIsGreaterEquals(Digit, Digit);
bool tryDigitAdd(Digit, Digit, Digit *, int *);
bool tryDigitSub(Digit, Digit, Digit *, int *);
bool tryDigitMul(Digit, Digit, Digit *, int *);

Digit digitZero() {
    Digit result = { ._value = 0 };
    return result;
}

char digitToChar(Digit digit) {
    return digit._value;
}

bool tryCharToDigit(char value, Digit *result) {
    if (value < 0 || value > 9) return false;
    result->_value = value;
    return true;
}

bool digitEquals(Digit src, Digit dst) {
    return src._value == dst._value;
}

bool digitIsLess(Digit src, Digit dst) {
    return src._value < dst._value;
}

bool digitIsGreater(Digit src, Digit dst) {
    return src._value > dst._value;
}

bool digitIsLessEquals(Digit src, Digit dst) {
    return src._value <= dst._value;
}

bool digitIsGreaterEquals(Digit src, Digit dst) {
    return src._value >= dst._value;
}

bool tryDigitAdd(Digit a, Digit b, Digit *result, int *carry) {
    int sum = a._value + b._value + *carry;
    int mod = sum % 10;
    *carry = (sum < 0 ? sum - 9 : sum) / 10;
    if (!tryCharToDigit(mod < 0 ? mod + 10 : mod, result)) return false;
    return true;
}

bool tryDigitSub(Digit a, Digit b, Digit *result, int *carry) {
    int diff = a._value - b._value + *carry;
    int mod = diff % 10;
    *carry = (diff < 0 ? diff - 9 : diff) / 10;
    if (!tryCharToDigit(mod < 0 ? mod + 10 : mod, result)) return false;
    return true;
}

bool tryDigitMul(Digit a, Digit b, Digit *result, int *carry) {
    int pro = a._value * b._value + *carry;
    int mod = pro % 10;
    *carry = (pro < 0 ? pro - 9 : pro) / 10;
    if (!tryCharToDigit(mod < 0 ? mod + 10 : mod, result)) return false;
    return true;
}

#endif
