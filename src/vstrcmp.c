#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#include "vstrcmp.h"

/*
 * Return if string s is equal to one of the variadic string arguments,
 * where n is the number of proceeding arguments
 */

bool vstrcmp(const char *s, unsigned int n, ...)
{
    va_list args;
    va_start(args, n);
    
    for (int i = 0; i < n; ++i)
    {
        if (strcmp(s, va_arg(args, const char *)) == 0)
        {
            /* Succeed if equal to any comparison string */
            va_end(args);
            return true;
        }
    }

    va_end(args);
    return false;
}
