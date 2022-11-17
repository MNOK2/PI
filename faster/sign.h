#ifndef _H_SIGN_
#define _H_SIGN_

#include <stdbool.h>
#include "common.h"

typedef enum _Sign {
    SIGN_NEGATIVE,
    SIGN_POSITIVE
} Sign;

Sign signReversed(Sign);
Sign signRandom();

Sign signReversed(Sign sign) {
    return sign == SIGN_NEGATIVE ? SIGN_POSITIVE : SIGN_NEGATIVE;
}

Sign signRandom() {
    return probabilityIsHit(0.5f) ? SIGN_NEGATIVE : SIGN_POSITIVE;
}

#endif
