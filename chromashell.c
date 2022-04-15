#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <cjson/cJSON.h>

#include "chromashell.h"

int main(int argc, char **argv)
{
    const char *basename = (const char *) argv[0];
    
    winsize winsz;
    ioctl(0, TIOCGWINSZ, &winsz);

    char *presets_path = NULL;
    SegmentGroup *presets = NULL;
    int n_presets = 0;

    Segment *segments = NULL;
    unsigned int n_segments = 0;
    
    bool arg_parsed = false;

    for (int arg_index = 1; arg_index < argc; ++arg_index)
    {
        const char *arg = argv[arg_index];
        if (strcmp(arg, "-s") == 0 || strcmp(arg, "--segment") == 0)
        {
            if (++arg_index < argc)
            {
                char **optargs = NULL;
                int n_optargs = split_optargs(&optargs, argv[arg_index], ",", 2);
                if (n_optargs >= 2)
                {
                    Segment segment;
                    char *s_color = optargs[0];
                    char *s_height = optargs[1];
                    if (hex_string_to_color(s_color, &segment.color, false) == 0 && is_uint(s_height))
                    {
                        segment.height = (unsigned int) atoi(s_height);
                        segments = realloc(segments, sizeof(Segment) * ++n_segments);
                        if (segments != NULL)
                        {
                            memcpy(&segments[n_segments - 1], &segment, sizeof(Segment));
                            free(optargs);
                        }
                        else
                        {
                            return err_alloc();
                        }
                    }
                    else
                    {
                        display_help(basename);
                        return EXIT_FAILURE;
                    }
                }
                else
                {
                    free(optargs);
                    return err_no_arg(basename, arg);
                }

                arg_parsed = true;
            }
            else
            {
                return err_no_arg(basename, arg);
            }
        }
        else if (strcmp(arg, "-p") == 0 || strcmp(arg, "--preset") == 0)
        {
            if (++arg_index < argc)
            {
                if (!presets)
                {
                    if (!presets_path)
                    {
                        presets_path = DEFAULT_PRESET_PATH;
                    }

                    FILE *file;
                    if (file = fopen(presets_path, "r"))
                    {
                        n_presets = load_presets(&presets, file);
                        if (n_presets < 0)
                        {
                            return err_parse_json(basename, presets_path);
                        }
                    }
                    else
                    {
                        return err_no_file(basename, presets_path);
                    }
                }

                char *optarg = argv[arg_index];
                if (strcmp(optarg, "list") == 0)
                {
                    display_presets(presets, n_presets);
                    return EXIT_SUCCESS;
                }
                else
                {
                    bool found_preset = false;
                    for (int preset_i = 0; preset_i < n_presets; ++preset_i)
                    {
                        const SegmentGroup preset = presets[preset_i];
                        if (strcmp(optarg, preset.name) == 0)
                        {
                            found_preset = true;
                            for (int segment_i = 0; segment_i < preset.length; ++segment_i)
                            {
                                segments = realloc(segments, sizeof(Segment) * ++n_segments);
                                if (segments != NULL)
                                {
                                    segments[n_segments - 1] = preset.segments[segment_i];
                                }
                                else
                                {
                                    return err_alloc();
                                }
                            }
                        }
                    }
                    
                    if (!found_preset)
                    {
                        return err_no_preset(basename, optarg);
                    }
                }

                arg_parsed = true;
            }
            else
            {
                return err_no_arg(basename, arg);
            }
        }
        else if (strcmp(arg, "-c") == 0 || strcmp(arg, "--config") == 0)
        {
            if (++arg_index < argc)
            {
                presets_path = argv[arg_index];
            }
        }
        else if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0)
        {
            display_help(basename);
            return EXIT_SUCCESS;
        }
        else if (strcmp(arg, "--version") == 0)
        {
            display_version();
            return EXIT_SUCCESS;
        }
        else
        {
            return err_no_opt(basename, arg);
        }
    }

    if (!arg_parsed)
    {
        display_help(basename);
        return EXIT_FAILURE;
    }
    else if (n_segments > 0)
    {
        for (int i = 0; i < n_segments; ++i)
        {
            print_segment(&winsz, &segments[i]);
        }

        free(segments);
    }

    return EXIT_SUCCESS;
}

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

void print_segment(winsize *winsz, Segment *segment)
{
    printf(
        "\033[38;2;%i;%i;%im",
        segment->color.red, segment->color.green, segment->color.blue);

    for (int line = 0; line < segment->height; ++line)
    {
        for (int col = 0; col < winsz->ws_col; ++col)
        {
            printf("\u2588");
        }
    }

    printf("\n\033[0m");
}

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

