#ifndef _H_NUMBER_
#define _H_NUMBER_

#include <stdio.h>
#include <stdbool.h>
#include "sign.h"
#include "digit.h"
#include "common.h"

#define NUMBER_DIGITS_COUNT_MAX (1 << 10)

typedef struct _Number {
    Sign _sign;
    Digit _digits[NUMBER_DIGITS_COUNT_MAX];
} Number;

void numberPrint(const Number *);
void numberPrintWithName(const Number *, const char *);
void numberMakeZero(Number *);
void numberMakeRandom(int, Number *);
void numberSignReverse(const Number *, Number *);
void numberAbs(const Number *, Number *);
void numberDigitShiftLeft(const Number *, int, Number *);
void numberDigitShiftRight(const Number *, int, Number *);
void numberDigitInsert(const Number *, Digit, Number *);
void intToNumber(int, Number *);
bool tryNumberToInt(const Number *, int *);
bool numberIsZero(const Number *);
bool numberEquals(const Number *, const Number *);
bool numberIsLess(const Number *, const Number *);
bool numberIsGreater(const Number *, const Number *);
bool numberIsLessEquals(const Number *, const Number *);
bool numberIsGreaterEquals(const Number *, const Number *);
bool tryNumberAdd(const Number *, const Number *, Number *);
bool tryNumberSub(const Number *, const Number *, Number *);
bool tryNumberMul(const Number *, const Number *, Number *);
bool tryNumberDiv(const Number *, const Number *, Number *);
bool tryNumberMod(const Number *, const Number *, Number *);
bool tryNumberDivMod(const Number *, const Number *, Number *, Number *);
bool tryNumberMulDigit(const Number *, Digit, Number *);
bool tryNumberDivDigit(const Number *, const Number *, Digit *result, Number *mod);

void numberPrint(const Number *number) {
    if (numberIsZero(number)) {
        printf("0");
        return;
    }

    bool zeroOngoing = true;
    printf(number->_sign == SIGN_NEGATIVE ? "- " : "+ ");
    for (int i = NUMBER_DIGITS_COUNT_MAX - 1; i >= 0; i--) {
        if (!digitEquals(number->_digits[i], digitZero())) zeroOngoing = false;
        if (!zeroOngoing) {
            printf("%d", digitToChar(number->_digits[i]));
            if (i > 0) putchar(' ');
        }
    }
}

void numberPrintWithName(const Number *number, const char *name) {
    printf("%s = ", name);
    numberPrint(number);
    putchar('\n');
}

void numberMakeZero(Number *result) {
    result->_sign = SIGN_POSITIVE;
    for (int i = 0; i < NUMBER_DIGITS_COUNT_MAX; i++) result->_digits[i] = digitZero();
}

void numberMakeRandom(int digitsCount, Number *result) {
    result->_sign = signRandom();
    for (int i = 0; i < digitsCount; i++) {
        Digit digit;
        tryCharToDigit(randomRangeInt(0, 10), &digit);
        result->_digits[i] = digit;
    }
    for (int i = digitsCount; i < NUMBER_DIGITS_COUNT_MAX; i++) result->_digits[i] = digitZero();
}

void numberSignReverse(const Number *src, Number *dst) {
    dst->_sign = signReversed(src->_sign);
    for (int i = 0; i < NUMBER_DIGITS_COUNT_MAX; i++) dst->_digits[i] = src->_digits[i];
}

void numberAbs(const Number *src, Number *dst) {
    dst->_sign = SIGN_POSITIVE;
    for (int i = 0; i < NUMBER_DIGITS_COUNT_MAX; i++) dst->_digits[i] = src->_digits[i];
}

void numberDigitShiftLeft(const Number *src, int digitsCount, Number *dst) {
    if (digitsCount < 0) return numberDigitShiftRight(src, -digitsCount, dst);
    if (digitsCount > NUMBER_DIGITS_COUNT_MAX) return numberDigitShiftLeft(src, NUMBER_DIGITS_COUNT_MAX, dst);

    dst->_sign = src->_sign;
    for (int i = 0; i < digitsCount; i++) dst->_digits[i] = digitZero();
    for (int i = digitsCount; i < NUMBER_DIGITS_COUNT_MAX; i++) dst->_digits[i] = src->_digits[i - digitsCount];
}

