#ifndef _H_MULTI_INT_
#define _H_MULTI_INT_

#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include "sign.h"
#include "digit.h"
#include "common.h"

#define MULTI_INT_DIGITS_SIZE 1024

typedef struct _MultiInt {
    Sign _sign;
    Digit _digits[MULTI_INT_DIGITS_SIZE];
} MultiInt;

void multiIntPrint(MultiInt);
void multiIntPrintWithName(const char *, MultiInt);
bool multiIntIsZero(MultiInt);
bool multiIntIsOne(MultiInt);
bool multiIntIsPrime(MultiInt);
int multiIntCompareTo(MultiInt, MultiInt);
int multiIntToInt(MultiInt);
MultiInt multiIntZero();
MultiInt multiIntOne();
MultiInt intToMultiInt(int);
MultiInt multiIntRandom(int);
MultiInt multiIntAbs(MultiInt);
MultiInt multiIntSignReverse(MultiInt);
MultiInt multiIntSqrt(MultiInt);
MultiInt multiIntDigitShiftLeft(MultiInt, int);
MultiInt multiIntDigitShiftRight(MultiInt, int);
MultiInt multiIntAdd(MultiInt, MultiInt);
MultiInt multiIntSub(MultiInt, MultiInt);
MultiInt multiIntMul(MultiInt, MultiInt);
MultiInt multiIntDiv(MultiInt, MultiInt);
MultiInt multiIntMod(MultiInt, MultiInt);
MultiInt multiIntPow(MultiInt, MultiInt);
static Digit multiIntDivDigit(MultiInt, MultiInt);
static MultiInt newMultiInt(Sign, const Digit *);
static MultiInt multiIntInsert(MultiInt, Digit);
static MultiInt multiIntMulDigit(MultiInt, Digit);
static MultiInt multiIntModDigit(MultiInt, MultiInt);

void multiIntPrint(MultiInt this) {
    if (multiIntIsZero(this)) {
        printf("0");
        return;
    }
    signPrint(this._sign);
    bool zeroOngoing = true;
    for (int i = MULTI_INT_DIGITS_SIZE - 1; i >= 0; i--) {
        if (zeroOngoing && !digitIsZero(this._digits[i])) zeroOngoing = false;
        if (zeroOngoing) continue;
        digitPrint(this._digits[i]);
        if (i > 0) putchar(' ');
    }
}

void multiIntPrintWithName(const char *name, MultiInt this) {
    printf("%s = ", name);
    multiIntPrint(this);
    putchar('\n');
}

bool multiIntIsZero(MultiInt this) {
    for (int i = 0; i < MULTI_INT_DIGITS_SIZE; i++) if (!digitIsZero(this._digits[i])) return false;
    return true;
}

bool multiIntIsOne(MultiInt this) {
    if (signEquals(this._sign, signNegative()) || !digitIsOne(this._digits[0])) return false;
    for (int i = 1; i < MULTI_INT_DIGITS_SIZE; i++) if (!digitIsZero(this._digits[i])) return false;
    return true;
}

bool multiIntIsPrime(MultiInt this) {
    if (multiIntCompareTo(this, multiIntOne()) <= 0) return false;
    MultiInt two = intToMultiInt(2);
    if (!multiIntCompareTo(this, two)) return true;
    if (multiIntIsZero(multiIntMod(this, intToMultiInt(2)))) return false;
    MultiInt sqrt = multiIntSqrt(this);
    for (MultiInt i = intToMultiInt(3); multiIntCompareTo(i, sqrt) <= 0; i = multiIntAdd(i, two)) if (multiIntIsZero(multiIntMod(this, i))) return false;
    return true;
}

