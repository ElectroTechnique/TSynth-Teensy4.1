#include "SettingsService.h"
#include <stdint.h>
#include <vector>

using namespace settings;

// currently selected settings option value index
int selectedSettingIndex = 0;
int selectedSettingValueIndex = 0;

// Helpers

int SettingsService::currentSettingIndex() {
  return selectedSettingIndex;
}

int SettingsService::nextSettingIndex() {
  return (selectedSettingIndex + 1) % settingsOptions.size();
}

int SettingsService::prevSettingIndex() {
  if (selectedSettingIndex == 0) {
    return settingsOptions.size() - 1;
  }
  return selectedSettingIndex - 1;
}

void SettingsService::refresh_current_value_index() {
  selectedSettingValueIndex = settingsOptions[currentSettingIndex()].currentIndex();
}

// Add new option

void SettingsService::append(SettingsOption option) {
  settingsOptions.push_back(option);

  if (settingsOptions.size() == 1) {
    selectedSettingIndex = 0;
    refresh_current_value_index();
  }
}

void SettingsService::reset() {
  settingsOptions.clear();
}

// Setting names

const char* SettingsService::current_setting() {
  return settingsOptions[currentSettingIndex()].option;
}

const char* SettingsService::previous_setting() {
  return settingsOptions[prevSettingIndex()].option;
}

const char* SettingsService::next_setting() {
  return settingsOptions[nextSettingIndex()].option;
}

// Values

const char* SettingsService::previous_setting_value() {
  return settingsOptions[prevSettingIndex()].value[settingsOptions[prevSettingIndex()].currentIndex()];
}
const char* SettingsService::next_setting_value() {
  return settingsOptions[nextSettingIndex()].value[settingsOptions[nextSettingIndex()].currentIndex()];
}

const char* SettingsService::current_setting_value() {
  return settingsOptions[currentSettingIndex()].value[selectedSettingValueIndex];
}

const char* SettingsService::current_setting_previous_value() {
  if (selectedSettingValueIndex == 0) {
    return "";
  }
  return settingsOptions[currentSettingIndex()].value[selectedSettingValueIndex - 1];
}

const char* SettingsService::current_setting_next_value() {
  if (settingsOptions[currentSettingIndex()].value[selectedSettingValueIndex + 1][0] == '\0') {
    return "";
  }
  return settingsOptions[currentSettingIndex()].value[selectedSettingValueIndex - 1];
}

// Change settings

void SettingsService::increment_setting() {
  selectedSettingIndex = nextSettingIndex();
  refresh_current_value_index();
}

void SettingsService::decrement_setting() {
  selectedSettingIndex = prevSettingIndex();
  refresh_current_value_index();
}

// Change setting values

void SettingsService::increment_setting_value() {
  if (settingsOptions[currentSettingIndex()].value[selectedSettingValueIndex + 1][0] == '\0') {
    return;
  }
  selectedSettingValueIndex++;
}

void SettingsService::decrement_setting_value() {
  if (selectedSettingValueIndex == 0) {
    return;
  }
  selectedSettingValueIndex--;
}

void SettingsService::save_current_value() {
  settingsOptions[currentSettingIndex()].updateHandler(selectedSettingValueIndex, current_setting_value());
}