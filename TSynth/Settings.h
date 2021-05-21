#define SETTINGSOPTIONSNO 16//No of options
#define SETTINGSVALUESNO 19//Maximum number of settings option values needed
#include "VoiceGroup.h"
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
void settingsMonophonic(const char * value);
void settingsAmpEnv(const char *value);
void settingsFiltEnv(const char *value);
void settingsGlideShape(const char *value);
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
int currentIndexMonophonicMode();
int currentIndexAmpEnv();
int currentIndexFiltEnv();
int currentIndexGlideShape();
int getCurrentIndex(int (*f)());

FLASHMEM int currentIndexGlideShape() {
  return glideShape;
}

FLASHMEM void settingsGlideShape(const char * value) {
  if (strcmp(value, "Lin") == 0) glideShape = 0;
  else if (strcmp(value, "Exp") == 0) glideShape = 1;
  else glideShape = 1;
  for (uint8_t i = 0; i < global.maxVoices(); i++) {
    global.Oscillators[i].glide_.setMode(glideShape);
  }
  storeGlideShape(glideShape); 
}

FLASHMEM int currentIndexAmpEnv() {
  if((envTypeAmp>=-8) && (envTypeAmp<=8))return envTypeAmp+9;
  else return 0;
}

FLASHMEM int currentIndexFiltEnv() {
  if((envTypeFilt>=-8) && (envTypeFilt<=8))return envTypeFilt+9;
  else return 0;
}

FLASHMEM void settingsAmpEnv(const char * value) {
  if (strcmp(value, "Lin") == 0) envTypeAmp = -128;
  else if (strcmp(value, "Exp -8") == 0)  envTypeAmp = -8;
  else if (strcmp(value, "Exp -7") == 0)  envTypeAmp = -7;
  else if (strcmp(value, "Exp -6") == 0)  envTypeAmp = -6;
  else if (strcmp(value, "Exp -5") == 0)  envTypeAmp = -5;
  else if (strcmp(value, "Exp -4") == 0)  envTypeAmp = -4;
  else if (strcmp(value, "Exp -3") == 0)  envTypeAmp = -3;
  else if (strcmp(value, "Exp -2") == 0)  envTypeAmp = -2;
  else if (strcmp(value, "Exp -1") == 0)  envTypeAmp = -1;
  else if (strcmp(value, "Exp 0") == 0)  envTypeAmp = 0;
  else if (strcmp(value, "Exp +1") == 0)  envTypeAmp = 1;
  else if (strcmp(value, "Exp +2") == 0)  envTypeAmp = 2;
  else if (strcmp(value, "Exp +3") == 0)  envTypeAmp = 3;
  else if (strcmp(value, "Exp +4") == 0)  envTypeAmp = 4;
  else if (strcmp(value, "Exp +5") == 0)  envTypeAmp = 5;
  else if (strcmp(value, "Exp +6") == 0)  envTypeAmp = 6;
  else if (strcmp(value, "Exp +7") == 0)  envTypeAmp = 7;
  else if (strcmp(value, "Exp +8") == 0)  envTypeAmp = 8;
  else envTypeAmp = -128;
  for (uint8_t i = 0; i < global.maxVoices(); i++) {
    global.Oscillators[i].ampEnvelope_.setEnvType(envTypeAmp);
  }
  storeAmpEnv(envTypeAmp);
}

FLASHMEM void settingsFiltEnv(const char * value) {
  if (strcmp(value, "Lin") == 0) envTypeFilt = -128;
  else if (strcmp(value, "Exp -8") == 0)  envTypeFilt = -8;
  else if (strcmp(value, "Exp -7") == 0)  envTypeFilt = -7;
  else if (strcmp(value, "Exp -6") == 0)  envTypeFilt = -6;
  else if (strcmp(value, "Exp -5") == 0)  envTypeFilt = -5;
  else if (strcmp(value, "Exp -4") == 0)  envTypeFilt = -4;
  else if (strcmp(value, "Exp -3") == 0)  envTypeFilt = -3;
  else if (strcmp(value, "Exp -2") == 0)  envTypeFilt = -2;
  else if (strcmp(value, "Exp -1") == 0)  envTypeFilt = -1;
  else if (strcmp(value, "Exp 0") == 0)  envTypeFilt = 0;
  else if (strcmp(value, "Exp +1") == 0)  envTypeFilt = 1;
  else if (strcmp(value, "Exp +2") == 0)  envTypeFilt = 2;
  else if (strcmp(value, "Exp +3") == 0)  envTypeFilt = 3;
  else if (strcmp(value, "Exp +4") == 0)  envTypeFilt = 4;
  else if (strcmp(value, "Exp +5") == 0)  envTypeFilt = 5;
  else if (strcmp(value, "Exp +6") == 0)  envTypeFilt = 6;
  else if (strcmp(value, "Exp +7") == 0)  envTypeFilt = 7;
  else if (strcmp(value, "Exp +8") == 0)  envTypeFilt = 8;
  else envTypeFilt = -128;
  for (uint8_t i = 0; i < global.maxVoices(); i++) {
    global.Oscillators[i].filterEnvelope_.setEnvType(envTypeFilt);
  }
  storeFiltEnv(envTypeFilt);
}						

