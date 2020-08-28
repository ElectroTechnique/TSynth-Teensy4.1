#define SETTINGSOPTIONSNO 10
#define SETTINGSVALUESNO 18//Maximum number of settings option values needed
uint32_t settingsValueIndex = 0;//currently selected settings option value index

struct SettingsOption
{
  char * option;//Settings option string
  char *value[SETTINGSVALUESNO];//Array of strings of settings option values
  int  handler;//Function to handle the values for this settings option
  int  currentIndex;//Function to array index of current value for this settings option
};

void settingsMIDICh(char * value);
void settingsVelocitySens(char * value);
void settingsKeyTracking(char * value);
void settingsPitchBend(char * value);
void settingsModWheelDepth(char * value);
void settingsMIDIOutCh(char * value);
void settingsEncoderDir(char * value);
void settingsPickupEnable(char * value);
void settingsBassEnhanceEnable(char * value);
void settingsScopeEnable(char * value);
void settingsHandler(char * s, void (*f)(char*));

int currentIndexMIDICh();
int currentIndexVelocitySens();
int currentIndexKeyTracking();
int currentIndexPitchBend();
int currentIndexModWheelDepth();
int currentIndexEncoderDir();
int currentIndexPickupEnable();
int currentIndexBassEnhanceEnable();
int currentIndexScopeEnable();
int getCurrentIndex(int (*f)());


FLASHMEM void settingsMIDICh(char * value) {
  if (strcmp(value, "ALL") == 0) {
    midiChannel = MIDI_CHANNEL_OMNI;
  } else {
    midiChannel = atoi(value);
  }
  storeMidiChannel(midiChannel);
}

FLASHMEM void settingsVelocitySens(char * value) {
  if (strcmp(value, "Off") == 0) {
    velocitySens = 0;
  } else {
    velocitySens = atoi(value);
  }
}

FLASHMEM void settingsKeyTracking(char * value) {
  if (strcmp(value, "None") == 0) keytrackingAmount = 0;
  if (strcmp(value, "Half") == 0)  keytrackingAmount =  0.5;
  if (strcmp(value, "Full") == 0) keytrackingAmount =  1.0;
}

FLASHMEM void settingsPitchBend(char * value) {
  pitchBendRange = atoi(value);
  storePitchBendRange(pitchBendRange);
}

FLASHMEM void settingsModWheelDepth(char * value) {
  modWheelDepth = atoi(value) / 10.0f;
  storeModWheelDepth(modWheelDepth);
}

FLASHMEM void settingsMIDIOutCh(char * value) {
  if (strcmp(value, "Off") == 0) {
    midiOutCh = 0;
  } else {
    midiOutCh = atoi(value);
  }
  storeMidiOutCh(midiOutCh);
}

FLASHMEM void settingsEncoderDir(char * value) {
  if (strcmp(value, "Type 1") == 0) {
    encCW = true;
  } else {
    encCW =  false;
  }
  storeEncoderDir(encCW ? 1 : 0);
}

FLASHMEM void settingsPickupEnable(char * value) {
  if (strcmp(value, "Off") == 0) {
    pickUp = false;
  } else {
    pickUp =  true;
  }
  storePickupEnable(pickUp ? 1 : 0);
}

FLASHMEM void settingsBassEnhanceEnable(char * value) {
  if (strcmp(value, "Off") == 0) {
    sgtl5000_1.enhanceBassDisable();
    storeBassEnhanceEnable(0);
  } else {
    sgtl5000_1.enhanceBassEnable();
    storeBassEnhanceEnable(1);
  }
}

FLASHMEM void settingsScopeEnable(char * value) {
  if (strcmp(value, "Off") == 0) {
    enableScope(false);
    storeScopeEnable(0);
  } else {
    enableScope(true);
    storeScopeEnable(1);
  }
}

//Takes a pointer to a specific method for the settings option and invokes it.
FLASHMEM void settingsHandler(char * s, void (*f)(char*) ) {
  f(s);
}

FLASHMEM int currentIndexMIDICh() {
  return getMIDIChannel();
}

FLASHMEM int currentIndexVelocitySens() {
  return velocitySens;
}

FLASHMEM int currentIndexKeyTracking() {
  if (keytrackingAmount == 0.0f) return 0;
  if (keytrackingAmount == 0.5f)  return 1;
  if (keytrackingAmount == 1.0f) return 2;
  return 0;
}

FLASHMEM int currentIndexPitchBend() {
  return  getPitchBendRange() - 1;
}

FLASHMEM int currentIndexModWheelDepth() {
  return (getModWheelDepth() * 10) - 1;
}

FLASHMEM int currentIndexMIDIOutCh() {
  return getMIDIOutCh();
}

FLASHMEM int currentIndexEncoderDir() {
  return getEncoderDir() ? 0 : 1;
}

FLASHMEM int currentIndexPickupEnable() {
  return getPickupEnable() ? 1 : 0;
}

FLASHMEM int currentIndexBassEnhanceEnable() {
  return getBassEnhanceEnable() ? 1 : 0;
}

FLASHMEM int currentIndexScopeEnable() {
  return getScopeEnable() ? 1 : 0;
}

//Takes a pointer to a specific method for the current settings option value and invokes it.
FLASHMEM int getCurrentIndex(int (*f)() ) {
  return f();
}

const CircularBuffer<SettingsOption, SETTINGSOPTIONSNO>  settingsOptions;

// add settings to the circular buffer
FLASHMEM void setUpSettings() {
  settingsOptions.push(SettingsOption{"MIDI Ch.", {"All", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", '\0'}, settingsMIDICh, currentIndexMIDICh});
  settingsOptions.push(SettingsOption{"Key Tracking", {"None", "Half", "Full", '\0'}, settingsKeyTracking, currentIndexKeyTracking});
  settingsOptions.push(SettingsOption{"Vel. Sens.", {"Off", "1", "2", "3", "4", '\0'}, settingsVelocitySens, currentIndexVelocitySens});
  settingsOptions.push(SettingsOption{"Pitch Bend", {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", '\0'}, settingsPitchBend, currentIndexPitchBend});
  settingsOptions.push(SettingsOption{"MW Depth", {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", '\0'}, settingsModWheelDepth, currentIndexModWheelDepth});
  settingsOptions.push(SettingsOption{"MIDI Out Ch.", {"Off", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", '\0'}, settingsMIDIOutCh, currentIndexMIDIOutCh});
  settingsOptions.push(SettingsOption{"Encoder", {"Type 1", "Type 2", '\0'}, settingsEncoderDir, currentIndexEncoderDir});
  settingsOptions.push(SettingsOption{"Pick-up", {"Off", "On", '\0'}, settingsPickupEnable, currentIndexPickupEnable});
  settingsOptions.push(SettingsOption{"Bass Enh.", {"Off", "On", '\0'}, settingsBassEnhanceEnable, currentIndexBassEnhanceEnable});
  settingsOptions.push(SettingsOption{"Oscilloscope", {"Off", "On", '\0'}, settingsScopeEnable, currentIndexScopeEnable});
}
