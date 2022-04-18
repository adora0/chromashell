#include <stdio.h>
#include <stdlib.h>
#include "errmsg.h"

int err_alloc(void)
{
    printf("Error (re)allocating memory\n");
    return EXIT_FAILURE;
}

int err_no_arg(const char *basename, const char *opt)
{
    printf(
        "%s: missing required argument for '%s'\n"
        "Try '%s --help' for more information.\n",
        basename, opt, basename);
    return EXIT_FAILURE;
}

int err_no_preset(const char *basename, const char *optarg)
{
    printf(
        "%s: unrecognized preset '%s'\n"
        "Try '%s --preset list' to display available preset names,"
        " or '%s --help' for more information.\n",
        basename, optarg, basename, basename);
    return EXIT_FAILURE;
}

int err_no_opt(const char *basename, const char *opt)
{
    printf(
        "%s: unrecognized option '%s'\n"
        "Try '%s --help' for more information.\n",
        basename, opt, basename);
    return EXIT_FAILURE;
}

int err_parse_color(const char *basename, const char *str)
{
    printf(
        "%s: invalid hexadecimal color '%s'\n"
        "Try '%s --help' for more information.\n",
        basename, str, basename);
    return EXIT_FAILURE;
}

int err_parse_json(const char *basename, const char *filename)
{
    printf(
        "%s: error parsing JSON file '%s'\n",
        basename, filename);
    return EXIT_FAILURE;
}

int err_no_file(const char *basename, const char *filename)
{
    printf(
        "%s: file '%s' not found\n",
        basename, filename);
    return EXIT_FAILURE;
}

int err_json_root_not_array(void)
{
    printf("JSON error: root object is not an array\n");
    return -1;
}

int err_json_no_object(const char *name)
{
    printf(
        "JSON error: object '%s' not found\n",
        name);
    return -1;
}

int err_json_not_string(const char *name)
{
    printf(
        "JSON error: object '%s' is not a string\n",
        name);
    return -1;
}

int err_json_not_int(const char *name)
{
    printf(
        "JSON error: object '%s' is not an integer\n",
        name);
    return -1;
}

int err_json_not_number(const char *name)
{
    printf(
        "JSON error: object '%s' is not a number\n",
        name);
    return -1;
}

int err_json_not_array(const char *name)
{
    printf(
        "JSON error: object '%s' is not an array\n",
        name);
    return -1;
}

int err_json_parse_color(const char *str)
{
    printf(
        "JSON error: invalid hexadecimal color '%s'\n",
        str);
    return -1;
}