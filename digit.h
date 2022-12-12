#ifndef _H_DIGIT_
#define _H_DIGIT_

#include <stdio.h>
#include <stdbool.h>
#include "common.h"

#define DIGIT_CARDINAL_NUMBER 10

typedef struct _Digit {
    unsigned char _value;
} Digit;

void digitPrint(Digit);
bool digitIsZero(Digit);
bool digitIsOne(Digit);
int digitWithWeight(Digit, int);
int digitCompareTo(Digit, Digit);
Digit newDigit(int value);
Digit digitZero();
Digit digitOne();
Digit digitRandom();
Digit digitAdd(Digit, Digit, int *);
Digit digitSub(Digit, Digit, int *);
Digit digitMul(Digit, Digit, int *);

void digitPrint(Digit this) {
    printf("%d", this._value);
}

bool digitIsZero(Digit this) {
    return !this._value;
}

bool digitIsOne(Digit this) {
    return this._value == 1;
}

int digitWithWeight(Digit this, int weight) {
    int result = this._value;
    for (int i = 0; i < weight; i++) result *= DIGIT_CARDINAL_NUMBER;
    return result;
}

int digitCompareTo(Digit this, Digit other) {
    return this._value - other._value;
}

Digit newDigit(int value) {
    if (value < 0 || value >= DIGIT_CARDINAL_NUMBER) throwException("newDigitに%dが渡されました。", value);
    Digit result = { ._value = value };
    return result;
}

Digit digitZero() {
    return newDigit(0);
}

Digit digitOne() {
    return newDigit(1);
}

Digit digitRandom() {
    return newDigit(randomRangeInt(0, DIGIT_CARDINAL_NUMBER));
}

Digit digitAdd(Digit this, Digit other, int *carry) {
    int sum = this._value + other._value + *carry;
    *carry = euclideanDivInt(sum, DIGIT_CARDINAL_NUMBER);
    return newDigit(euclideanModInt(sum, DIGIT_CARDINAL_NUMBER));
}

Digit digitSub(Digit this, Digit other, int *carry) {
    int diff = this._value - other._value + *carry;
    *carry = euclideanDivInt(diff, DIGIT_CARDINAL_NUMBER);
    return newDigit(euclideanModInt(diff, DIGIT_CARDINAL_NUMBER));
}

Digit digitMul(Digit this, Digit other, int *carry) {
    int pro = this._value * other._value + *carry;
    *carry = euclideanDivInt(pro, DIGIT_CARDINAL_NUMBER);
    return newDigit(euclideanModInt(pro, DIGIT_CARDINAL_NUMBER));
}

#endif