#pragma once

#include <stdint.h>
#include "Arduino.h"

#define RE_READ -99
#define PWMWAVEFORM WAVEFORM_SINE
#define PATCHES_LIMIT 999

extern const char* VERSION;
extern const uint16_t PROGMEM FILTERFREQS256[256];
extern const float PROGMEM POWER[128];
extern const float PROGMEM NOTEFREQS[128];
extern const uint16_t PROGMEM ENVTIMES[128];
extern const float PROGMEM LFOTEMPO[128];
extern const String LFOTEMPOSTR[128];
extern const uint8_t PROGMEM OSCMIXA[128];
extern const uint8_t PROGMEM OSCMIXB[128];
extern const int8_t PROGMEM PITCH[128];
extern const float PROGMEM KEYTRACKINGAMT[128];
extern const float PROGMEM DIV8192;
extern const float PROGMEM DIV127;
extern const float PROGMEM DIV12;
extern const float PROGMEM DIV24;
extern const float PROGMEM DIV1270;
extern const float PROGMEM LINEAR[128];
extern const float PROGMEM LINEARCENTREZERO[128];
extern const int8_t PROGMEM BANDPASS;
extern const int8_t PROGMEM LOWPASS;
extern const int8_t PROGMEM HIGHPASS;
extern const float PROGMEM LINEAR_FILTERMIXER[128];
extern const int16_t PROGMEM PARABOLIC_WAVE[256];
extern const int16_t PROGMEM HARMONIC_WAVE[256];
extern const int16_t PROGMEM PPG_WAVE[256];
extern const float PROGMEM AWFREQ;
extern const float PROGMEM PWMRATE_PW_MODE;
extern const float PROGMEM PWMRATE_SOURCE_FILTER_ENV;
extern const float PROGMEM PWMRATE[128];
extern const float PROGMEM PITCHLFOOCTAVERANGE;
extern const uint8_t PROGMEM MINUNISONVOICES;
extern const float PROGMEM LFOMAXRATE;
extern const uint8_t PROGMEM PWMSOURCELFO;
extern const uint8_t PROGMEM PWMSOURCEFENV;
extern const float PROGMEM ONE;
extern const float PROGMEM SGTL_MAXVOLUME;
extern const float PROGMEM WAVEFORMLEVEL;
extern const float PROGMEM VOICEMIXERLEVEL;
extern const float PROGMEM UNISONVOICEMIXERLEVEL;
extern const float PROGMEM UNISONNOISEMIXERLEVEL;
extern const float PROGMEM OSCMODMIXERMAX;
extern const float PROGMEM FILTERMODMIXERMAX;
extern const float PROGMEM GLIDEFACTOR;
extern const uint8_t PROGMEM NO_OF_PARAMS;
extern const char* INITPATCHNAME;
extern const uint16_t PROGMEM HOLD_DURATION;
extern const uint16_t PROGMEM CLICK_DURATION;
extern const float PROGMEM ENSEMBLE_LFO[128];
extern const char* INITPATCH;
