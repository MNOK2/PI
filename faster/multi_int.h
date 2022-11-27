#ifndef _H_MULTI_INT_
#define _H_MULTI_INT_

#include <stdio.h>
#include "sign.h"
#include "digit.h"
#include "common.h"

#define MULTI_INT_DIGITS_COUNT_MAX (1 << 10)

typedef struct _MultiInt {
    Sign _sign;
    Digit _digits[MULTI_INT_DIGITS_COUNT_MAX];
} MultiInt;

typedef enum _MultiIntProcessState {
    MULTI_INT_PROCESS_STATE_NO_PROBLEM = 0,
    MULTI_INT_PROCESS_STATE_ILLEGAL_ARGUMENT,
    MULTI_INT_PROCESS_STATE_OVERFLOW,
    MULTI_INT_PROCESS_STATE_DIV_BY_ZERO
} MultiIntProcessState;

void multiIntPrint(const MultiInt *);
void multiIntPrintWithName(const MultiInt *, const char *);
void multiIntMakeZero(MultiInt *);
void multiIntMakeRandom(MultiInt *, int);
void multiIntAbs(MultiInt *, const MultiInt *);
void multiIntSignReverse(MultiInt *, const MultiInt *);
int multiIntIsZero(const MultiInt *);
int multiIntCompareTo(const MultiInt *, const MultiInt *);
MultiIntProcessState intToMultiInt(MultiInt *, int);
MultiIntProcessState multiIntToInt(int *, const MultiInt *);
MultiIntProcessState multiIntDigitShift(MultiInt *, const MultiInt *, int);
MultiIntProcessState multiIntInsertDigit(MultiInt *, const MultiInt *, Digit);
MultiIntProcessState multiIntAdd(MultiInt *, const MultiInt *, const MultiInt *);
MultiIntProcessState multiIntSub(MultiInt *, const MultiInt *, const MultiInt *);
MultiIntProcessState multiIntMul(MultiInt *, const MultiInt *, const MultiInt *);
MultiIntProcessState multiIntDiv(MultiInt *, const MultiInt *, const MultiInt *);
MultiIntProcessState multiIntMod(MultiInt *, const MultiInt *, const MultiInt *);
MultiIntProcessState multiIntMulDigit(MultiInt *, const MultiInt *, Digit);
MultiIntProcessState multiIntDivDigit(Digit *, const MultiInt *, const MultiInt *);
MultiIntProcessState multiIntModDigit(MultiInt *, const MultiInt *, const MultiInt *);

void multiIntPrint(const MultiInt *this) {
    if (multiIntIsZero(this)) {
        printf("0");
        return;
    }
    signPrint(this->_sign);
    int zeroOngoing = 1;
    for (int i = MULTI_INT_DIGITS_COUNT_MAX - 1; i >= 0; i--) {
        if (digitCompareTo(this->_digits[i], digitZero())) zeroOngoing = 0;
        if (!zeroOngoing) {
            digitPrint(this->_digits[i]);
            if (i > 0) putchar(' ');
        }
    }
}

void multiIntPrintWithName(const MultiInt *this, const char *name) {
    printf("%s = ", name);
    multiIntPrint(this);
    putchar('\n');
}

void multiIntMakeZero(MultiInt *this) {
    this->_sign = signPositive();
    for (int i = 0; i < MULTI_INT_DIGITS_COUNT_MAX; i++) this->_digits[i] = digitZero();
}

void multiIntMakeRandom(MultiInt *this, int digitsCount) {
    this->_sign = signRandom();
    for (int i = 0; i < digitsCount; i++) this->_digits[i] = digitRandom();
    for (int i = digitsCount; i < MULTI_INT_DIGITS_COUNT_MAX; i++) this->_digits[i] = digitZero();
}

void multiIntAbs(MultiInt *dst, const MultiInt *src) {
    *dst = *src;
    dst->_sign = signPositive();
}

