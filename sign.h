#ifndef _H_SIGN_
#define _H_SIGN_

#include "string.h"

typedef struct _Sign {
    String _value;
} Sign;

Sign signPositive();
Sign signNegative();
Sign signReversed(Sign sign);
char signEquals(Sign a, Sign b);
String signToString(Sign sign);

Sign signPositive() {
    Sign result = { ._value = stringPrintf("+ ") };
    return result;
}

Sign signNegative() {
    Sign result = { ._value = stringPrintf("- ") };
    return result;
}

Sign signReversed(Sign sign) {
    return signEquals(sign, signNegative()) ? signPositive() : signNegative();
}

char signEquals(Sign a, Sign b) {
    return stringEquals(a._value, b._value);
}

String signToString(Sign sign) {
    return sign._value;
}

#endif
