#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "color.h"
#include "typecheck.h"

int hex_string_to_color(const char *str, Color *color, bool has_prefix)
{
    if (has_prefix)
    {
        int input_len = strlen(str);
        if (input_len == 7 && str[0] == '#')
        {
            char substr[input_len - 1];
            strcpy(substr, &str[1]);
            return __hex_string_to_color(substr, color);
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return __hex_string_to_color(str, color);
    }
}

int __hex_string_to_color(const char *str, Color *color)
{
    if (strlen(str) == 6 && is_hex(str))
    {
        char s_red[3], s_green[3], s_blue[3];
        sprintf(s_red, "%c%c", str[0], str[1]);
        sprintf(s_green, "%c%c", str[2], str[3]);
        sprintf(s_blue, "%c%c", str[4], str[5]);
        color->red = (unsigned int) strtol(s_red, NULL, 16);
        color->green = (unsigned int) strtol(s_green, NULL, 16);
        color->blue = (unsigned int) strtol(s_blue, NULL, 16);
    }
    else
    {
        return -1;
    }

    return 0;
}