void multiIntSignReverse(MultiInt *dst, const MultiInt *src) {
    *dst = *src;
    dst->_sign = signReversed(dst->_sign);
}

int multiIntIsZero(const MultiInt *this) {
    for (int i = 0; i < MULTI_INT_DIGITS_COUNT_MAX; i++) if (digitCompareTo(this->_digits[i], digitZero())) return 0;
    return 1;
}

int multiIntCompareTo(const MultiInt *this, const MultiInt *other) {
    if (multiIntIsZero(this) && multiIntIsZero(other)) return 0;
    if (signIsNegative(this->_sign) && signIsPositive(other->_sign)) return -1;
    if (signIsPositive(this->_sign) && signIsNegative(other->_sign)) return 1;
    int areNegative = signIsNegative(this->_sign);
    for (int i = MULTI_INT_DIGITS_COUNT_MAX - 1; i >= 0; i--) {
        if (digitCompareTo(this->_digits[i], other->_digits[i]) < 0) return areNegative ? 1 : -1;
        if (digitCompareTo(this->_digits[i], other->_digits[i]) > 0) return areNegative ? -1 : 1;
    }
    return 0;
}

MultiIntProcessState intToMultiInt(MultiInt *dst, int src) {
    multiIntMakeZero(dst);
    dst->_sign = src < 0 ? signNegative() : signPositive();
    int temp = src;
    for (int i = 0; i < MULTI_INT_DIGITS_COUNT_MAX; i++) {
        if (!tryIntToDigit(&dst->_digits[i], intAbs(temp % (DIGIT_MAX + 1)))) return MULTI_INT_PROCESS_STATE_ILLEGAL_ARGUMENT;
        temp /= 10;
    }
    return temp ? MULTI_INT_PROCESS_STATE_OVERFLOW : MULTI_INT_PROCESS_STATE_NO_PROBLEM;
}

MultiIntProcessState multiIntToInt(int *dst, const MultiInt *src) {
    *dst = 0;
    int weight = 1;
    for (int i = 0; i < MULTI_INT_DIGITS_COUNT_MAX; i++) {
        int delta = digitToInt(src->_digits[i]) * weight;
        if (*dst > __INT_MAX__ - delta) return MULTI_INT_PROCESS_STATE_OVERFLOW;
        *dst += delta;
    }
    if (signIsNegative(src->_sign)) *dst *= -1;
    return MULTI_INT_PROCESS_STATE_NO_PROBLEM;
}

MultiIntProcessState multiIntDigitShift(MultiInt *dst, const MultiInt *src, int count) {
    if (intAbs(count) > MULTI_INT_DIGITS_COUNT_MAX) return MULTI_INT_PROCESS_STATE_ILLEGAL_ARGUMENT;

    MultiInt result;
    result._sign = src->_sign;
    if (count < 0) {
        for (int i = 0; i < -count; i++) result._digits[i] = digitZero();
        for (int i = -count; i < MULTI_INT_DIGITS_COUNT_MAX; i++) result._digits[i] = src->_digits[i + count];
        for (int i = MULTI_INT_DIGITS_COUNT_MAX - 1 + count; i < MULTI_INT_DIGITS_COUNT_MAX; i++) if (digitCompareTo(src->_digits[i], digitZero())) return MULTI_INT_PROCESS_STATE_OVERFLOW;
    }
    else {
        for (int i = 0; i < MULTI_INT_DIGITS_COUNT_MAX - count; i++) result._digits[i] = src->_digits[i + count];
        for (int i = MULTI_INT_DIGITS_COUNT_MAX - 1 - count; i < MULTI_INT_DIGITS_COUNT_MAX; i++) result._digits[i] = digitZero();
        for (int i = 0; i < count; i++) if (digitCompareTo(src->_digits[i], digitZero())) return MULTI_INT_PROCESS_STATE_OVERFLOW;
    }
    *dst = result;
    return MULTI_INT_PROCESS_STATE_NO_PROBLEM;
}

