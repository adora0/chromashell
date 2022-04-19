#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include "vstrcmp.h"

bool vstrcmp(const char *s, unsigned int n, ...)
{
    va_list args;
    va_start(args, n);
    
    for (int i = 0; i < n; ++i)
    {
        if (strcmp(s, va_arg(args, const char *)) == 0)
        {
            va_end(args);
            return true;
        }
    }

    va_end(args);
    return false;
}