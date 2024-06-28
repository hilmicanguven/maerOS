#include "stdio.h"
#include "peachos.h"
#include "stdlib.h"
#include <stdarg.h>


int putchar(int c)
{
    maeros_putchar((char)c);
    return 0;
}

int printf(const char *fmt, ...)
{
    va_list ap;
    const char *p;
    char* sval;
    int ival;

    va_start(ap, fmt);
    for (p = fmt; *p; p++)
    {
        if (*p != '%')
        {
            putchar(*p);
            continue;
        }

        switch (*++p)
        {
        case 'i':   //integer
            ival = va_arg(ap, int);
            print(itoa(ival));
            break;

        case 's':   //character
            sval = va_arg(ap, char *);
            print(sval);
            break;

        default:
            putchar(*p);
            break;
        }
    }

    va_end(ap);

    return 0;
}