MultiIntProcessState multiIntInsertDigit(MultiInt *dst, const MultiInt *src, Digit digit) {
    MultiIntProcessState processState;
    if ((processState = multiIntDigitShift(dst, dst, -1)) != MULTI_INT_PROCESS_STATE_NO_PROBLEM) return processState;
    dst->_digits[0] = digit;
    return MULTI_INT_PROCESS_STATE_NO_PROBLEM;
}

MultiIntProcessState multiIntAdd(MultiInt *dst, const MultiInt *this, const MultiInt *other) {
    if (signIsNegative(this->_sign) && signIsPositive(other->_sign)) {
        MultiInt thisAbs;
        multiIntAbs(&thisAbs, this);
        return multiIntSub(dst, other, &thisAbs);
    }
    if (signIsPositive(this->_sign) && signIsNegative(other->_sign)) {
        MultiInt otherAbs;
        multiIntAbs(&otherAbs, other);
        return multiIntSub(dst, this, &otherAbs);
    }
    MultiInt result;
    result._sign = this->_sign;
    int carry = 0;
    for (int i = 0; i < MULTI_INT_DIGITS_COUNT_MAX; i++) result._digits[i] = digitAdd(this->_digits[i], other->_digits[i], &carry);
    *dst = result;
    return carry ? MULTI_INT_PROCESS_STATE_OVERFLOW : MULTI_INT_PROCESS_STATE_NO_PROBLEM;
}

MultiIntProcessState multiIntSub(MultiInt *dst, const MultiInt *this, const MultiInt *other) {
    if (signIsNegative(this->_sign) && signIsPositive(other->_sign)) {
        MultiInt thisAbs;
        MultiIntProcessState processState;
        multiIntAbs(&thisAbs, this);
        if ((processState = multiIntAdd(dst, &thisAbs, other)) != MULTI_INT_PROCESS_STATE_NO_PROBLEM) return processState;
        multiIntSignReverse(dst, dst);
        return MULTI_INT_PROCESS_STATE_NO_PROBLEM;
    }
    if (signIsPositive(this->_sign) && signIsNegative(other->_sign)) {
        MultiInt otherAbs;
        multiIntAbs(&otherAbs, other);
        return multiIntAdd(dst, this, &otherAbs);
    }
    if (signIsNegative(this->_sign)) {
        MultiInt thisAbs, otherAbs;
        multiIntAbs(&thisAbs, this);
        multiIntAbs(&otherAbs, other);
        return multiIntSub(dst, &otherAbs, &thisAbs);
    }
    if (multiIntCompareTo(this, other) < 0) {
        MultiIntProcessState processState;
        if ((processState = multiIntSub(dst, other, this)) != MULTI_INT_PROCESS_STATE_NO_PROBLEM) return processState;
        multiIntSignReverse(dst, dst);
        return MULTI_INT_PROCESS_STATE_NO_PROBLEM;
    }
    MultiInt result;
    result._sign = signPositive();
    int carry = 0;
    for (int i = 0; i < MULTI_INT_DIGITS_COUNT_MAX; i++) result._digits[i] = digitSub(this->_digits[i], other->_digits[i], &carry);
    *dst = result;
    return carry ? MULTI_INT_PROCESS_STATE_OVERFLOW : MULTI_INT_PROCESS_STATE_NO_PROBLEM;
}

