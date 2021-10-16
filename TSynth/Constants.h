#pragma once

#include <Arduino.h>
#include <stdint.h>

#define MONOPHONIC_OFF 0
#define MONOPHONIC_LAST 1
#define MONOPHONIC_FIRST 2
#define MONOPHONIC_HIGHEST 3
#define MONOPHONIC_LOWEST 4
// Legato isn't supported, the envelope state from the previous note needs to transfer to the new note
#define MONOPHONIC_LEGATO 5

#define WAVEFORM_PARABOLIC 103
#define WAVEFORM_HARMONIC 104

// Display page constants
#define PARAMETER 0     //The main page for displaying the current patch and control (parameter) changes
#define RECALL 1        //Patches list
#define SAVE 2          //Save patch page
#define REINITIALISE 3  // Reinitialise message
#define PATCH 4         // Show current patch bypassing PARAMETER
#define PATCHNAMING 5   // Patch naming page
#define DELETE 6        //Delete patch page
#define DELETEMSG 7     //Delete patch message page
#define SETTINGS 8      //Settings page
#define SETTINGSVALUE 9 //Settings page


#define RE_READ -99
#define PWMWAVEFORM WAVEFORM_SINE
#define PATCHES_LIMIT 999

extern const char* VERSION;
extern const float PROGMEM FILTERFREQS256[256];
extern const float POWER[128];
extern const float NOTEFREQS[128];
extern const uint32_t ENVTIMES[128];
extern const float LFOTEMPO[128];
extern const String LFOTEMPOSTR[128];
extern const uint32_t OSCMIXA[128];
extern const uint32_t OSCMIXB[128];
extern const int PITCH[128];
extern const float KEYTRACKINGAMT[128];
extern const float DIV8192;
extern const float DIV127;
extern const float DIV12;
extern const float DIV24;
extern const float DIV1270;
extern const float LINEAR[128];
extern const float LINEARCENTREZERO[128];
extern const float BANDPASS;
extern const float LOWPASS;
extern const float HIGHPASS;
extern const float LINEAR_FILTERMIXER[128];
extern const int16_t PROGMEM PARABOLIC_WAVE[256];
extern const int16_t PROGMEM HARMONIC_WAVE[256];
extern const int16_t PROGMEM PPG_WAVE[256];
extern const float AWFREQ;
extern const float PWMRATE_PW_MODE;
extern const float PWMRATE_SOURCE_FILTER_ENV;
extern const float PWMRATE[128];
extern const float PITCHLFOOCTAVERANGE;
extern const uint32_t MINUNISONVOICES;
extern const float LFOMAXRATE;
extern const uint8_t PWMSOURCELFO;
extern const uint8_t PWMSOURCEFENV;
extern const float ONE;
extern const float SGTL_MAXVOLUME;
extern const float WAVEFORMLEVEL;
extern const float VOICEMIXERLEVEL;
extern const float UNISONVOICEMIXERLEVEL;
extern const float UNISONNOISEMIXERLEVEL;
extern const float OSCMODMIXERMAX;
extern const float FILTERMODMIXERMAX;
extern const float GLIDEFACTOR;
extern const uint32_t NO_OF_PARAMS;
extern const char* INITPATCHNAME;
extern const uint32_t HOLD_DURATION;
extern const uint32_t CLICK_DURATION;
extern const float ENSEMBLE_LFO[128];
extern const char* INITPATCH;
