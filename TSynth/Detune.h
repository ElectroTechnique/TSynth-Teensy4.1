#pragma once

#include <stdint.h>

#define MAXDETUNE 0.06f
#define CDT_DETUNE 0.992f

extern const float DETUNE[4][24];
extern const char* CDT_STR[128];
extern const uint32_t CHORD_DETUNE[12][128];