void numberDigitShiftRight(const Number *src, int digitsCount, Number *dst) {
    if (digitsCount < 0) return numberDigitShiftLeft(src, -digitsCount, dst);
    if (digitsCount > NUMBER_DIGITS_COUNT_MAX) return numberDigitShiftRight(src, NUMBER_DIGITS_COUNT_MAX, dst);

    dst->_sign = src->_sign;
    for (int i = NUMBER_DIGITS_COUNT_MAX - 1; i >= NUMBER_DIGITS_COUNT_MAX - digitsCount; i--) dst->_digits[i] = digitZero();
    for (int i = NUMBER_DIGITS_COUNT_MAX - 1 - digitsCount; i >= 0; i--) dst->_digits[i] = src->_digits[i + digitsCount];
}

void numberDigitInsert(const Number *src, Digit digit, Number *dst) {
    numberDigitShiftLeft(src, 1, dst);
    dst->_digits[0] = digit;
}

void intToNumber(int src, Number *dst) {
    int temp = src;
    dst->_sign = src < 0 ? SIGN_NEGATIVE : SIGN_POSITIVE;
    for (int i = 0; i < NUMBER_DIGITS_COUNT_MAX; i++) {
        tryCharToDigit(intAbs(temp % 10), &dst->_digits[i]);
        temp /= 10;
    }
}

bool tryNumberToInt(const Number *src, int *dst) {
    int sub, temp = 1;
    *dst = 0;
    for (int i = 0; i < NUMBER_DIGITS_COUNT_MAX; i++) {
        if (*dst > (sub = __INT_MAX__ - digitToChar(src->_digits[i]) * temp)) return false;
        *dst += digitToChar(src->_digits[i]) * temp;
    }
    *dst *= src->_sign == SIGN_NEGATIVE ? -1 : 1;
    return true;
}

bool numberIsZero(const Number *number) {
    for (int i = 0; i < NUMBER_DIGITS_COUNT_MAX; i++) if (!digitEquals(number->_digits[i], digitZero())) return false;
    return true;
}

bool numberEquals(const Number *a, const Number *b) {
    if (numberIsZero(a) && numberIsZero(b)) return true;
    if (a->_sign != b->_sign) return false;
    for (int i = 0; i < NUMBER_DIGITS_COUNT_MAX; i++) if (!digitEquals(a->_digits[i], b->_digits[i])) return false;
    return true;
}

bool numberIsLess(const Number *a, const Number *b) {
    if (numberIsZero(a) && numberIsZero(b)) return false;
    if (a->_sign == SIGN_NEGATIVE && b->_sign == SIGN_POSITIVE) return true;
    if (a->_sign == SIGN_POSITIVE && b->_sign == SIGN_NEGATIVE) return false;

    for (int i = NUMBER_DIGITS_COUNT_MAX - 1; i >= 0; i--) {
        if (digitIsLess(a->_digits[i], b->_digits[i])) return a->_sign == SIGN_POSITIVE;
        if (digitIsGreater(a->_digits[i], b->_digits[i])) return a->_sign == SIGN_NEGATIVE;
    }
    return false;
}

bool numberIsGreater(const Number *a, const Number *b) {
    return numberIsLess(b, a);
}

bool numberIsLessEquals(const Number *a, const Number *b) {
    return !numberIsGreater(a, b);
}

bool numberIsGreaterEquals(const Number *a, const Number *b) {
    return !numberIsLess(a, b);
}

bool tryNumberAdd(const Number *a, const Number *b, Number *result) {
    if (a->_sign == SIGN_NEGATIVE && b->_sign == SIGN_POSITIVE) {
        Number aAbs;
        numberAbs(a, &aAbs);
        return tryNumberSub(b, &aAbs, result);
    }
    if (a->_sign == SIGN_POSITIVE && b->_sign == SIGN_NEGATIVE) {
        Number bAbs;
        numberAbs(b, &bAbs);
        return tryNumberSub(a, &bAbs, result);
    }

    int carry = 0;
    result->_sign = a->_sign;
    for (int i = 0; i < NUMBER_DIGITS_COUNT_MAX; i++) if (!tryDigitAdd(a->_digits[i], b->_digits[i], &result->_digits[i], &carry)) return false;
    return !carry;
}

