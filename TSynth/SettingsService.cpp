#include "SettingsService.h"
#include <stdint.h>
#include <vector>

// global settings buffer
std::vector<settings::SettingsOption> settingsOptions;
//CircularBuffer<settings::SettingsOption, SETTINGSOPTIONSNO>  settingsOptions;

// currently selected settings option value index
int selectedSettingIndex = 0;
int selectedSettingValueIndex = 0;

// Helpers

int currentSettingIndex() {
  return selectedSettingIndex;
}

int nextSettingIndex() {
  return (selectedSettingIndex + 1) % settingsOptions.size();
}

int prevSettingIndex() {
  if (selectedSettingIndex == 0) {
    return settingsOptions.size() - 1;
  }
  return selectedSettingIndex - 1;
}

void refresh_current_value_index() {
  selectedSettingValueIndex = settingsOptions[currentSettingIndex()].currentIndex();
}

// Add new option

void settings::append(SettingsOption option) {
  settingsOptions.push_back(option);

  if (settingsOptions.size() == 1) {
    selectedSettingIndex = 0;
    refresh_current_value_index();
  }
}

void settings::reset() {
  settingsOptions.clear();
}

// Setting names

const char* settings::current_setting() {
  return settingsOptions[currentSettingIndex()].option;
}

const char* settings::previous_setting() {
  return settingsOptions[prevSettingIndex()].option;
}

const char* settings::next_setting() {
  return settingsOptions[nextSettingIndex()].option;
}

// Values

const char* settings::previous_setting_value() {
  return settingsOptions[prevSettingIndex()].value[settingsOptions[prevSettingIndex()].currentIndex()];
}
const char* settings::next_setting_value() {
  return settingsOptions[nextSettingIndex()].value[settingsOptions[nextSettingIndex()].currentIndex()];
}

const char* settings::current_setting_value() {
  return settingsOptions[currentSettingIndex()].value[selectedSettingValueIndex];
}

const char* settings::current_setting_previous_value() {
  if (selectedSettingValueIndex == 0) {
    return "";
  }
  return settingsOptions[currentSettingIndex()].value[selectedSettingValueIndex - 1];
}

const char* settings::current_setting_next_value() {
  if (settingsOptions[currentSettingIndex()].value[selectedSettingValueIndex + 1][0] == '\0') {
    return "";
  }
  return settingsOptions[currentSettingIndex()].value[selectedSettingValueIndex - 1];
}

// Change settings

void settings::increment_setting() {
  selectedSettingIndex = nextSettingIndex();
  refresh_current_value_index();
}

void settings::decrement_setting() {
  selectedSettingIndex = prevSettingIndex();
  refresh_current_value_index();
}

// Change setting values

void settings::increment_setting_value() {
  if (settingsOptions[currentSettingIndex()].value[selectedSettingValueIndex + 1][0] == '\0') {
    return;
  }
  selectedSettingValueIndex++;
}

void settings::decrement_setting_value() {
  if (selectedSettingValueIndex == 0) {
    return;
  }
  selectedSettingValueIndex--;
}

void settings::save_current_value() {
  settingsOptions[currentSettingIndex()].updateHandler(selectedSettingValueIndex, current_setting_value());
}