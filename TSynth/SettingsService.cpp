#include "SettingsService.h"
#include <stdint.h>
#include <vector>

using namespace settings;

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

void SettingsService::refresh_current_values() {
  for(auto option : settingsOptions) {
    option->refresh();
  }
}

// Add new option

void SettingsService::append(SettingsOption &option) {
  option.refresh();
  settingsOptions.push_back(&option);

  if (settingsOptions.size() == 1) {
    selectedSettingIndex = 0;
  }
}

void SettingsService::reset() {
  settingsOptions.clear();
}

// Setting names
const char* SettingsService::current_setting() {
  return settingsOptions[currentSettingIndex()]->name();
}

const char* SettingsService::previous_setting() {
  return settingsOptions[prevSettingIndex()]->name();
}

const char* SettingsService::next_setting() {
  return settingsOptions[nextSettingIndex()]->name();
}

// Values

const char* SettingsService::previous_setting_value() {
  return settingsOptions[prevSettingIndex()]->current_setting_value();
}
const char* SettingsService::next_setting_value() {
  return settingsOptions[nextSettingIndex()]->current_setting_value();
}

const char* SettingsService::current_setting_value() {
  return settingsOptions[currentSettingIndex()]->current_setting_value();
}

const char* SettingsService::current_setting_previous_value() {
  return settingsOptions[currentSettingIndex()]->previous_setting_value();
}

const char* SettingsService::current_setting_next_value() {
  return settingsOptions[currentSettingIndex()]->next_setting_value();
}

// Change settings

void SettingsService::increment_setting() {
  selectedSettingIndex = nextSettingIndex();
}

void SettingsService::decrement_setting() {
  selectedSettingIndex = prevSettingIndex();
}

// Change setting values

void SettingsService::increment_setting_value() {
  settingsOptions[currentSettingIndex()]->increment();
}

void SettingsService::decrement_setting_value() {
  settingsOptions[currentSettingIndex()]->decrement();
}

void SettingsService::save_current_value() {
  settingsOptions[currentSettingIndex()]->save();
  //settingsOptions[currentSettingIndex()].updateHandler(selectedSettingValueIndex, current_setting_value());
}