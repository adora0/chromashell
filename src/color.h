/*
 * Color string conversions
 */

#ifndef COLOR_H
#define COLOR_H

#include <stdbool.h>

typedef struct Color
{
    unsigned int red;
    unsigned int green;
    unsigned int blue;
} Color;

int hex_string_to_color(const char *, Color *, bool);
int __hex_string_to_color(const char *, Color *);

#endif
