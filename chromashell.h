#ifndef CHROMASHELL_H
#define CHROMASHELL_H

#include <stdbool.h>
#include <stdio.h>
#include <sys/ioctl.h>

#include <config.h>

#define VERSION_STRING      chromashell_VERSION_MAJOR"."chromashell_VERSION_MINOR
#define VERSION_YEAR        chromashell_VERSION_YEAR

#define DEFAULT_PRESET_PATH     INSTALL_PREFIX"/"chromashell_SYSTEM_DIR"/presets.json"

#define JSON_PRESET_NAME        "name"
#define JSON_PRESET_SEGMENTS    "segments"
#define JSON_PRESET_COLOR       "color"
#define JSON_PRESET_HEIGHT      "height"

typedef struct Color
{
    unsigned int red;
    unsigned int green;
    unsigned int blue;
} Color;

typedef struct Segment
{
    Color color;
    unsigned int height;
} Segment;

typedef struct SegmentGroup
{
    const char *name;
    unsigned int length;
    Segment *segments;
} SegmentGroup;

typedef struct winsize winsize;

int split_optargs(char ***, char *, const char *, unsigned int);
bool is_hex(const char *);
bool is_uint(const char *);
int hex_string_to_color(const char *, Color *, bool);
int __hex_string_to_color(const char *, Color *);
void print_segment(winsize *, Segment *);
int load_presets(SegmentGroup **, FILE *);
void display_presets(SegmentGroup *, unsigned int);
void display_help(const char *);
void display_version(void);
int err_alloc(void);
int err_no_arg(const char *, const char *);
int err_no_preset(const char *, const char *);
int err_no_opt(const char *, const char *);
int err_parse_color(const char *, const char *);
int err_parse_json(const char *, const char *);
int err_no_file(const char *, const char *);
int err_json_root_not_array(void);
int err_json_no_object(const char *);
int err_json_not_string(const char *);
int err_json_not_int(const char *);
int err_json_not_number(const char *);
int err_json_not_array(const char *);
int err_json_parse_color(const char *);

#endif