bool tryNumberSub(const Number *a, const Number *b, Number *result) {
    if (a->_sign == SIGN_NEGATIVE && b->_sign == SIGN_POSITIVE) {
        Number aAbs, sum;
        numberAbs(a, &aAbs);
        if (!tryNumberAdd(&aAbs, b, &sum)) return false;
        numberSignReverse(&sum, result);
        return true;
    }
    if (a->_sign == SIGN_POSITIVE && b->_sign == SIGN_NEGATIVE) {
        Number bAbs;
        numberAbs(b, &bAbs);
        return tryNumberAdd(a, &bAbs, result);
    }
    if (a->_sign == SIGN_NEGATIVE) {
        Number aAbs, bAbs;
        numberAbs(a, &aAbs);
        numberAbs(b, &bAbs);
        return tryNumberSub(&bAbs, &aAbs, result);
    }
    if (numberIsLess(a, b)) {
        Number sub;
        if (!tryNumberSub(b, a, &sub)) return false;
        numberSignReverse(&sub, result);
        return true;
    }

    int carry = 0;
    result->_sign = SIGN_POSITIVE;
    for (int i = 0; i < NUMBER_DIGITS_COUNT_MAX; i++) if (!tryDigitSub(a->_digits[i], b->_digits[i], &result->_digits[i], &carry)) return false;
    return !carry;
}

bool tryNumberMul(const Number *a, const Number *b, Number *result) {
    if (a->_sign != b->_sign) {
        Number aAbs, bAbs, pro;
        numberAbs(a, &aAbs);
        numberAbs(b, &bAbs);
        if (!tryNumberMul(&aAbs, &bAbs, &pro)) return false;
        numberSignReverse(&pro, result);
        return true;
    }
    if (a->_sign == SIGN_NEGATIVE) {
        Number aAbs, bAbs;
        numberAbs(a, &aAbs);
        numberAbs(b, &bAbs);
        return tryNumberMul(&aAbs, &bAbs, result);
    }

    Number pro, sum, shifted;
    numberMakeZero(result);
    result->_sign = SIGN_POSITIVE;
    for (int i = 0; i < NUMBER_DIGITS_COUNT_MAX; i++) {
        if (!tryNumberMulDigit(a, b->_digits[i], &pro)) return false;
        numberDigitShiftLeft(&pro, i, &shifted);
        if (!tryNumberAdd(result, &shifted, &sum)) return false;
        *result = sum;
    }
    return true;
}

bool tryNumberDiv(const Number *a, const Number *b, Number *result) {
    Number rem;
    return tryNumberDivMod(a, b, result, &rem);
}

bool tryNumberMod(const Number *a, const Number *b, Number *result) {
    Number pro;
    return tryNumberDivMod(a, b, &pro, result);
}

bool tryNumberDivMod(const Number *a, const Number *b, Number *result, Number *rem) {
    if (a->_sign != b->_sign) {
        Number aAbs, bAbs, div;
        numberAbs(a, &aAbs);
        numberAbs(b, &bAbs);
        if (!tryNumberDivMod(&aAbs, &bAbs, &div, rem)) return false;
        numberSignReverse(&div, result);
        rem->_sign = a->_sign;
        return true;
    }
    if (a->_sign == SIGN_NEGATIVE) {
        Number aAbs, bAbs;
        numberAbs(a, &aAbs);
        numberAbs(b, &bAbs);
        if (!tryNumberDivMod(&aAbs, &bAbs, result, rem)) return false;
        rem->_sign = a->_sign;
        return true;
    }

    Number inserted;
    numberMakeZero(rem);
    result->_sign = SIGN_POSITIVE;
    for (int i = NUMBER_DIGITS_COUNT_MAX - 1; i >= 0; i--) {
        numberDigitInsert(rem, a->_digits[i], &inserted);
        if (!tryNumberDivDigit(&inserted, b, &result->_digits[i], rem)) return false;
    }
    return true;
}

bool tryNumberMulDigit(const Number *number, Digit digit, Number *result) {
    int carry = 0;
    result->_sign = number->_sign;
    for (int i = 0; i < NUMBER_DIGITS_COUNT_MAX; i++) if (!tryDigitMul(number->_digits[i], digit, &result->_digits[i], &carry)) return false;
    return !carry;
}

bool tryNumberDivDigit(const Number *a, const Number *b, Digit *result, Number *rem) {
    Number pro;
    Digit digit;
    if (a->_sign != b->_sign) return false;
    rem->_sign = a->_sign;
    for (int i = 1; i < 10; i++) {
        if (!tryCharToDigit(i, &digit)) return false;
        if (!tryNumberMulDigit(b, digit, &pro)) return false;
        if (numberIsLess(a, &pro)) {
            if (!tryCharToDigit(i - 1, result)) return false;
            if (!tryNumberMulDigit(b, *result, &pro)) return false;
            return tryNumberSub(a, &pro, rem);
        }
    }
    if (!tryCharToDigit(9, result)) return false;
    if (!tryNumberMulDigit(b, *result, &pro)) return false;
    return tryNumberSub(a, &pro, rem);
}

#endif
