#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "multi_int.h"

void randInitialize();
void testMultiIntCalculationOnce();
void testMultiIntCalculationMany();
int main(int, const char **);

int main(int argc, const char **argv) {
    randInitialize();
    testMultiIntCalculationOnce();

    return 0;
}

void randInitialize() {
    srand((unsigned int)time(NULL));
    rand();
}

void testMultiIntCalculationOnce() {
    int intA = randomRangeInt(-100, 100);
    int intB = randomRangeInt(-100, 100);
    MultiInt multiIntA = intToMultiInt(intA);
    MultiInt multiIntB = intToMultiInt(intB);

    if (!intB) {
        printf("除数が0であるため計算を中止します。\n");
        return;
    }

    printf("=== int ===\n");
    printf("a = %d\n", intA);
    printf("b = %d\n", intB);
    printf("a + b = %d\n", intA + intB);
    printf("a - b = %d\n", intA - intB);
    printf("a * b = %d\n", intA * intB);
    printf("a / b = %d\n", intA / intB);
    printf("a %% b = %d\n", intA % intB);

    putchar('\n');

    printf("=== MultiInt ===\n");
    multiIntPrintWithName("a", multiIntA);
    multiIntPrintWithName("b", multiIntB);
    multiIntPrintWithName("a + b", multiIntAdd(multiIntA, multiIntB));
    multiIntPrintWithName("a - b", multiIntSub(multiIntA, multiIntB));
    multiIntPrintWithName("a * b", multiIntMul(multiIntA, multiIntB));
    multiIntPrintWithName("a / b", multiIntDiv(multiIntA, multiIntB));
    multiIntPrintWithName("a % b", multiIntMod(multiIntA, multiIntB));
    multiIntPrintWithName("sqrt(|a|)", multiIntSqrt(multiIntAbs(multiIntA)));
    multiIntPrintWithName("fact(|a|)", multiIntFact(multiIntAbs(multiIntA)));
    multiIntPrintWithName("pow(a, |b|)", multiIntPow(multiIntA, multiIntAbs(multiIntB)));
    multiIntPrintWithName("gcd(a, b)", multiIntGCD(multiIntA, multiIntB));
    multiIntPrintWithName("lcm(a, b)", multiIntLCM(multiIntA, multiIntB));
    printf("a is prime = %s\n", multiIntIsPrime(multiIntA) ? "TRUE" : "FALSE");
}

void testMultiIntCalculationMany() {
    int intA, intB;
    MultiInt multiIntA, multiIntB;

    printf("連続計算を開始します。\n");
    for (int i = 0; i < 10000; i++) {
        if (!((i + 1) % 1000)) printf("%d回目の計算中……\n", i + 1);
        intA = randomRangeInt(-10000, 10000);
        intB = randomRangeInt(-10000, 10000);
        multiIntA = intToMultiInt(intA);
        multiIntB = intToMultiInt(intB);
        if (intA + intB != multiIntToInt(multiIntAdd(multiIntA, multiIntB))) {
            printf("加算に失敗しました。\n");
            multiIntPrintWithName("a", multiIntA);
            multiIntPrintWithName("b", multiIntB);
            multiIntPrintWithName("a + b", multiIntAdd(multiIntA, multiIntB));
            return;
        }
        if (intA - intB != multiIntToInt(multiIntSub(multiIntA, multiIntB))) {
            printf("減算に失敗しました。\n");
            multiIntPrintWithName("a", multiIntA);
            multiIntPrintWithName("b", multiIntB);
            multiIntPrintWithName("a - b", multiIntSub(multiIntA, multiIntB));
            return;
        }
        if (intA * intB != multiIntToInt(multiIntMul(multiIntA, multiIntB))) {
            printf("乗算に失敗しました。\n");
            multiIntPrintWithName("a", multiIntA);
            multiIntPrintWithName("b", multiIntB);
            multiIntPrintWithName("a * b", multiIntMul(multiIntA, multiIntB));
            return;
        }
        if (multiIntIsZero(multiIntB)) continue;
        if (intA / intB != multiIntToInt(multiIntDiv(multiIntA, multiIntB))) {
            printf("除算に失敗しました。\n");
            multiIntPrintWithName("a", multiIntA);
            multiIntPrintWithName("b", multiIntB);
            multiIntPrintWithName("a / b", multiIntDiv(multiIntA, multiIntB));
            return;
        }
        if (intA % intB != multiIntToInt(multiIntMod(multiIntA, multiIntB))) {
            printf("剰余算に失敗しました。\n");
            multiIntPrintWithName("a", multiIntA);
            multiIntPrintWithName("b", multiIntB);
            multiIntPrintWithName("a % b", multiIntMod(multiIntA, multiIntB));
            return;
        }
    }
    printf("正常に終了しました。\n");
}
