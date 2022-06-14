/*
 * Segment and segment array definitions
 */

#ifndef SEGMENT_H
#define SEGMENT_H

#include "color.h"

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

#endif
