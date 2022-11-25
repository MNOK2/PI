#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "multi_int.h"
#include "common.h"

int main() {
    srand((unsigned int)time(NULL)); rand();

    int intA = randomRangeInt(-100000, 100000);
    int intB = randomRangeInt(-10000, 10000);

    MultiInt multiIntA, multiIntB;
    MultiInt sum, diff, pro, quo, rem;

    printf("(int)a = %d\n", intA);
    printf("(int)b = %d\n", intB);

    putchar('\n');

    if (intToMultiInt(&multiIntA, intA) || intToMultiInt(&multiIntB, intB)) {
        printf("intをMultiIntに変換できませんでした。\n");
        return 1;
    }

    multiIntPrintWithName(&multiIntA, "a");
    multiIntPrintWithName(&multiIntB, "b");

    putchar('\n');

    if (multiIntAdd(&sum, &multiIntA, &multiIntB)) {
        printf("加算に失敗しました（絶望）\n");
        return 1;
    }
    else multiIntPrintWithName(&sum, "a + b");
    if (multiIntSub(&diff, &multiIntA, &multiIntB)) {
        printf("減算に失敗しました（絶望）\n");
        return 1;
    }
    else multiIntPrintWithName(&diff, "a - b");
    if (multiIntMul(&pro, &multiIntA, &multiIntB)) {
        printf("乗算に失敗しました（絶望）\n");
        return 1;
    }
    else multiIntPrintWithName(&pro, "a * b");

    if (multiIntIsZero(&multiIntB)) {
        printf("除数が0のため計算を中止します。\n");
        return 0;
    }

    if (multiIntDiv(&quo, &multiIntA, &multiIntB)) {
        printf("除算に失敗しました（絶望）\n");
        return 1;
    }
    else multiIntPrintWithName(&quo, "a / b");
    if (multiIntMod(&rem, &multiIntA, &multiIntB)) {
        printf("剰余算に失敗しました（絶望）\n");
        return 1;
    }
    else multiIntPrintWithName(&rem, "a % b");
    
    return 0;
}
