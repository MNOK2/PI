#ifndef _H_NUMBER_
#define _H_NUMBER_

#include "sign.h"
#include "digit.h"
#include "string.h"
#include "system.h"

#define NUMBER_DIGITS_COUNT_MAX (1 << 6)

typedef struct _Number {
    Sign _sign;
    Digit _digits[NUMBER_DIGITS_COUNT_MAX];
} Number;

Number newNumber(Sign sign, const Digit *digits);
Number numberZero();
Number numberRandom(int digitsCount);
Number numberAbs(Number number);
Number numberSignReversed(Number number);
Number numberDigitShiftLeft(Number number, int count);
Number numberDigitShiftRight(Number number, int count);
Number numberAdd(Number a, Number b);
Number numberSub(Number a, Number b);
Number numberMul(Number a, Number b);
Number numberDiv(Number a, Number b);
Number numberMod(Number a, Number b);
Number numberMulDigit(Number number, Digit digit);
Digit numberDivDigit(Number a, Number b, Number *mod);
Number numberBeforeIncrement(Number *number);
Number numberAfterIncrement(Number *number);
Number numberBeforeDecrement(Number *number);
Number numberAfterDecrement(Number *number);
char numberIsZero(Number number);
char numberEquals(Number a, Number b);
char numberIsLess(Number a, Number b);
char numberIsGreater(Number a, Number b);
char numberIsLessEquals(Number a, Number b);
char numberIsGreaterEquals(Number a, Number b);
Number intToNumber(int value);
int numberToInt(Number number);
String numberToString(Number number);

Number newNumber(Sign sign, const Digit *digits) {
    Number result;
    result._sign = sign;
    for (int i = 0; i < NUMBER_DIGITS_COUNT_MAX; i++) result._digits[i] = newDigit(digitToChar(digits[i]));
    return result;
}

Number numberZero() {
    Digit digits[NUMBER_DIGITS_COUNT_MAX];
    for (int i = 0; i < NUMBER_DIGITS_COUNT_MAX; i++) digits[i] = newDigit(0);
    return newNumber(signPositive(), digits);
}

Number numberRandom(int digitsCount) {
    if (digitsCount < 0 || digitsCount > NUMBER_DIGITS_COUNT_MAX) throwExceptionIllegalArgument("numberRandom");

    Digit digits[NUMBER_DIGITS_COUNT_MAX];
    for (int i = 0; i < digitsCount; i++) digits[i] = newDigit(randomRangeInt(0, 10));
    for (int i = digitsCount; i < NUMBER_DIGITS_COUNT_MAX; i++) digits[i] = newDigit(0);
    return newNumber(probabilityIsHit(0.5f) ? signNegative() : signPositive(), digits);
}

Number numberAbs(Number number) {
    return newNumber(signPositive(), number._digits);
}

Number numberSignReversed(Number number) {
    return newNumber(signReversed(number._sign), number._digits);
}

Number numberDigitShiftLeft(Number number, int count) {
    if (count < 0 || count > NUMBER_DIGITS_COUNT_MAX) throwExceptionIllegalArgument("numberDigitShiftLeft");

    Digit digits[NUMBER_DIGITS_COUNT_MAX];
    for (int i = 0; i < count; i++) digits[i] = newDigit(0);
    for (int i = count; i < NUMBER_DIGITS_COUNT_MAX; i++) digits[i] = number._digits[i - count];
    return newNumber(number._sign, digits);
}

Number numberDigitShiftRight(Number number, int count) {
    if (count < 0 || count > NUMBER_DIGITS_COUNT_MAX) throwExceptionIllegalArgument("numberDigitShiftRight");

    Digit digits[NUMBER_DIGITS_COUNT_MAX];
    for (int i = 0; i < NUMBER_DIGITS_COUNT_MAX - count; i++) digits[i] = number._digits[i + count];
    for (int i = NUMBER_DIGITS_COUNT_MAX - count; i < NUMBER_DIGITS_COUNT_MAX; i++) digits[i] = newDigit(0);
    return newNumber(number._sign, digits);
}

