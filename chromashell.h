#ifndef CHROMASHELL_H
#define CHROMASHELL_H

#include <stdbool.h>
#include <sys/ioctl.h>

#define VERSION_STRING      "1.0"
#define VERSION_YEAR        "2022"

struct segment
{
    unsigned int red;
    unsigned int green;
    unsigned int blue;
    unsigned int height;
};

int split_optargs(char ***, char *, const char *, unsigned int);
bool is_hex(const char *);
bool is_uint(const char *);
void print_segment(struct winsize *, struct segment *);
void display_examples(const char *);
void display_help(const char *);
void display_version(void);
int err_alloc(void);
int err_no_arg(const char *, const char *);
int err_no_opt(const char *, const char *);

#endif