int multiIntCompareTo(MultiInt this, MultiInt other) {
    if (multiIntIsZero(this) && multiIntIsZero(other)) return 0;
    if (signEquals(this._sign, signNegative()) && signEquals(other._sign, signPositive())) return -1;
    if (signEquals(this._sign, signPositive()) && signEquals(other._sign, signNegative())) return 1;
    bool areNegative = signEquals(this._sign, signNegative());
    for (int i = MULTI_INT_DIGITS_SIZE - 1; i >= 0; i--) {
        if (digitCompareTo(this._digits[i], other._digits[i]) < 0) return areNegative ? 1 : -1;
        if (digitCompareTo(this._digits[i], other._digits[i]) > 0) return areNegative ? -1 : 1;
    }
    return 0;
}

int multiIntToInt(MultiInt this) {
    if (multiIntCompareTo(this, intToMultiInt(INT_MAX)) > 0 || multiIntCompareTo(this, intToMultiInt(INT_MIN)) < 0) throwException("MultiIntをintにキャストできません。");
    int result = 0;
    for (int i = 0; i < MULTI_INT_DIGITS_SIZE; i++) result += digitWithWeight(this._digits[i], i);
    return signEquals(this._sign, signNegative()) ? -result : result;
}

MultiInt multiIntZero() {
    Digit digits[MULTI_INT_DIGITS_SIZE];
    for (int i = 0; i < MULTI_INT_DIGITS_SIZE; i++) digits[i] = digitZero();
    return newMultiInt(signPositive(), digits);
}

MultiInt multiIntOne() {
    Digit digits[MULTI_INT_DIGITS_SIZE];
    digits[0] = digitOne();
    for (int i = 1; i < MULTI_INT_DIGITS_SIZE; i++) digits[i] = digitZero();
    return newMultiInt(signPositive(), digits);
}

MultiInt intToMultiInt(int value) {
    Digit digits[MULTI_INT_DIGITS_SIZE];
    int temp = value;
    for (int i = 0; i < MULTI_INT_DIGITS_SIZE; i++) {
        digits[i] = newDigit(intAbs(temp % 10));
        temp /= 10;
    }
    return newMultiInt(value < 0 ? signNegative() : signPositive(), digits);
}

MultiInt multiIntRandom(int digitsCount) {
    if (digitsCount < 0 || digitsCount > MULTI_INT_DIGITS_SIZE) throwException("multiIntRandomの引数が%dです。", digitsCount);
    Digit digits[MULTI_INT_DIGITS_SIZE];
    for (int i = 0; i < digitsCount; i++) digits[i] = digitRandom();
    for (int i = digitsCount; i < MULTI_INT_DIGITS_SIZE; i++) digits[i] = digitZero();
    return newMultiInt(signRandom(), digits);
}

MultiInt multiIntAbs(MultiInt this) {
    return newMultiInt(signPositive(), this._digits);
}

MultiInt multiIntSignReverse(MultiInt this) {
    return newMultiInt(signReverse(this._sign), this._digits);
}

MultiInt multiIntSqrt(MultiInt this) {
    const int N = 10;
    const MultiInt TWO = intToMultiInt(2);
    MultiInt x = multiIntOne();
    for (int i = 0; i < N; i++) {
        MultiInt y = multiIntSub(multiIntMul(x, x), this);
        MultiInt yd = multiIntMul(TWO, x);
        x = multiIntSub(x, multiIntDiv(y, yd));
    }
    return multiIntCompareTo(multiIntMul(x, x), this) > 0 ? multiIntSub(x, multiIntOne()) : x;
}

MultiInt multiIntDigitShiftLeft(MultiInt this, int count) {
    if (count < 0 || count > MULTI_INT_DIGITS_SIZE) throwException("multiIntDigitShiftLeftの引数が%dです。", count);
    Digit digits[MULTI_INT_DIGITS_SIZE];
    for (int i = 0; i < count; i++) digits[i] = digitZero();
    for (int i = count; i < MULTI_INT_DIGITS_SIZE; i++) digits[i] = this._digits[i - count];
    return newMultiInt(this._sign, digits);
}