int load_presets(SegmentGroup **dest, FILE *file)
{
    char *json_str = NULL;
    char buf[100];
    int json_strlen = 0;
    while (fscanf(file, "%s", buf) != EOF)
    {
        json_strlen += strlen(buf);
        json_str = realloc(json_str, (json_strlen * sizeof(char)) + 1);

        if (json_str != NULL)
        {
            strcat(json_str, buf);
        }
        else
        {
            exit(err_alloc());
        }
    }
    
    if (json_str != NULL)
    {
        const cJSON *root = cJSON_ParseWithLength(json_str, json_strlen);
        const cJSON *item = NULL;
        int parse_err = 0;
        SegmentGroup *presets = NULL;
        unsigned int n_presets = 0;
        unsigned int presets_sz = 0;
        if (cJSON_IsArray(root))
        {
            cJSON_ArrayForEach(item, root)
            {
                SegmentGroup preset;

                const cJSON *item_name = cJSON_GetObjectItem(item, JSON_PRESET_NAME);
                if (item_name != NULL)
                {
                    if (cJSON_IsString(item_name))
                    {
                        preset.name = cJSON_GetStringValue(item_name);
                    }
                    else
                    {
                        parse_err = err_json_not_string(JSON_PRESET_NAME);
                        break;
                    }
                }
                else
                {
                    parse_err = err_json_no_object(JSON_PRESET_NAME);
                    break;
                }
                
                cJSON *item_segments = cJSON_GetObjectItem(item, JSON_PRESET_SEGMENTS);
                if (item_segments != NULL)
                {
                    if (cJSON_IsArray(item_segments))
                    {
                        const cJSON *item_segment;
                        Segment *segments = NULL;
                        unsigned int n_segments = 0;
                        cJSON_ArrayForEach(item_segment, item_segments)
                        {
                            Segment segment;

                            cJSON *segment_color = cJSON_GetObjectItem(item_segment, JSON_PRESET_COLOR);
                            if (segment_color != NULL)
                            {
                                if (cJSON_IsString(segment_color))
                                {
                                    const char *color_str = cJSON_GetStringValue(segment_color);
                                    Color color;
                                    int color_parse_err = hex_string_to_color(color_str, &color, true);
                                    if (color_parse_err >= 0)
                                    {
                                        segment.color = color;
                                    }
                                    else
                                    {
                                        parse_err = err_json_parse_color(color_str);
                                        break;
                                    }
                                }
                                else
                                {
                                    parse_err = err_json_not_string(JSON_PRESET_COLOR);
                                    break;
                                }
                            }
                            else
                            {
                                parse_err = err_json_no_object(JSON_PRESET_COLOR);
                                break;
                            }

                            cJSON *segment_height = cJSON_GetObjectItem(item_segment, JSON_PRESET_HEIGHT);
                            if (segment_height != NULL)
                            {
                                if (cJSON_IsNumber(segment_height))
                                {
                                    double segment_height_d = cJSON_GetNumberValue(segment_height);
                                    if (segment_height_d == (int) segment_height_d)
                                    {
                                        segment.height = (int) segment_height_d;
                                    }
                                    else
                                    {
                                        parse_err = err_json_not_int(JSON_PRESET_HEIGHT);
                                        break;
                                    }
                                }
                                else
                                {
                                    parse_err = err_json_not_number(JSON_PRESET_HEIGHT);
                                    break;
                                }
                            }
                            else
                            {
                                parse_err = err_json_no_object(JSON_PRESET_HEIGHT);
                                break;
                            }

                            segments = realloc(segments, sizeof(Segment) * ++n_segments);
                            segments[n_segments - 1] = segment;
                        }

                        unsigned int segments_sz = n_segments * sizeof(Segment);
                        preset.segments = malloc(segments_sz);
                        memcpy(preset.segments, segments, segments_sz);
                        preset.length = n_segments;
                    }
                    else
                    {
                        parse_err = err_json_not_array(JSON_PRESET_SEGMENTS);
                        break;
                    }
                }
                else
                {
                    parse_err = err_json_no_object(JSON_PRESET_SEGMENTS);
                    break;
                }

                presets_sz += sizeof(SegmentGroup);
                presets = realloc(presets, presets_sz);
                if (presets != NULL)
                {
                    presets[n_presets] = preset;
                    ++n_presets;
                }
                else
                {
                    exit(err_alloc());
                }
            }
        }
        else
        {
            parse_err = err_json_root_not_array();
        }

        free(json_str);

        if (parse_err != 0)
        {
            return -1;
        }
        else if (presets != NULL)
        {
            *dest = presets;
        }

        return n_presets;
    }

    return -1;
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
        "Copyright (c) %s Adora Zero.\n"
        "License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.\n"
        "This is free software: you are free to change and redistribute it.\n"
        "There is NO WARRANTY, to the extent permitted by law.\n\n"
        "Written by Adora.\n",
        VERSION_STRING, VERSION_YEAR);
}

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