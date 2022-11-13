#ifndef _H_SIGN_
#define _H_SIGN_

#include <string.h>

typedef struct _Sign {
    char *_value;
} Sign;

Sign signPositive();
Sign signNegative();
Sign signReversed(Sign sign);
char signEquals(Sign a, Sign b);
char *signToCharP(Sign sign);

Sign signPositive() {
    Sign result = { ._value = "+ " };
    return result;
}

Sign signNegative() {
    Sign result = { ._value = "- " };
    return result;
}

Sign signReversed(Sign sign) {
    return signEquals(sign, signNegative()) ? signPositive() : signNegative();
}

char signEquals(Sign a, Sign b) {
    return !strcmp(a._value, b._value);
}

char *signToCharP(Sign sign) {
    return sign._value;
}

#endif
