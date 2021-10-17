// This stores all of the settings functions.
#pragma once

#include "VoiceGroup.h"
#include "SettingsService.h"

extern settings::SettingsService mainSettings;

// initialize all the settings objects.
void setUpSettings();

// Helpers to refresh settings.
void reloadFiltEnv();
void reloadAmpEnv();
void reloadGlideShape();