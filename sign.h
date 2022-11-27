#ifndef _H_SIGN_
#define _H_SIGN_

#include <stdbool.h>
#include <string.h>
#include "common.h"

typedef struct _Sign {
    char *_value;
} Sign;

void signPrint(Sign);
Sign signNegative();
Sign signPositive();
Sign signRandom();
Sign signReverse(Sign);
bool signEquals(Sign, Sign);
static Sign newSign(char *);

void signPrint(Sign this) {
    printf(this._value);
}

Sign signNegative() {
    return newSign("- ");
}

Sign signPositive() {
    return newSign("+ ");
}

Sign signRandom() {
    return probabilityIsHit(0.5f) ? signNegative() : signPositive();
}

Sign signReverse(Sign this) {
    return signEquals(this, signNegative()) ? signPositive() : signNegative();
}

bool signEquals(Sign this, Sign other) {
    return !strcmp(this._value, other._value);
}

static Sign newSign(char *value) {
    Sign result = { ._value = value };
    return result;
}

#endif