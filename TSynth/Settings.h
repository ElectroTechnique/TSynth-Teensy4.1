#define SETTINGSOPTIONSNO 12//No of options
#define SETTINGSVALUESNO 18//Maximum number of settings option values needed
uint32_t settingsValueIndex = 0;//currently selected settings option value index

struct SettingsOption
{
  const char * option;//Settings option string
  const char *value[SETTINGSVALUESNO];//Array of strings of settings option values
  void (*handler)(const char*);//Function to handle the values for this settings option
  int  (*currentIndex)();//Function to array index of current value for this settings option
};

void settingsMIDICh(const char * value);
void settingsVelocitySens(const char * value);
void settingsKeyTracking(const char * value);
void settingsPitchBend(const char * value);
void settingsModWheelDepth(const char * value);
void settingsMIDIOutCh(const char * value);
void settingsMIDIThru(const char * value);
void settingsEncoderDir(const char * value);
void settingsPickupEnable(const char * value);
void settingsBassEnhanceEnable(const char * value);
void settingsScopeEnable(const char * value);
void settingsVUEnable(const char * value);
void settingsHandler(const char * s, void (*f)(const char*));

int currentIndexMIDICh();
int currentIndexVelocitySens();
int currentIndexKeyTracking();
int currentIndexPitchBend();
int currentIndexModWheelDepth();
int currentIndexMIDIOutCh();
int currentIndexMIDIThru();
int currentIndexEncoderDir();
int currentIndexPickupEnable();
int currentIndexBassEnhanceEnable();
int currentIndexScopeEnable();
int currentIndexVUEnable();
int getCurrentIndex(int (*f)());


FLASHMEM void settingsMIDICh(const char * value) {
  if (strcmp(value, "ALL") == 0) {
    midiChannel = MIDI_CHANNEL_OMNI;
  } else {
    midiChannel = atoi(value);
  }
  storeMidiChannel(midiChannel);
}

FLASHMEM void settingsVelocitySens(const char * value) {
  if (strcmp(value, "Off") == 0) {
    velocitySens = 0;
  } else {
    velocitySens = atoi(value);
  }
}

FLASHMEM void settingsKeyTracking(const char * value) {
  if (strcmp(value, "None") == 0) updateKeyTracking(0);
  if (strcmp(value, "Half") == 0) updateKeyTracking(0.5);
  if (strcmp(value, "Full") == 0) updateKeyTracking(1.0);
}

FLASHMEM void settingsPitchBend(const char * value) {
  pitchBendRange = atoi(value);
  storePitchBendRange(pitchBendRange);
}

FLASHMEM void settingsModWheelDepth(const char * value) {
  modWheelDepth = atoi(value) / 10.0f;
  storeModWheelDepth(modWheelDepth);
}

FLASHMEM void settingsMIDIOutCh(const char * value) {
  if (strcmp(value, "Off") == 0) {
    midiOutCh = 0;
  } else {
    midiOutCh = atoi(value);
  }
  storeMidiOutCh(midiOutCh);
}

FLASHMEM void settingsMIDIThru(const char * value) {
  if (strcmp(value, "Off") == 0) MIDIThru = midi::Thru::Off;
  if (strcmp(value, "Full") == 0)  MIDIThru =  midi::Thru::Full;
  if (strcmp(value, "Same Ch.") == 0) MIDIThru =  midi::Thru::SameChannel;
  if (strcmp(value, "Diff. Ch.") == 0) MIDIThru =  midi::Thru::DifferentChannel;
  changeMIDIThruMode();
  storeMidiThru(MIDIThru);
}

FLASHMEM void settingsEncoderDir(const char * value) {
  if (strcmp(value, "Type 1") == 0) {
    encCW = true;
  } else {
    encCW =  false;
  }
  storeEncoderDir(encCW ? 1 : 0);
}

FLASHMEM void settingsPickupEnable(const char * value) {
  if (strcmp(value, "Off") == 0) {
    pickUp = false;
  } else {
    pickUp =  true;
  }
  storePickupEnable(pickUp ? 1 : 0);
}

FLASHMEM void settingsBassEnhanceEnable(const char * value) {
  if (strcmp(value, "Off") == 0) {
    global.sgtl5000_1.enhanceBassDisable();
    storeBassEnhanceEnable(0);
  } else {
    global.sgtl5000_1.enhanceBassEnable();
    storeBassEnhanceEnable(1);
  }
}

