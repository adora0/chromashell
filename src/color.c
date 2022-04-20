#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "color.h"
#include "typecheck.h"

/*
* Convert hexadecimal RRGGBB color as a string to struct pointed to by color.
* Set has_prefix to true if the format is #RRGGBB
*/
int hex_string_to_color(const char *str, Color *color, bool has_prefix)
{
    if (has_prefix)
    {
        // Check first character
        int input_len = strlen(str);
        if (input_len == 7 && str[0] == '#')
        {
            // Check remaining characters
            char substr[input_len - 1];
            strcpy(substr, &str[1]);
            return __hex_string_to_color(substr, color);
        }
        else
        {
            // Failure
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
    // Check length (RRGGBB) and if all characters are hex digits
    if (strlen(str) == 6 && is_hex(str))
    {
        // Assign 2 characters + null terminator for each color
        char s_red[3], s_green[3], s_blue[3];

        // Copy characters 0-1, 2-3 and 4-5 into red, green and blue
        sprintf(s_red, "%c%c", str[0], str[1]);
        sprintf(s_green, "%c%c", str[2], str[3]);
        sprintf(s_blue, "%c%c", str[4], str[5]);

        // Convert color digit strings to unsigned int and copy into Color struct
        color->red = (unsigned int) strtol(s_red, NULL, 16);
        color->green = (unsigned int) strtol(s_green, NULL, 16);
        color->blue = (unsigned int) strtol(s_blue, NULL, 16);
    }
    else
    {
        // Failure
        return -1;
    }

    // Success
    return 0;
}