Number numberAdd(Number a, Number b) {
    if (signEquals(a._sign, signNegative()) && signEquals(b._sign, signPositive())) return numberSub(b, numberSignReversed(a));
    if (signEquals(a._sign, signPositive()) && signEquals(b._sign, signNegative())) return numberSub(a, numberSignReversed(b));
    if (signEquals(a._sign, signNegative())) return numberSignReversed(numberAdd(numberSignReversed(a), numberSignReversed(b)));

    Digit digits[NUMBER_DIGITS_COUNT_MAX];
    int carry = 0;
    for (int i = 0; i < NUMBER_DIGITS_COUNT_MAX; i++) digits[i] = digitAdd(a._digits[i], b._digits[i], &carry);
    return newNumber(signPositive(), digits);
}

Number numberSub(Number a, Number b) {
    if (signEquals(a._sign, signNegative()) && signEquals(b._sign, signPositive())) return numberSignReversed(numberAdd(numberSignReversed(a), b));
    if (signEquals(a._sign, signPositive()) && signEquals(b._sign, signNegative())) return numberAdd(a, numberSignReversed(b));
    if (signEquals(a._sign, signNegative())) return numberSub(numberSignReversed(b), numberSignReversed(a));
    if (numberIsLess(a, b)) return numberSignReversed(numberSub(b, a));

    Digit digits[NUMBER_DIGITS_COUNT_MAX];
    int carry = 0;
    for (int i = 0; i < NUMBER_DIGITS_COUNT_MAX; i++) digits[i] = digitSub(a._digits[i], b._digits[i], &carry);
    return newNumber(signPositive(), digits);
}

Number numberMul(Number a, Number b) {
    if (!signEquals(a._sign, b._sign)) return numberSignReversed(numberMul(numberAbs(a), numberAbs(b)));
    if (signEquals(a._sign, signNegative())) return numberMul(numberSignReversed(a), numberSignReversed(b));

    Number result = numberZero();
    for (int i = 0; i < NUMBER_DIGITS_COUNT_MAX; i++) result = numberAdd(result, numberDigitShiftLeft(numberMulDigit(a, b._digits[i]), i));
    return result;
}

Number numberDiv(Number a, Number b) {
    if (numberIsZero(b)) return numberZero();
    if (!signEquals(a._sign, b._sign)) return numberSignReversed(numberDiv(numberAbs(a), numberAbs(b)));
    if (signEquals(a._sign, signNegative())) return numberDiv(numberSignReversed(a), numberSignReversed(b));

    Digit digits[NUMBER_DIGITS_COUNT_MAX];
    Number mod = a;
    for (int i = 0; i < NUMBER_DIGITS_COUNT_MAX; i++) digits[NUMBER_DIGITS_COUNT_MAX - 1 - i] = numberDivDigit(mod, numberDigitShiftLeft(b, NUMBER_DIGITS_COUNT_MAX - 1 - i), &mod);
    return newNumber(signPositive(), digits);
}

Number numberMod(Number a, Number b) {
    return numberSub(a, numberMul(b, numberDiv(a, b)));
}

Number numberMulDigit(Number number, Digit digit) {
    Digit digits[NUMBER_DIGITS_COUNT_MAX];
    int carry = 0;
    for (int i = 0; i < NUMBER_DIGITS_COUNT_MAX; i++) digits[i] = digitMul(number._digits[i], digit, &carry);
    return newNumber(number._sign, digits);
}

Digit numberDivDigit(Number a, Number b, Number *mod) {
    if (signEquals(a._sign, signNegative()) || signEquals(b._sign, signNegative())) throwExceptionIllegalArgument("numberDivDigit");

    for (int i = 1; i < 10; i++) {
        if (numberIsLess(a, numberMulDigit(b, newDigit(i)))) {
            *mod = numberSub(a, numberMulDigit(b, newDigit(i - 1)));
            return newDigit(i - 1);
        }
    }
    *mod = numberSub(a, numberMulDigit(b, newDigit(9)));
    return newDigit(9);
}