MultiInt multiIntDigitShiftRight(MultiInt this, int count) {
    if (count < 0 || count > MULTI_INT_DIGITS_SIZE) throwException("multiIntDigitShiftRightの引数が%dです。", count);
    Digit digits[MULTI_INT_DIGITS_SIZE];
    for (int i = 0; i < MULTI_INT_DIGITS_SIZE - count; i++) digits[i] = this._digits[i + count];
    for (int i = MULTI_INT_DIGITS_SIZE - count; i < MULTI_INT_DIGITS_SIZE; i++) digits[i] = digitZero();
    return newMultiInt(this._sign, digits);
}

MultiInt multiIntAdd(MultiInt this, MultiInt other) {
    if (signEquals(this._sign, signNegative()) && signEquals(other._sign, signPositive())) return multiIntSub(other, multiIntAbs(this));
    if (signEquals(this._sign, signPositive()) && signEquals(other._sign, signNegative())) return multiIntSub(this, multiIntAbs(other));
    Digit digits[MULTI_INT_DIGITS_SIZE];
    int carry = 0;
    for (int i = 0; i < MULTI_INT_DIGITS_SIZE; i++) digits[i] = digitAdd(this._digits[i], other._digits[i], &carry);
    return newMultiInt(this._sign, digits);
}

MultiInt multiIntSub(MultiInt this, MultiInt other) {
    if (signEquals(this._sign, signNegative()) && signEquals(other._sign, signPositive())) return multiIntSignReverse(multiIntAdd(multiIntAbs(this), other));
    if (signEquals(this._sign, signPositive()) && signEquals(other._sign, signNegative())) return multiIntAdd(this, multiIntAbs(other));
    if (signEquals(this._sign, signNegative())) return multiIntSub(multiIntAbs(other), multiIntAbs(this));
    if (multiIntCompareTo(this, other) < 0) return multiIntSignReverse(multiIntSub(other, this));
    Digit digits[MULTI_INT_DIGITS_SIZE];
    int carry = 0;
    for (int i = 0; i < MULTI_INT_DIGITS_SIZE; i++) digits[i] = digitSub(this._digits[i], other._digits[i], &carry);
    return newMultiInt(signPositive(), digits);
}

MultiInt multiIntMul(MultiInt this, MultiInt other) {
    if (!signEquals(this._sign, other._sign)) return multiIntSignReverse(multiIntMul(multiIntAbs(this), multiIntAbs(other)));
    if (signEquals(this._sign, signNegative())) return multiIntMul(multiIntAbs(this), multiIntAbs(other));
    MultiInt result = multiIntZero();
    for (int i = 0; i < MULTI_INT_DIGITS_SIZE; i++) {
        if (digitIsZero(other._digits[i])) continue;
        result = multiIntAdd(result, multiIntDigitShiftLeft(multiIntMulDigit(this, other._digits[i]), i));
    }
    return result;
}

MultiInt multiIntDiv(MultiInt this, MultiInt other) {
    if (multiIntIsZero(other)) throwException("multiIntDivでゼロ除算が発生しました。");
    if (!signEquals(this._sign, other._sign)) return multiIntSignReverse(multiIntDiv(multiIntAbs(this), multiIntAbs(other)));
    if (signEquals(this._sign, signNegative())) return multiIntDiv(multiIntAbs(this), multiIntAbs(other));
    MultiInt rem = multiIntZero();
    Digit digits[MULTI_INT_DIGITS_SIZE];
    bool zeroOngoing = true;
    for (int i = MULTI_INT_DIGITS_SIZE - 1; i >= 0; i--) {
        if (zeroOngoing && !digitIsZero(this._digits[i])) zeroOngoing = false;
        if (zeroOngoing) {
            digits[i] = digitZero();
            continue;
        }
        rem = multiIntInsert(rem, this._digits[i]);
        digits[i] = multiIntDivDigit(rem, other);
        rem = multiIntModDigit(rem, other);
    }
    return newMultiInt(signPositive(), digits);
}

