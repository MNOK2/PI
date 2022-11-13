#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "number.h"

void initialize();

int main() {
    initialize();
    
    for (int i = -1; i <= 1; i++) for (int j = -1; j <= 1; j++) printf("(%s) + (%s) = (%s)\n", numberToString(intToNumber(i)).value, numberToString(intToNumber(j)).value, numberToString(numberAdd(intToNumber(i), intToNumber(j))).value);

    return 0;
}

void initialize() {
    srand((unsigned int)time(NULL));
    rand();
}
