#define SETTINGSOPTIONSNO 5
#define SETTINGSVALUESNO 18//Maximum number of settings option values needed
int settingsValueIndex = 0;//currently selected settings option value index

struct SettingsOption
{
  char * option;//Settings option string
  char *value[SETTINGSVALUESNO];//Array of strings of settings option values
  int  handler;//Function to handle the values for this settings option
  int  currentIndex;//Function to array index of current value for this settings option
};

void settingsMIDICh(char * value);
void settingsKeyTracking(char * value);
void settingsPitchBend(char * value);
void settingsModWheelDepth(char * value);
void settingsEncoderDir(char * value);
void settingsHandler(char * s, void (*f)(char*));

int currentIndexMIDICh();
int currentIndexKeyTracking();
int currentIndexPitchBend();
int currentIndexModWheelDepth();
int currentIndexEncoderDir();
int getCurrentIndex(int (*f)());


void settingsMIDICh(char * value) {
  if (strcmp(value, "ALL") == 0) {
    midiChannel = MIDI_CHANNEL_OMNI;
  } else {
    midiChannel = atoi(value);
  }
  storeMidiChannel(midiChannel);
}
void settingsKeyTracking(char * value) {
  if (strcmp(value, "None") == 0) keytrackingAmount = 0;
  if (strcmp(value, "Half") == 0)  keytrackingAmount =  0.5;
  if (strcmp(value, "Full") == 0) keytrackingAmount =  1.0;
  storeKeyTracking(keytrackingAmount);
}

void settingsPitchBend(char * value) {
  pitchBendRange = atoi(value);
  storePitchBendRange(pitchBendRange);
}

void settingsModWheelDepth(char * value) {
  modWheelDepth = atoi(value) / 10.0f;
  storeModWheelDepth(modWheelDepth);
}

void settingsEncoderDir(char * value) {
  if (strcmp(value, "Type 1") == 0) {
    encCW = true;
  } else {
    encCW =  false;
  }
  storeEncoderDir(encCW ? 1 : 0);
}

//Takes a pointer to a specific method for the settings option and invokes it.
void settingsHandler(char * s, void (*f)(char*) ) {
  f(s);
}

int currentIndexMIDICh() {
  return getMIDIChannel();
}

int currentIndexKeyTracking() {
  float value = getKeyTracking();
  if (value == 0) return 0;
  if (value == 0.5)  return 1;
  if (value == 1.0) return 2;
  return 0;
}

int currentIndexPitchBend() {
  return  getPitchBendRange() - 1;
}

int currentIndexModWheelDepth() {
  return (getModWheelDepth() * 10) - 1;
}

int currentIndexEncoderDir() {
  return getEncoderDir() ? 0 : 1;
}

//Takes a pointer to a specific method for the current settings option value and invokes it.
int getCurrentIndex(int (*f)() ) {
  return f();
}

CircularBuffer<SettingsOption, SETTINGSOPTIONSNO>  settingsOptions;

// add settings to the circular buffer
void setUpSettings() {
  settingsOptions.push(SettingsOption{"MIDI Ch.", {"All", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", '\0'}, settingsMIDICh, currentIndexMIDICh});
  settingsOptions.push(SettingsOption{"Key Tracking", {"None", "Half", "Full", '\0'}, settingsKeyTracking, currentIndexKeyTracking});
  settingsOptions.push(SettingsOption{"Pitch Bend", {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", '\0'}, settingsPitchBend, currentIndexPitchBend});
  settingsOptions.push(SettingsOption{"MW Depth", {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", '\0'}, settingsModWheelDepth, currentIndexModWheelDepth});
  settingsOptions.push(SettingsOption{"Encoder", {"Type 1", "Type 2", '\0'}, settingsEncoderDir, currentIndexEncoderDir});
}
