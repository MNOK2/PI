#ifndef _H_SIGN_
#define _H_SIGN_

#include <stdio.h>
#include "common.h"

typedef struct _Sign {
    char *_value;
} Sign;

void signPrint(Sign);
int signIsNegative(Sign);
int signIsPositive(Sign);
int signEquals(Sign, Sign);
Sign signNegative();
Sign signPositive();
Sign signRandom();
Sign signReversed(Sign);

void signPrint(Sign this) {
    printf("%s", this._value);
}

int signIsNegative(Sign this) {
    return signEquals(this, signNegative());
}

int signIsPositive(Sign this) {
    return signEquals(this, signPositive());
}

int signEquals(Sign this, Sign other) {
    return this._value == other._value;
}

Sign signNegative() {
    Sign result = { ._value = "- " };
    return result;
}

Sign signPositive() {
    Sign result = { ._value = "+ " };
    return result;
}

Sign signRandom() {
    return probabilityIsHit(0.5f) ? signNegative() : signPositive();
}

Sign signReversed(Sign this) {
    return signIsNegative(this) ? signPositive() : signNegative();
}

#endif