FLASHMEM void reloadGlideShape(){
  glideShape = getGlideShape();
  for (uint8_t i = 0; i < global.maxVoices(); i++) {
    global.Oscillators[i].glide_.setMode(glideShape);
  }
}

FLASHMEM void reloadAmpEnv(){
  envTypeAmp = getAmpEnv();
  for (uint8_t i = 0; i < global.maxVoices(); i++) {
    global.Oscillators[i].ampEnvelope_.setEnvType(envTypeAmp);
  }
}

FLASHMEM void reloadFiltEnv(){
  envTypeFilt = getFiltEnv();
  for (uint8_t i = 0; i < global.maxVoices(); i++) {
    global.Oscillators[i].filterEnvelope_.setEnvType(envTypeFilt);
  }
}

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
  groupvec[activeGroupIndex]->setMonophonic(monophonic);
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
  if (groupvec[activeGroupIndex]->getKeytrackingAmount() == 0.0f) return 0;
  if (groupvec[activeGroupIndex]->getKeytrackingAmount() == 0.5f) return 1;
  if (groupvec[activeGroupIndex]->getKeytrackingAmount() == 1.0f) return 2;
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
  return groupvec[activeGroupIndex]->getMonophonicMode();
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
  settingsOptions.push(SettingsOption{"Monophonic", {"Off", "Last", "First", "Highest", "Lowest"/* , "Legato"*/, "\0"}, settingsMonophonic, currentIndexMonophonicMode});
  settingsOptions.push(SettingsOption{"Pitch Bend", {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "\0"}, settingsPitchBend, currentIndexPitchBend});
  settingsOptions.push(SettingsOption{"MW Depth", {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "\0"}, settingsModWheelDepth, currentIndexModWheelDepth});
  settingsOptions.push(SettingsOption{"MIDI Out Ch.", {"Off", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "\0"}, settingsMIDIOutCh, currentIndexMIDIOutCh});
  settingsOptions.push(SettingsOption{"MIDI Thru", {"Off", "Full", "Same Ch.", "Diff. Ch.", "\0"}, settingsMIDIThru, currentIndexMIDIThru});
  settingsOptions.push(SettingsOption{"Amp Env", {"Lin", "Exp -8", "Exp -7", "Exp -6", "Exp -5", "Exp -4", "Exp -3", "Exp -2", "Exp -1", "Exp 0", "Exp +1", "Exp +2", "Exp +3", "Exp +4", "Exp +5", "Exp +6", "Exp +7", "Exp +8", "\0"}, settingsAmpEnv, currentIndexAmpEnv});
  settingsOptions.push(SettingsOption{"Fil Env", {"Lin", "Exp -8", "Exp -7", "Exp -6", "Exp -5", "Exp -4", "Exp -3", "Exp -2", "Exp -1", "Exp 0", "Exp +1", "Exp +2", "Exp +3", "Exp +4", "Exp +5", "Exp +6", "Exp +7", "Exp +8", "\0"}, settingsFiltEnv, currentIndexFiltEnv});
  settingsOptions.push(SettingsOption{"Glide Shape", {"Lin", "Exp", "\0"}, settingsGlideShape, currentIndexGlideShape});
  settingsOptions.push(SettingsOption{"Pick-up", {"Off", "On", "\0"}, settingsPickupEnable, currentIndexPickupEnable});
  settingsOptions.push(SettingsOption{"Encoder", {"Type 1", "Type 2", "\0"}, settingsEncoderDir, currentIndexEncoderDir});
  settingsOptions.push(SettingsOption{"Oscilloscope", {"Off", "On", "\0"}, settingsScopeEnable, currentIndexScopeEnable});
  settingsOptions.push(SettingsOption{"VU Meter", {"Off", "On", "\0"}, settingsVUEnable, currentIndexVUEnable});
  settingsOptions.push(SettingsOption{"Bass Enh.", {"Off", "On", "\0"}, settingsBassEnhanceEnable, currentIndexBassEnhanceEnable});
}
