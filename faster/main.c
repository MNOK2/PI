#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "number.h"

void initialize();

int main() {
    initialize();

    int intA, intB;
    Number a, b, sum, diff, pro, quo, rem;
    
    intA = randomRangeInt(-9999, 10000);
    intB = randomRangeInt(-999, 1000);
    intToNumber(intA, &a);
    intToNumber(intB, &b);

    numberPrintWithName(&a, "a");
    numberPrintWithName(&b, "b");

    putchar('\n');

    if (tryNumberAdd(&a, &b, &sum)) numberPrintWithName(&sum, "a + b");
    else printf("加算に失敗しました（絶望）");
    if (tryNumberSub(&a, &b, &diff)) numberPrintWithName(&diff, "a - b");
    else printf("減算に失敗しました（絶望）");
    if (tryNumberMul(&a, &b, &pro)) numberPrintWithName(&pro, "a * b");
    else printf("乗算に失敗しました（絶望）");
    if (tryNumberDivMod(&a, &b, &quo, &rem)) {
        numberPrintWithName(&quo, "a / b");
        numberPrintWithName(&rem, "a %% b");
    }
    else printf("除算に失敗しました（絶望）");

    putchar('\n');

    printf("(int)a + (int)b = %d\n", intA + intB);
    printf("(int)a - (int)b = %d\n", intA - intB);
    printf("(int)a * (int)b = %d\n", intA * intB);
    printf("(int)a / (int)b = %d\n", intA / intB);
    printf("(int)a %% (int)b = %d\n", intA % intB);

    return 0;
}

void initialize() {
    srand((unsigned int)time(NULL));
    rand();
}
