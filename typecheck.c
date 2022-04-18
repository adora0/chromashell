#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>
#include "typecheck.h"

bool is_hex(const char *s)
{
    bool ret = true;
    char *digits = "0123456789abcdef";
    for (int i = 0; i < strlen(s); ++i)
    {
        if (strchr(digits, tolower(s[i])) == NULL)
        {
            ret = false;
        }
    }

    return ret;
}

bool is_uint(const char *s)
{
    bool ret = true;
    char *digits = "0123456789";
    for (int i = 0; i < strlen(s); ++i)
    {
        if (strchr(digits, s[i]) == NULL)
        {
            ret = false;
        }
    }

    return ret;
}