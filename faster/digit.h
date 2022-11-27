#ifndef _H_DIGIT_
#define _H_DIGIT_

#include <stdio.h>

#define DIGIT_MIN 0
#define DIGIT_MAX 9

typedef struct _Digit {
    char _value;
} Digit;

void digitPrint(Digit);
int digitCompareTo(Digit, Digit);
int tryIntToDigit(Digit *, int);
int digitToInt(Digit);
Digit digitZero();
Digit digitRandom();
Digit digitAdd(Digit, Digit, int *);
Digit digitSub(Digit, Digit, int *);
Digit digitMul(Digit, Digit, int *);

void digitPrint(Digit digit) {
    printf("%d", digit._value);
}

int digitCompareTo(Digit this, Digit other) {
    if (this._value < other._value) return -1;
    if (this._value > other._value) return 1;
    return 0;
}

int tryIntToDigit(Digit *dst, int src) {
    if (src < DIGIT_MIN || src > DIGIT_MAX) return 0;
    dst->_value = src;
    return 1;
}

int digitToInt(Digit this) {
    return this._value;
}

Digit digitZero() {
    Digit result = { ._value = 0 };
    return result;
}

Digit digitRandom() {
    Digit result = { ._value = randomRangeInt(DIGIT_MIN, DIGIT_MAX + 1) };
    return result;
}

Digit digitAdd(Digit this, Digit other, int *carry) {
    int sum = this._value + other._value + *carry;
    int rem = sum % (DIGIT_MAX + 1);
    *carry = (sum < 0 ? sum - DIGIT_MAX : sum) / (DIGIT_MAX + 1);
    Digit result = { ._value = rem < 0 ? rem + 10 : rem };
    return result;
}

Digit digitSub(Digit this, Digit other, int *carry) {
    int diff = this._value - other._value + *carry;
    int rem = diff % (DIGIT_MAX + 1);
    *carry = (diff < 0 ? diff - DIGIT_MAX : diff) / (DIGIT_MAX + 1);
    Digit result = { ._value = rem < 0 ? rem + 10 : rem };
    return result;
}

Digit digitMul(Digit this, Digit other, int *carry) {
    int pro = this._value * other._value + *carry;
    int rem = pro % (DIGIT_MAX + 1);
    *carry = (pro < 0 ? pro - DIGIT_MAX : pro) / (DIGIT_MAX + 1);
    Digit result = { ._value = rem < 0 ? rem + 10 : rem };
    return result;
}

#endif