MultiInt multiIntMod(MultiInt this, MultiInt other) {
    if (multiIntIsZero(other)) throwException("multiIntModでゼロ除算が発生しました。");
    if (signEquals(this._sign, signNegative())) return multiIntSignReverse(multiIntMod(multiIntAbs(this), multiIntAbs(other)));
    if (signEquals(other._sign, signNegative())) return multiIntMod(this, multiIntAbs(other));
    MultiInt result = multiIntZero();
    bool zeroOngoing = true;
    for (int i = MULTI_INT_DIGITS_SIZE - 1; i >= 0; i--) {
        if (zeroOngoing && !digitIsZero(this._digits[i])) zeroOngoing = false;
        if (zeroOngoing) continue;
        result = multiIntModDigit(multiIntInsert(result, this._digits[i]), other);
    }
    return result;
}

MultiInt multiIntPow(MultiInt this, MultiInt other) {
    int compareResult = multiIntCompareTo(other, multiIntZero());
    if (compareResult < 0) throwException("multiIntPowの指数が負です。");
    if (!compareResult) return multiIntOne();
    return multiIntMul(this, multiIntPow(this, multiIntSub(other, multiIntOne())));
}

MultiInt multiIntFact(MultiInt this) {
    if (multiIntCompareTo(this, multiIntZero()) < 0) throwException("multiIntFactの引数が負です。");
    if (multiIntIsZero(this)) return multiIntOne();
    return multiIntMul(this, multiIntFact(multiIntSub(this, multiIntOne())));
}

MultiInt multiIntGCD(MultiInt this, MultiInt other) {
    if (multiIntIsZero(other)) return multiIntAbs(this);
    return multiIntGCD(other, multiIntMod(this, other));
}

MultiInt multiIntLCM(MultiInt this, MultiInt other) {
    MultiInt gcd = multiIntGCD(this, other);
    if (multiIntIsZero(gcd)) return multiIntZero();
    return multiIntAbs(multiIntDiv(multiIntMul(this, other), gcd));
}

static Digit multiIntDivDigit(MultiInt this, MultiInt other) {
    if (multiIntIsZero(other)) throwException("multiIntDivDigitでゼロ除算が発生しました。");
    if (!signEquals(this._sign, other._sign)) throwException("multiIntDivDigitの結果が負になります。");
    if (signEquals(this._sign, signNegative())) return multiIntDivDigit(multiIntAbs(this), multiIntAbs(other));
    MultiInt rem = this;
    for (int i = 0; i < DIGIT_CARDINAL_NUMBER; i++) {
        if (multiIntCompareTo(rem, other) < 0) return newDigit(i);
        rem = multiIntSub(rem, other);
    }
    throwException("multiIntDivDigitの結果が一桁に収まりません。");
    return digitZero();
}

static MultiInt newMultiInt(Sign sign, const Digit *digits) {
    MultiInt result;
    result._sign = sign;
    for (int i = 0; i < MULTI_INT_DIGITS_SIZE; i++) result._digits[i] = digits[i];
    return result;
}

static MultiInt multiIntInsert(MultiInt this, Digit digit) {
    MultiInt result = multiIntDigitShiftLeft(this, 1);
    result._digits[0] = digit;
    return result;
}

static MultiInt multiIntMulDigit(MultiInt this, Digit other) {
    Digit digits[MULTI_INT_DIGITS_SIZE];
    int carry = 0;
    for (int i = 0; i < MULTI_INT_DIGITS_SIZE; i++) digits[i] = digitMul(this._digits[i], other, &carry);
    return newMultiInt(this._sign, digits);
}

static MultiInt multiIntModDigit(MultiInt this, MultiInt other) {
    if (multiIntIsZero(other)) throwException("multiIntModDigitでゼロ除算が発生しました。");
    if (signEquals(this._sign, signNegative())) return multiIntSignReverse(multiIntModDigit(multiIntAbs(this), multiIntAbs(other)));
    if (signEquals(other._sign, signNegative())) return multiIntModDigit(this, multiIntAbs(other));
    MultiInt result = this;
    for (int i = 0; i < DIGIT_CARDINAL_NUMBER; i++) {
        if (multiIntCompareTo(result, other) < 0) return result;
        result = multiIntSub(result, other);
    }
    throwException("multiIntModDigitの商が一桁に収まりません。");
    return multiIntZero();
}

#endif