FLASHMEM void settingsMonophonic(const char * value) {
  uint8_t monophonic;
  if (strcmp(value, "Off") == 0)     monophonic = MONOPHONIC_OFF;
  if (strcmp(value, "Last") == 0)    monophonic = MONOPHONIC_LAST;
  if (strcmp(value, "First") == 0)   monophonic = MONOPHONIC_FIRST;
  if (strcmp(value, "Highest") == 0) monophonic = MONOPHONIC_HIGHEST;
  if (strcmp(value, "Lowest") == 0)  monophonic = MONOPHONIC_LOWEST;
  if (strcmp(value, "Legato") == 0)  monophonic = MONOPHONIC_LEGATO;
  voices.setMonophonic(monophonic);
}

FLASHMEM void settingsScopeEnable(const char * value) {
  if (strcmp(value, "Off") == 0) {
    enableScope(false);
    storeScopeEnable(0);
  } else {
    enableScope(true);
    storeScopeEnable(1);
  }
}

FLASHMEM void settingsVUEnable(const char * value) {
  if (strcmp(value, "Off") == 0) {
    vuMeter = false;
    storeVUEnable(0);
  } else {
    vuMeter = true;
    storeVUEnable(1);
  }
}

//Takes a pointer to a specific method for the settings option and invokes it.
FLASHMEM void settingsHandler(const char * s, void (*f)(const char*) ) {
  f(s);
}

FLASHMEM int currentIndexMIDICh() {
  return getMIDIChannel();
}

FLASHMEM int currentIndexVelocitySens() {
  return velocitySens;
}

FLASHMEM int currentIndexKeyTracking() {
  if (voices.getKeytrackingAmount() == 0.0f) return 0;
  if (voices.getKeytrackingAmount() == 0.5f) return 1;
  if (voices.getKeytrackingAmount() == 1.0f) return 2;
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

FLASHMEM int currentIndexMIDIThru() {
  if (MIDIThru == midi::Thru::Off) return 0;
  if (MIDIThru == midi::Thru::Full)  return 1;
  if (MIDIThru == midi::Thru::SameChannel) return 2;
  if (MIDIThru == midi::Thru::DifferentChannel) return 3;
  return 0;
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

FLASHMEM int currentIndexMonophonicMode() {
  return voices.getMonophonicMode();
}

FLASHMEM int currentIndexScopeEnable() {
  return getScopeEnable() ? 1 : 0;
}

FLASHMEM int currentIndexVUEnable() {
  return getVUEnable() ? 1 : 0;
}

//Takes a pointer to a specific method for the current settings option value and invokes it.
FLASHMEM int getCurrentIndex(int (*f)() ) {
  return f();
}

CircularBuffer<SettingsOption, SETTINGSOPTIONSNO>  settingsOptions;

// add settings to the circular buffer
FLASHMEM void setUpSettings() {
  settingsOptions.push(SettingsOption{"MIDI Ch.", {"All", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "\0"}, settingsMIDICh, currentIndexMIDICh});
  settingsOptions.push(SettingsOption{"Key Tracking", {"None", "Half", "Full", "\0"}, settingsKeyTracking, currentIndexKeyTracking});
  settingsOptions.push(SettingsOption{"Vel. Sens.", {"Off", "1", "2", "3", "4", "\0"}, settingsVelocitySens, currentIndexVelocitySens});
  settingsOptions.push(SettingsOption{"Pitch Bend", {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "\0"}, settingsPitchBend, currentIndexPitchBend});
  settingsOptions.push(SettingsOption{"MW Depth", {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "\0"}, settingsModWheelDepth, currentIndexModWheelDepth});
  settingsOptions.push(SettingsOption{"MIDI Out Ch.", {"Off", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "\0"}, settingsMIDIOutCh, currentIndexMIDIOutCh});
  settingsOptions.push(SettingsOption{"MIDI Thru", {"Off", "Full", "Same Ch.", "Diff. Ch.", "\0"}, settingsMIDIThru, currentIndexMIDIThru});
  settingsOptions.push(SettingsOption{"Encoder", {"Type 1", "Type 2", "\0"}, settingsEncoderDir, currentIndexEncoderDir});
  settingsOptions.push(SettingsOption{"Pick-up", {"Off", "On", "\0"}, settingsPickupEnable, currentIndexPickupEnable});
  settingsOptions.push(SettingsOption{"Bass Enh.", {"Off", "On", "\0"}, settingsBassEnhanceEnable, currentIndexBassEnhanceEnable});
  settingsOptions.push(SettingsOption{"Monophonic", {"Off", "Last", "First", "Highest", "Lowest"/* , "Legato"*/, "\0"}, settingsMonophonic, currentIndexMonophonicMode});
  settingsOptions.push(SettingsOption{"Oscilloscope", {"Off", "On", "\0"}, settingsScopeEnable, currentIndexScopeEnable});
  settingsOptions.push(SettingsOption{"VU Meter", {"Off", "On", "\0"}, settingsVUEnable, currentIndexVUEnable});
}
