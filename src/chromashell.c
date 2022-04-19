#include <locale.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include "config.h"
#include "segment.h"
#include "vstrcmp.h"
#include "options.h"
#include "shell.h"
#include "typecheck.h"
#include "errmsg.h"
#include "presets.h"

int main(int argc, char **argv)
{
    setlocale(LC_ALL, NULL);
    textdomain(PROJECT_NAME);

    const char *basename = (const char *) argv[0];
    
    winsize winsz;
    ioctl(0, TIOCGWINSZ, &winsz);

    char *presets_path = NULL;
    SegmentGroup *presets = NULL;
    int n_presets = 0;

    Segment *segments = NULL;
    unsigned int n_segments = 0;
    
    bool arg_parsed = false;

    for (int argi = 1; argi < argc; ++argi)
    {
        const char *arg = argv[argi];
        if (vstrcmp(arg, 2, OPT_SEGMENT, OPT_SEGMENT_LONG))
        {
            if (++argi < argc)
            {
                char **optargs = NULL;
                int n_optargs = split_optargs(&optargs, argv[argi], OPTARG_SEPARATOR, 2);
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
        else if (vstrcmp(arg, 2, OPT_PRESET, OPT_PRESET_LONG))
        {
            if (++argi < argc)
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

                char *optarg = argv[argi];
                if (vstrcmp(optarg, 1, OPTARG_PRESET_LIST))
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
                        if (vstrcmp(optarg, 1, preset.name))
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
        else if (vstrcmp(arg, 2, OPT_CONFIG, OPT_CONFIG_LONG))
        {
            if (++argi < argc)
            {
                presets_path = argv[argi];
            }
        }
        else if (vstrcmp(arg, 2, OPT_HELP, OPT_HELP_LONG))
        {
            display_help(basename);
            return EXIT_SUCCESS;
        }
        else if (vstrcmp(arg, 1, OPT_VERSION_LONG))
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