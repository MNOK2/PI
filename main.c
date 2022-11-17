#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "number.h"

void initialize();

int main() {
    initialize();
    
    Number a = numberRandom(2);
    Number b = numberRandom(1);

    printf("a = %s\n", numberToString(a).value);
    printf("b = %s\n", numberToString(b).value);
    printf("a + b = %s\n", numberToString(numberAdd(a, b)).value);
    printf("a - b = %s\n", numberToString(numberSub(a, b)).value);
    printf("a * b = %s\n", numberToString(numberMul(a, b)).value);
    printf("a / b = %s\n", numberToString(numberDiv(a, b)).value);
    printf("a %% b = %s\n", numberToString(numberMod(a, b)).value);
    printf("(int)a * (int)b = %d\n", numberToInt(a) * numberToInt(b));
    printf("(int)a / (int)b = %d\n", numberToInt(a) / numberToInt(b));
    printf("(int)a %% (int)b = %d\n", numberToInt(a) % numberToInt(b));

    return 0;
}

void initialize() {
    srand((unsigned int)time(NULL));
    rand();
}