MultiIntProcessState multiIntMul(MultiInt *dst, const MultiInt *this, const MultiInt *other) {
    if (!signEquals(this->_sign, other->_sign)) {
        MultiInt thisAbs, otherAbs;
        MultiIntProcessState processState;
        multiIntAbs(&thisAbs, this);
        multiIntAbs(&otherAbs, other);
        if ((processState = multiIntMul(dst, &thisAbs, &otherAbs)) != MULTI_INT_PROCESS_STATE_NO_PROBLEM) return processState;
        multiIntSignReverse(dst, dst);
        return MULTI_INT_PROCESS_STATE_NO_PROBLEM;
    }
    if (signIsNegative(this->_sign)) {
        MultiInt thisAbs, otherAbs;
        multiIntAbs(&thisAbs, this);
        multiIntAbs(&otherAbs, other);
        return multiIntMul(dst, &thisAbs, &otherAbs);
    }

    MultiInt result;
    multiIntMakeZero(&result);
    for (int i = 0; i < MULTI_INT_DIGITS_COUNT_MAX; i++) {
        MultiInt pro;
        MultiIntProcessState processState;
        if ((processState = multiIntMulDigit(&pro, this, other->_digits[i])) != MULTI_INT_PROCESS_STATE_NO_PROBLEM) return processState;
        if ((processState = multiIntDigitShift(&pro, &pro, -i)) != MULTI_INT_PROCESS_STATE_NO_PROBLEM) return processState;
        if ((processState = multiIntAdd(&result, &result, &pro)) != MULTI_INT_PROCESS_STATE_NO_PROBLEM) return processState;
    }
    *dst = result;
    return MULTI_INT_PROCESS_STATE_NO_PROBLEM;
}

MultiIntProcessState multiIntDiv(MultiInt *dst, const MultiInt *this, const MultiInt *other) {
    if (multiIntIsZero(other)) return MULTI_INT_PROCESS_STATE_DIV_BY_ZERO;
    if (!signEquals(this->_sign, other->_sign)) {
        MultiInt thisAbs, otherAbs;
        MultiIntProcessState processState;
        multiIntAbs(&thisAbs, this);
        multiIntAbs(&otherAbs, other);
        if ((processState = multiIntDiv(dst, &thisAbs, &otherAbs)) != MULTI_INT_PROCESS_STATE_NO_PROBLEM) return processState;
        multiIntSignReverse(dst, dst);
        return MULTI_INT_PROCESS_STATE_NO_PROBLEM;
    }
    if (signIsNegative(this->_sign)) {
        MultiInt thisAbs, otherAbs;
        multiIntAbs(&thisAbs, this);
        multiIntAbs(&otherAbs, other);
        return multiIntDiv(dst, &thisAbs, &otherAbs);
    }

    MultiInt result, rem;
    multiIntMakeZero(&rem);
    result._sign = signPositive();
    for (int i = MULTI_INT_DIGITS_COUNT_MAX - 1; i >= 0; i--) {
        MultiIntProcessState processState;
        if ((processState = multiIntInsertDigit(&rem, &rem, this->_digits[i])) != MULTI_INT_PROCESS_STATE_NO_PROBLEM) return processState;
        if ((processState = multiIntDivDigit(&result._digits[i], &rem, other)) != MULTI_INT_PROCESS_STATE_NO_PROBLEM) return processState;
        if ((processState = multiIntModDigit(&rem, &rem, other)) != MULTI_INT_PROCESS_STATE_NO_PROBLEM) return processState;
    }
    *dst = result;
    return MULTI_INT_PROCESS_STATE_NO_PROBLEM;
}

