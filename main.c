#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "number.h"

void initialize();

int main() {
    initialize();
    
    Number a = numberRandom(3);
    Number b = numberRandom(2);

    printf("a = %s\n", numberToString(a).value);
    printf("b = %s\n", numberToString(b).value);
    printf("a + b = %s\n", numberToString(numberAdd(a, b)).value);
    printf("a - b = %s\n", numberToString(numberSub(a, b)).value);

    return 0;
}

void initialize() {
    srand((unsigned int)time(NULL));
    rand();
}
