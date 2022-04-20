/*
* Preset loading routines
*/

#ifndef PRESETS_H
#define PRESETS_H

#include <stdio.h>

#include "segment.h"

#define JSON_PRESET_NAME        "name"
#define JSON_PRESET_SEGMENTS    "segments"
#define JSON_PRESET_COLOR       "color"
#define JSON_PRESET_HEIGHT      "height"

int load_presets(SegmentGroup **, FILE *);

#endif