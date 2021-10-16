#pragma once

#include <Adafruit_GFX.h>
#include "ST7735_t3.h" // Local copy from TD1.48 that works for 0.96" IPS 160x80 display
#include "Voice.h"

#include "Fonts/Org_01.h"
#include "Yeysk16pt7b.h"
#include "Fonts/FreeSansBold18pt7b.h"
#include "Fonts/FreeSans12pt7b.h"
#include "Fonts/FreeSans9pt7b.h"
#include "Fonts/FreeSansOblique24pt7b.h"
#include "Fonts/FreeSansBoldOblique24pt7b.h"

#define PULSE 1
#define VAR_TRI 2
#define FILTER_ENV 3
#define AMP_ENV 4

void startTimer();

void setMIDIClkSignal(bool val);

bool getMIDIClkSignal();

void renderBootUpPage();

void renderPeak();

void renderCurrentPatchPage();

void renderPulseWidth(float value);

void renderVarTriangle(float value);

void renderEnv(float att, float dec, float sus, float rel);

void renderCurrentParameterPage();

void renderDeletePatchPage();

void renderDeleteMessagePage();

void renderSavePage();

void renderReinitialisePage();

void renderPatchNamingPage();

void renderRecallPage();

void showRenamingPage(String newName);

void renderUpDown(uint16_t  x, uint16_t  y, uint16_t  colour);

void renderSettingsPage();

void showCurrentParameterPage( const char *param, float val, int pType);

void showCurrentParameterPage(const char *param, String val, int pType);

void showCurrentParameterPage(const char *param, String val);

void showPatchPage(String number, String patchName);

void showSettingsPage(const char *  option, const char * value, int settingsPart);

void enableScope(boolean enable);

void displayThread();

void setupDisplay();
