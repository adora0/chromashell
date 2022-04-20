/*
* Command line routines
*/

#ifndef SHELL_H
#define SHELL_H

#include <sys/ioctl.h>

#include "color.h"
#include "i18n.h"
#include "segment.h"

#define UNICODE_FULL_BLOCK  "\u2588"

typedef struct winsize winsize;

int split_optargs(char ***, char *, const char *, unsigned int);
void print_segment(winsize *, Segment *);
void term_set_fg(Color);
void term_unset_color(void);
void display_presets(SegmentGroup *, unsigned int);
void display_help(const char *);
void display_version(void);

#endif