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

#include <vector>
#include <stdint.h>

#define SETTINGSVALUESNO 19//Maximum number of settings option values needed

namespace settings {

//Function to handle the values for this settings option
typedef void (*updater)(int index, const char* value);

//Function to array index of current value for this settings option
typedef int (*index)();

class SettingsOption {

  public:
  virtual const char* current_setting_value() = 0;
  virtual const char* previous_setting_value() = 0;
  virtual const char* next_setting_value() = 0;

  virtual void refresh() = 0;
  virtual void increment() = 0;
  virtual void decrement() = 0;
  virtual void save() = 0;
  virtual const char* name() = 0;
};

class ArrayOption : public SettingsOption
{
  private:
  const char * optionName;//Settings option string
  const char ** values;
  settings::updater updateHandler;
  settings::index currentIndex;
  uint8_t i;
  //const char * values[SETTINGSVALUESNO];

  public:
  // values must be null terminated.
  ArrayOption(const char* name, const char** list, settings::updater updateHandler, settings::index currentIndex):
    optionName(name),
    values(list),
    updateHandler(updateHandler),
    currentIndex(currentIndex)
  {}

  virtual const char* name() {
    return optionName;
  }

  virtual void refresh() {
    i = currentIndex();
  }

  virtual const char* current_setting_value() {
    return values[i];
  };

  virtual const char* previous_setting_value() {
    if (i == 0) {
      return "";
    }
    return values[i - 1];
  }

  virtual const char* next_setting_value() {
    if (*(values[i + 1]) == '\0') {
      return "";
    }
    return values[i + 1];
  }

  virtual void increment() {
    if (*(values[i + 1]) == '\0') {
      return;
    }
    ++i;
  }

  virtual void decrement() {
    if (i == 0) {
      return;
    }
    --i;
  }

  virtual void save() {
    updateHandler(i, values[i]);
  }
};

class SettingsService {
  private:
  // global settings buffer
  std::vector<settings::SettingsOption*> settingsOptions;
  // currently selected settings option value index
  int selectedSettingIndex;
  //int selectedSettingValueIndex;

  public:
  SettingsService(): selectedSettingIndex(0) {};

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

  void append(SettingsOption &option);
  void refresh_current_values();
  void reset();

  private:
  int currentSettingIndex();
  int nextSettingIndex();
  int prevSettingIndex();
};

}
