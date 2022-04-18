#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include "errmsg.h"
#include "segment.h"
#include "chromashell.h"
#include "shell.h"

int split_optargs(char ***dest, char *optarg, const char *delim, unsigned int max_args)
{
    char **buf = *dest;
    char *tok = strtok(optarg, delim);
    int n_args = 0;
    while (tok != NULL && n_args < max_args)
    {
        buf = realloc(buf, sizeof(char *) * ++n_args);
        if (buf != NULL)
        {
            buf[n_args - 1] = tok;
            tok = strtok(NULL, delim);
        }
        else
        {
            exit(err_alloc());
        }
    }
    
    *dest = buf;
    return n_args;
}

void print_segment(winsize *winsz, Segment *segment)
{
    term_set_fg(segment->color);

    for (int line = 0; line < segment->height; ++line)
    {
        for (int col = 0; col < winsz->ws_col; ++col)
        {
            printf(UNICODE_FULL_BLOCK);
        }
    }

    term_unset_color();
}

void term_set_fg(Color color)
{
    printf("\033[38;2;%i;%i;%im", color.red, color.green, color.blue);
}

void term_unset_color(void)
{
    printf("\n\033[0m");
}

void display_presets(SegmentGroup *presets, unsigned int n_presets)
{
    printf("Available presets:\n ");
    for (int i = 0; i < n_presets; ++i)
    {
        printf("%s", presets[i].name);
        if (i < n_presets - 1)
        {
            printf(" ");
        }
    }
    printf("\n");
}

void display_help(const char *basename)
{
    printf(
        "Usage: %s OPTIONS ...\n"
        "Display lines of color in a true color terminal.\n"
        " -s,   --segment RRGGBB,HEIGHT         specify segment hex color and height in lines\n"
        " -p,   --preset NAME                   display preset\n"
        "                                         use '--preset list' to display available preset names\n"
        " -c,   --config FILE                   specify path of preset configuration JSON\n"
        " -h,   --help                          display this help and exit\n"
        "       --version                       display version information and exit\n",
        basename);
}

void display_version(void)
{
    printf(
        "ChromaShell %s\n"
        "Copyright (c) %s Kriss Wiggins.\n"
        "License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.\n"
        "This is free software: you are free to change and redistribute it.\n"
        "There is NO WARRANTY, to the extent permitted by law.\n",
        VERSION_STRING, VERSION_YEAR);
}