Number numberBeforeIncrement(Number *number) {
    Number result = *number;
    *number = numberAdd(*number, intToNumber(1));
    return result;
}

Number numberAfterIncrement(Number *number) {
    *number = numberAdd(*number, intToNumber(1));
    return *number;
}

Number numberBeforeDecrement(Number *number) {
    Number result = *number;
    *number = numberSub(*number, intToNumber(1));
    return result;
}

Number numberAfterDecrement(Number *number) {
    *number = numberSub(*number, intToNumber(1));
    return *number;
}

char numberIsZero(Number number) {
    for (int i = 0; i < NUMBER_DIGITS_COUNT_MAX; i++) if (!digitEquals(number._digits[i], newDigit(0))) return 0;
    return 1;
}

char numberEquals(Number a, Number b) {
    if (numberIsZero(a) && numberIsZero(b)) return 1;
    if (!signEquals(a._sign, b._sign)) return 0;

    for (int i = 0; i < NUMBER_DIGITS_COUNT_MAX; i++) if (!digitEquals(a._digits[i], b._digits[i])) return 0;
    return 1;
}

char numberIsLess(Number a, Number b) {
    if (numberIsZero(a) && numberIsZero(b)) return 0;
    if (signEquals(a._sign, signNegative()) && signEquals(b._sign, signPositive())) return 1;
    if (signEquals(a._sign, signPositive()) && signEquals(b._sign, signNegative())) return 0;
    if (signEquals(a._sign, signNegative())) return numberIsLess(numberSignReversed(b), numberSignReversed(a));

    for (int i = 0; i < NUMBER_DIGITS_COUNT_MAX; i++) {
        if (digitIsLess(a._digits[NUMBER_DIGITS_COUNT_MAX - 1 - i], b._digits[NUMBER_DIGITS_COUNT_MAX - 1 - i])) return 1;
        if (digitIsGreater(a._digits[NUMBER_DIGITS_COUNT_MAX - 1 - i], b._digits[NUMBER_DIGITS_COUNT_MAX - 1 - i])) return 0;
    }
    return 0;
}

char numberIsGreater(Number a, Number b) {
    return numberIsLess(b, a);
}

char numberIsLessEquals(Number a, Number b) {
    return !numberIsGreater(a, b);
}

char numberIsGreaterEquals(Number a, Number b) {
    return !numberIsLess(a, b);
}

Number intToNumber(int value) {
    Digit digits[NUMBER_DIGITS_COUNT_MAX];
    int temp = value;
    for (int i = 0; i < NUMBER_DIGITS_COUNT_MAX; i++) {
        digits[i] = newDigit(intAbs(temp % 10));
        temp /= 10;
    }
    return newNumber(value < 0 ? signNegative() : signPositive(), digits);
}

int numberToInt(Number number) {
    int result = 0;
    int temp = 1;
    for (int i = 0; i < NUMBER_DIGITS_COUNT_MAX; i++) {
        result += digitToChar(number._digits[i]) * temp;
        temp *= 10;
    }
    return signEquals(number._sign, signNegative()) ? -result : result;
}

String numberToString(Number number) {
    if (numberIsZero(number)) return stringPrintf("0");

    String result;
    char zeroOngoing = 1;

    result = stringPrintf(signToCharP(number._sign));
    for (int i = 0; i < NUMBER_DIGITS_COUNT_MAX; i++) {
        if (!digitEquals(number._digits[NUMBER_DIGITS_COUNT_MAX - 1 - i], newDigit(0))) zeroOngoing = 0;
        if (!zeroOngoing) result = stringPrintf("%s%d%s", result.value, digitToChar(number._digits[NUMBER_DIGITS_COUNT_MAX - 1 - i]), i < NUMBER_DIGITS_COUNT_MAX - 1 ? " " : "");
    }
    return result;
}

#endif
