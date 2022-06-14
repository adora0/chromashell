#include <cjson/cJSON.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errmsg.h"
#include "presets.h"
#include "segment.h"

/*
 * Load presets from file into struct pointed to by dest.
 */

int load_presets(SegmentGroup **dest, FILE *file)
{
    /* Read lines */
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
    
    /* Parse JSON */
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
            /* Process each preset in root array */
            cJSON_ArrayForEach(item, root)
            {
                SegmentGroup preset;

                /* Name */
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
                
                /* Segments */
                cJSON *item_segments = cJSON_GetObjectItem(item, JSON_PRESET_SEGMENTS);
                if (item_segments != NULL)
                {
                    if (cJSON_IsArray(item_segments))
                    {
                        const cJSON *item_segment;
                        Segment *segments = NULL;
                        unsigned int n_segments = 0;

                        /* Process each segment in segment array */
                        cJSON_ArrayForEach(item_segment, item_segments)
                        {
                            Segment segment;

                            /* Color */
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

                            /* Height */
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

                            /* Add to segments */
                            segments = realloc(segments, sizeof(Segment) * ++n_segments);
                            segments[n_segments - 1] = segment;
                        }

                        /* Copy segments into Preset struct */
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

                /* Copy preset into array */
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
            /* Point destination argument to preset array */
            *dest = presets;
        }

        return n_presets;
    }

    return -1;
}
