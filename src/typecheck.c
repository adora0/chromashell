#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>
#include "typecheck.h"

static const char *hex_digits_lower = "0123456789abcdef";
static const char *digits = "0123456789";

bool is_hex(const char *s)
{
    for (int i = 0; i < strlen(s); ++i)
    {
        if (strchr(hex_digits_lower, tolower(s[i])) == NULL)
        {
            return false;
        }
    }

    return true;
}

bool is_uint(const char *s)
{
    for (int i = 0; i < strlen(s); ++i)
    {
        if (strchr(digits, s[i]) == NULL)
        {
            return false;
        }
    }

    return true;
}