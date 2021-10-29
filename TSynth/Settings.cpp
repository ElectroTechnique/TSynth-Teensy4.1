#include "Settings.h"
#include "SettingsService.h"
#include "Parameters.h"
#include "globals.h"
#include "EepromMgr.h"
#include "Display.h"

// timbre settings page
settings::SettingsService timbreSettings;

// main settings page
settings::SettingsService mainSettings;

// Defined in TSynth.ino
void updateKeyTracking(float value);
void changeMIDIThruMode();

void settingsTimbreMIDICh(int index, const char * value);
int currentIndexTimbreMIDICh();

FLASHMEM void settingsTimbreMIDICh(int index, const char * value) {
  if (strcmp(value, "ALL") == 0) {
    midiChannel = MIDI_CHANNEL_OMNI;
  } else {
    midiChannel = atoi(value);
  }
  //storeMidiChannel(midiChannel);
}

// Forward declare everything
void settingsMIDICh(int index, const char * value);
void settingsVelocitySens(int index, const char * value);
void settingsKeyTracking(int index, const char * value);
void settingsPitchBend(int index, const char * value);
void settingsModWheelDepth(int index, const char * value);
void settingsMIDIOutCh(int index, const char * value);
void settingsMIDIThru(int index, const char * value);
void settingsEncoderDir(int index, const char * value);
void settingsPickupEnable(int index, const char * value);
void settingsBassEnhanceEnable(int index, const char * value);
void settingsScopeEnable(int index, const char * value);
void settingsVUEnable(int index, const char * value);
void settingsMonophonic(int index, const char * value);
void settingsAmpEnv(int index, const char *value);
void settingsFiltEnv(int index, const char *value);
void settingsGlideShape(int index, const char *value);

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

// MT settings
void mtSettingsMIDICh1(int index, const char * value);
void mtSettingsMIDICh2(int index, const char * value);
void mtSettingsMIDICh3(int index, const char * value);
void mtSettingsVoices(int index, const char * value);
int mtCurrentIndexMIDICh1();
int mtCurrentIndexMIDICh2();
int mtCurrentIndexMIDICh3();
int mtCurrentIndexVoices();


// Add options to program memory
#define SINGLE_ARG(...) __VA_ARGS__
#define ARRAY_OPTION_GLOBAL(PREFIX, NAME, VALUES, HANDLER, INDEX)                              \
  const prog_char* PREFIX##Values[] PROGMEM = VALUES;                                          \
  settings::ArrayOption PREFIX##Option(NAME, (const char**)(&PREFIX##Values), HANDLER, INDEX);