MultiIntProcessState multiIntMod(MultiInt *dst, const MultiInt *this, const MultiInt *other) {
    if (signIsNegative(this->_sign)) {
        MultiInt thisAbs, otherAbs;
        MultiIntProcessState processState;
        multiIntAbs(&thisAbs, this);
        multiIntAbs(&otherAbs, other);
        if ((processState = multiIntMod(dst, &thisAbs, &otherAbs)) != MULTI_INT_PROCESS_STATE_NO_PROBLEM) return processState;
        multiIntSignReverse(dst, dst);
        return MULTI_INT_PROCESS_STATE_NO_PROBLEM;
    }
    if (signIsNegative(other->_sign)) {
        MultiInt otherAbs;
        multiIntAbs(&otherAbs, other);
        return multiIntMod(dst, this, &otherAbs);
    }

    MultiInt result;
    multiIntMakeZero(&result);
    for (int i = MULTI_INT_DIGITS_COUNT_MAX - 1; i >= 0; i--) {
        MultiIntProcessState processState;
        if ((processState = multiIntInsertDigit(&result, &result, this->_digits[i])) != MULTI_INT_PROCESS_STATE_NO_PROBLEM) return processState;
        if ((processState = multiIntModDigit(&result, &result, other)) != MULTI_INT_PROCESS_STATE_NO_PROBLEM) return processState;
    }
    *dst = result;
    return MULTI_INT_PROCESS_STATE_NO_PROBLEM;
}

MultiIntProcessState multiIntMulDigit(MultiInt *dst, const MultiInt *this, Digit other) {
    MultiInt result;
    result._sign = this->_sign;
    int carry = 0;
    for (int i = 0; i < MULTI_INT_DIGITS_COUNT_MAX; i++) result._digits[i] = digitMul(this->_digits[i], other, &carry);
    *dst = result;
    return carry ? MULTI_INT_PROCESS_STATE_OVERFLOW : MULTI_INT_PROCESS_STATE_NO_PROBLEM;
}

MultiIntProcessState multiIntDivDigit(Digit *dst, const MultiInt *this, const MultiInt *other)  {
    if (!signEquals(this->_sign, other->_sign)) return MULTI_INT_PROCESS_STATE_ILLEGAL_ARGUMENT;
    if (signIsNegative(this->_sign)) {
        MultiInt thisAbs, otherAbs;
        multiIntAbs(&thisAbs, this);
        multiIntAbs(&otherAbs, other);
        return multiIntDivDigit(dst, &thisAbs, &otherAbs);
    }
    MultiInt rem = *this;
    for (int i = 0; i < DIGIT_MAX + 1; i++) {
        MultiIntProcessState processState;
        if (multiIntCompareTo(&rem, other) < 0) {
            if (!tryIntToDigit(dst, i)) return MULTI_INT_PROCESS_STATE_ILLEGAL_ARGUMENT;
            return MULTI_INT_PROCESS_STATE_NO_PROBLEM;
        }
        if ((processState = multiIntSub(&rem, &rem, other)) != MULTI_INT_PROCESS_STATE_NO_PROBLEM) return processState;
    }
    return MULTI_INT_PROCESS_STATE_ILLEGAL_ARGUMENT;
}

MultiIntProcessState multiIntModDigit(MultiInt *dst, const MultiInt *this, const MultiInt *other)  {
    if (signIsNegative(this->_sign)) {
        MultiInt thisAbs, otherAbs;
        MultiIntProcessState processState;
        multiIntAbs(&thisAbs, this);
        multiIntAbs(&otherAbs, other);
        if ((processState = multiIntModDigit(dst, &thisAbs, &otherAbs)) != MULTI_INT_PROCESS_STATE_NO_PROBLEM) return processState;
        multiIntSignReverse(dst, dst);
        return MULTI_INT_PROCESS_STATE_NO_PROBLEM;
    }
    if (signIsNegative(other->_sign)) {
        MultiInt otherAbs;
        multiIntAbs(&otherAbs, other);
        return multiIntModDigit(dst, this, &otherAbs);
    }
    *dst = *this;
    for (int i = 0; i < DIGIT_MAX + 1; i++) {
        MultiIntProcessState processState;
        if (multiIntCompareTo(dst, other) < 0) return MULTI_INT_PROCESS_STATE_NO_PROBLEM;
        if ((processState = multiIntSub(dst, dst, other)) != MULTI_INT_PROCESS_STATE_NO_PROBLEM) return processState;
    }
    return MULTI_INT_PROCESS_STATE_ILLEGAL_ARGUMENT;
}

#endif
