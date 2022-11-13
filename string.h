#ifndef _H_STRING_
#define _H_STRING_

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define STRING_LENGTH_MAX (1 << 14)

typedef struct _String {
    char value[STRING_LENGTH_MAX];
} String;

String stringPrintf(const char *str, ...);
char stringEquals(String a, String b);

String stringPrintf(const char *str, ...) {
    String result;
    va_list ap;

    va_start(ap, str);
    vsprintf(result.value, str, ap);
    return result;
}

char stringEquals(String a, String b) {
    return !strcmp(a.value, b.value);
}

#endif