ARRAY_OPTION_GLOBAL(option_midiIn, "MIDI In Ch.", {SINGLE_ARG("All", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "\0")}, settingsMIDICh, currentIndexMIDICh);
ARRAY_OPTION_GLOBAL(option_keyTracking, "Key Tracking", {SINGLE_ARG("None", "Half", "Full", "\0")}, settingsMIDICh, currentIndexKeyTracking);
ARRAY_OPTION_GLOBAL(option_mono, "Monophonic", {SINGLE_ARG("Off", "Last", "First", "Highest", "Lowest"/* , "Legato"*/, "\0")}, settingsMonophonic, currentIndexMonophonicMode);
ARRAY_OPTION_GLOBAL(option_pitchBend, "Pitch Bend", {SINGLE_ARG("1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "\0")}, settingsPitchBend, currentIndexPitchBend);
ARRAY_OPTION_GLOBAL(option_mwDepth, "MW Depth", {SINGLE_ARG("1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "\0")}, settingsModWheelDepth, currentIndexModWheelDepth);
ARRAY_OPTION_GLOBAL(option_midiOut, "MIDI Out Ch.", {SINGLE_ARG("Off", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "\0")}, settingsMIDIOutCh, currentIndexMIDIOutCh);
ARRAY_OPTION_GLOBAL(option_midiThru, "MIDI Thru", {SINGLE_ARG("Off", "Full", "Same Ch.", "Diff. Ch.", "\0")}, settingsMIDIThru, currentIndexMIDIThru);
ARRAY_OPTION_GLOBAL(option_ampEnv, "Amp. Env.", {SINGLE_ARG("Lin", "Exp -8", "Exp -7", "Exp -6", "Exp -5", "Exp -4", "Exp -3", "Exp -2", "Exp -1", "Exp 0", "Exp +1", "Exp +2", "Exp +3", "Exp +4", "Exp +5", "Exp +6", "Exp +7", "Exp +8", "\0")}, settingsAmpEnv, currentIndexAmpEnv);
ARRAY_OPTION_GLOBAL(option_filterEnv, "Filter Env.", {SINGLE_ARG("Lin", "Exp -8", "Exp -7", "Exp -6", "Exp -5", "Exp -4", "Exp -3", "Exp -2", "Exp -1", "Exp 0", "Exp +1", "Exp +2", "Exp +3", "Exp +4", "Exp +5", "Exp +6", "Exp +7", "Exp +8", "\0")}, settingsFiltEnv, currentIndexFiltEnv);
ARRAY_OPTION_GLOBAL(option_glideShape, "Glide Shape", {SINGLE_ARG("Lin", "Exp", "\0")}, settingsGlideShape, currentIndexGlideShape);
ARRAY_OPTION_GLOBAL(option_pickUp, "Pick-up", {SINGLE_ARG("Off", "On", "\0")}, settingsPickupEnable, currentIndexPickupEnable);
ARRAY_OPTION_GLOBAL(option_encoder, "Encoder", {SINGLE_ARG("Type 1", "Type 2", "\0")}, settingsEncoderDir, currentIndexEncoderDir);
ARRAY_OPTION_GLOBAL(option_oscilloscope, "Oscilloscope", {SINGLE_ARG("Off", "On", "\0")}, settingsScopeEnable, currentIndexScopeEnable);
ARRAY_OPTION_GLOBAL(option_meter, "VU Meter", {SINGLE_ARG("Off", "On", "\0")}, settingsVUEnable, currentIndexVUEnable);
ARRAY_OPTION_GLOBAL(option_bassEnh, "Bass Enh.", {SINGLE_ARG("Off", "On", "\0")}, settingsBassEnhanceEnable, currentIndexBassEnhanceEnable);

ARRAY_OPTION_GLOBAL(option_mt_midiCh1, "MIDI Ch. 1", {SINGLE_ARG("All", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "\0")}, mtSettingsMIDICh1, mtCurrentIndexMIDICh1);
ARRAY_OPTION_GLOBAL(option_mt_midiCh2, "MIDI Ch. 2", {SINGLE_ARG("All", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "\0")}, mtSettingsMIDICh2, mtCurrentIndexMIDICh2);
ARRAY_OPTION_GLOBAL(option_mt_midiCh3, "MIDI Ch. 3", {SINGLE_ARG("All", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "\0")}, mtSettingsMIDICh3, mtCurrentIndexMIDICh3);
ARRAY_OPTION_GLOBAL(option_mt_voices, "Voices", {SINGLE_ARG("1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "\0")}, mtSettingsVoices, mtCurrentIndexVoices);

// Initialize settings object.
FLASHMEM void setUpSettings() {
  mainSettings.append(option_midiInOption);
  mainSettings.append(option_keyTrackingOption);
  mainSettings.append(option_monoOption);
  mainSettings.append(option_pitchBendOption);
  mainSettings.append(option_mwDepthOption);
  mainSettings.append(option_midiOutOption);
  mainSettings.append(option_midiThruOption);
  mainSettings.append(option_ampEnvOption);
  mainSettings.append(option_filterEnvOption);
  mainSettings.append(option_glideShapeOption);
  mainSettings.append(option_pickUpOption);
  mainSettings.append(option_encoderOption);
  mainSettings.append(option_oscilloscopeOption);
  mainSettings.append(option_meterOption);
  mainSettings.append(option_bassEnhOption);

  //timbreSettings.append(settings::SettingsOption{"MIDI In Ch.", {"All", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "\0"}, timbreMIDICh, currenttimbreIDICh});
  timbreSettings.append(option_mt_midiCh1Option);
  timbreSettings.append(option_mt_midiCh2Option);
  timbreSettings.append(option_mt_midiCh3Option);
  timbreSettings.append(option_mt_voicesOption);
}

FLASHMEM int currentIndexGlideShape() {
  return glideShape;
}

FLASHMEM void settingsGlideShape(int index, const char * value) {
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

FLASHMEM void settingsAmpEnv(int index, const char * value) {
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

FLASHMEM void settingsFiltEnv(int index, const char * value) {
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

FLASHMEM void settingsMIDICh(int index, const char * value) {
  if (strcmp(value, "ALL") == 0) {
    midiChannel = MIDI_CHANNEL_OMNI;
  } else {
    midiChannel = atoi(value);
  }
  storeMidiChannel(midiChannel);
}

FLASHMEM void settingsVelocitySens(int index, const char * value) {
  if (strcmp(value, "Off") == 0) {
    velocitySens = 0;
  } else {
    velocitySens = atoi(value);
  }
}

FLASHMEM void settingsKeyTracking(int index, const char * value) {
  if (strcmp(value, "None") == 0) updateKeyTracking(0);
  if (strcmp(value, "Half") == 0) updateKeyTracking(0.5);
  if (strcmp(value, "Full") == 0) updateKeyTracking(1.0);
}

FLASHMEM void settingsPitchBend(int index, const char * value) {
  pitchBendRange = atoi(value);
  storePitchBendRange(pitchBendRange);
}

FLASHMEM void settingsModWheelDepth(int index, const char * value) {
  modWheelDepth = atoi(value) / 10.0f;
  storeModWheelDepth(modWheelDepth);
}

FLASHMEM void settingsMIDIOutCh(int index, const char * value) {
  if (strcmp(value, "Off") == 0) {
    midiOutCh = 0;
  } else {
    midiOutCh = atoi(value);
  }
  storeMidiOutCh(midiOutCh);
}

FLASHMEM void settingsMIDIThru(int index, const char * value) {
  if (strcmp(value, "Off") == 0) MIDIThru = midi::Thru::Off;
  if (strcmp(value, "Full") == 0)  MIDIThru =  midi::Thru::Full;
  if (strcmp(value, "Same Ch.") == 0) MIDIThru =  midi::Thru::SameChannel;
  if (strcmp(value, "Diff. Ch.") == 0) MIDIThru =  midi::Thru::DifferentChannel;
  changeMIDIThruMode();
  storeMidiThru(MIDIThru);
}

FLASHMEM void settingsEncoderDir(int index, const char * value) {
  if (strcmp(value, "Type 1") == 0) {
    encCW = true;
  } else {
    encCW =  false;
  }
  storeEncoderDir(encCW ? 1 : 0);
}

FLASHMEM void settingsPickupEnable(int index, const char * value) {
  if (strcmp(value, "Off") == 0) {
    pickUp = false;
  } else {
    pickUp =  true;
  }
  storePickupEnable(pickUp ? 1 : 0);
}

FLASHMEM void settingsBassEnhanceEnable(int index, const char * value) {
  if (strcmp(value, "Off") == 0) {
    global.sgtl5000_1.enhanceBassDisable();
    storeBassEnhanceEnable(0);
  } else {
    global.sgtl5000_1.enhanceBassEnable();
    storeBassEnhanceEnable(1);
  }
}

FLASHMEM void settingsMonophonic(int index, const char * value) {
  uint8_t monophonic;
  if (strcmp(value, "Off") == 0)     monophonic = MONOPHONIC_OFF;
  if (strcmp(value, "Last") == 0)    monophonic = MONOPHONIC_LAST;
  if (strcmp(value, "First") == 0)   monophonic = MONOPHONIC_FIRST;
  if (strcmp(value, "Highest") == 0) monophonic = MONOPHONIC_HIGHEST;
  if (strcmp(value, "Lowest") == 0)  monophonic = MONOPHONIC_LOWEST;
  if (strcmp(value, "Legato") == 0)  monophonic = MONOPHONIC_LEGATO;
  groupvec[activeGroupIndex]->setMonophonic(monophonic);
}

FLASHMEM void settingsScopeEnable(int index, const char * value) {
  if (strcmp(value, "Off") == 0) {
    enableScope(false);
    storeScopeEnable(0);
  } else {
    enableScope(true);
    storeScopeEnable(1);
  }
}

FLASHMEM void settingsVUEnable(int index, const char * value) {
  if (strcmp(value, "Off") == 0) {
    vuMeter = false;
    storeVUEnable(0);
  } else {
    vuMeter = true;
    storeVUEnable(1);
  }
}

FLASHMEM void mtSettingsMIDICh1(int index, const char * value) {
  if (strcmp(value, "ALL") == 0) {
    mtMidiChannel1 = MIDI_CHANNEL_OMNI;
  } else {
    mtMidiChannel1 = atoi(value);
  }
  // TODO: Store timbre config.
}

FLASHMEM void mtSettingsMIDICh2(int index, const char * value) {
  if (strcmp(value, "ALL") == 0) {
    mtMidiChannel2 = MIDI_CHANNEL_OMNI;
  } else {
    mtMidiChannel2 = atoi(value);
  }
  // TODO: Store timbre config.
}

FLASHMEM void mtSettingsMIDICh3(int index, const char * value) {
  if (strcmp(value, "ALL") == 0) {
    mtMidiChannel3 = MIDI_CHANNEL_OMNI;
  } else {
    mtMidiChannel3 = atoi(value);
  }
  // TODO: Store timbre config.
  //timbres.first()
}

FLASHMEM void mtSettingsVoices(int index, const char * value) {
  mtNumVoices = atoi(value);
  // TODO: Store timbre config.
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

FLASHMEM int mtCurrentIndexMIDICh1() {
  return mtMidiChannel1;
}

FLASHMEM int mtCurrentIndexMIDICh2() {
  return mtMidiChannel2;
}

FLASHMEM int mtCurrentIndexMIDICh3() {
  return mtMidiChannel3;
}

FLASHMEM int mtCurrentIndexVoices() {
  return mtNumVoices - 1;
}