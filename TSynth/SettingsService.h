// Global settings service.
//
// Settings can be appended from anywhere in the code and they are displayed
// as part of the menu system. Callbacks are used to manage initializing and
// setting values.
//
// Some features which are currently planned:
// - Different types of SettingsOptions. Currently there is a single "Array"
//   option. It would be nice to have a "text" input type which allowed for
//   editing text instead of selecting a value.
// - Multiple menus. For example "settings" opens the current menu, the encoder
//   opens the patch menu, holding the encoder opens the timbre menu.

#pragma once

#define SETTINGSOPTIONSNO 16//No of options
#define SETTINGSVALUESNO 19//Maximum number of settings option values needed

namespace settings {

//Function to handle the values for this settings option
typedef void (*updater)(int index, const char* value);

//Function to array index of current value for this settings option
typedef int (*index)();

struct SettingsOption
{
  const char * option;//Settings option string
  const char * value[SETTINGSVALUESNO];//Array of strings of settings option values
  updater updateHandler;
  index currentIndex;
};

// setting names
const char* current_setting();
const char* previous_setting();
const char* next_setting();

const char* previous_setting_value();
const char* next_setting_value();

const char* current_setting_value();
const char* current_setting_previous_value();
const char* current_setting_next_value();

void increment_setting();
void decrement_setting();

void increment_setting_value();
void decrement_setting_value();

void save_current_value();

void append(SettingsOption option);
void reset();

}