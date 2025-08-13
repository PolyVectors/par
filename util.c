#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

void
panic(const char *fmt, ...)
{
    va_list args;
    va_start (args, fmt);

    fputs("\x1b[1;91merror\x1b[0m: ", stdout);
    vprintf(fmt, args);
    exit(1);
}