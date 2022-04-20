/*
* ChomaShell
* Display lines of color in a true color terminal.
*/

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
    // Initialize internationalization
    setlocale(LC_ALL, NULL);
    textdomain(PROJECT_NAME);

    // Executable name
    const char *basename = (const char *) argv[0];

    // Initialize variables set by options
    char *presets_path = NULL;
    char *option_presets;
    size_t option_presets_sz = 0;
    size_t n_option_presets = 0;
    size_t len_option_presets = 0;

    Segment *segments = NULL;
    size_t n_segments = 0;
    
    bool arg_parsed = false;

    // Process command line arguments
    for (int argi = 1; argi < argc; ++argi)
    {
        const char *arg = argv[argi];

        // Check each option
        if (vstrcmp(arg, 2, OPT_SEGMENT, OPT_SEGMENT_LONG))
        {
            // Segment properties
            if (++argi < argc)
            {
                char **optargs = NULL;
                int n_optargs = split_optargs(&optargs, argv[argi], OPTARG_SEPARATOR, 2);

                // Check for RRGGBB,HEIGHT
                if (n_optargs >= 2)
                {
                    Segment segment;
                    char *s_color = optargs[0];
                    char *s_height = optargs[1];
                    if (hex_string_to_color(s_color, &segment.color, false) == 0 && is_uint(s_height))
                    {
                        // Convert height string
                        segment.height = (unsigned int) atoi(s_height);

                        // Extend segments
                        segments = realloc(segments, sizeof(Segment) * ++n_segments);
                        if (segments != NULL)
                        {
                            // Copy segment
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
            // Preset name
            if (++argi < argc)
            {
                char *optarg = argv[argi];

                // Extend specified presets string (separated by null terminator)
                size_t len_optarg = strlen(optarg);
                size_t new_sz = option_presets_sz + sizeof(char) * len_optarg + 1;
                option_presets = realloc(option_presets, new_sz);
                if (option_presets != NULL)
                {
                    // Copy preset name
                    strcpy(option_presets + option_presets_sz, optarg);
                    option_presets_sz = new_sz;
                    ++n_option_presets;
                    len_option_presets += len_optarg;
                }
                else
                {
                    return err_alloc();
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
            // Configuration path
            if (++argi < argc)
            {
                presets_path = argv[argi];
            }
        }
        else if (vstrcmp(arg, 2, OPT_HELP, OPT_HELP_LONG))
        {
            // Help
            display_help(basename);
            return EXIT_SUCCESS;
        }
        else if (vstrcmp(arg, 1, OPT_VERSION_LONG))
        {
            // Version
            display_version();
            return EXIT_SUCCESS;
        }
        else
        {
            // Unrecognized option
            return err_no_opt(basename, arg);
        }
    }

    // Terminal window size
    winsize winsz;
    ioctl(0, TIOCGWINSZ, &winsz);

    if (!arg_parsed)
    {
        // No arguments parsed
        display_help(basename);
        return EXIT_FAILURE;
    }
    else
    {
        if (n_option_presets > 0)
        {
            SegmentGroup *presets = NULL;
            int n_presets = 0;

            // Load presets
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

            char *option_preset = option_presets;
            for (int i = 0; i < n_option_presets; ++i)
            {
                // Find specified preset
                if (vstrcmp(option_preset, 1, OPTARG_PRESET_LIST))
                {
                    // List available presets
                    display_presets(presets, n_presets);
                    return EXIT_SUCCESS;
                }

                bool found_preset = false;
                for (int j = 0; j < n_presets; ++j)
                {
                    const SegmentGroup preset = presets[j];
                    if (vstrcmp(option_preset, 1, preset.name))
                    {
                        found_preset = true;

                        // Copy segments
                        for (int segment_i = 0; segment_i < preset.length; ++segment_i)
                        {
                            // Extend segments
                            segments = realloc(segments, sizeof(Segment) * ++n_segments);
                            if (segments != NULL)
                            {
                                // Copy segment
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
                    return err_no_preset(basename, option_preset);
                }

                // Get next specified preset name
                char *tmp = strchr(option_preset, 0);
                if (tmp != NULL && tmp < option_presets + len_option_presets + 1)
                {
                    option_preset = tmp + 1;
                }
                else
                {
                    i = n_option_presets;
                    break;
                }
            }
        }

        if (n_segments > 0)
        {
            // Print segments
            for (int i = 0; i < n_segments; ++i)
            {
                print_segment(&winsz, &segments[i]);
            }

            free(segments);
        }
    }

    return EXIT_SUCCESS;
}