#include <EEPROM.h>

#define EEPROM_MIDI_CH 0
#define EEPROM_PITCHBEND 1
#define EEPROM_MODWHEEL_DEPTH 2
#define EEPROM_ENCODER_DIR 3
#define EEPROM_PICKUP_ENABLE 4
#define EEPROM_BASSENHANCE_ENABLE 5
#define EEPROM_SCOPE_ENABLE 6
#define EEPROM_MIDI_OUT_CH 7
#define EEPROM_VU_ENABLE 8

FLASHMEM int getMIDIChannel() {
  byte midiChannel = EEPROM.read(EEPROM_MIDI_CH);
  if (midiChannel < 0 || midiChannel > 16) midiChannel = MIDI_CHANNEL_OMNI;//If EEPROM has no MIDI channel stored
  return midiChannel;
}

FLASHMEM void storeMidiChannel(byte channel){
  EEPROM.update(EEPROM_MIDI_CH, channel);
}

FLASHMEM int getPitchBendRange() {
  byte pitchbend = EEPROM.read(EEPROM_PITCHBEND);
  if (pitchbend < 1 || pitchbend > 12) return pitchBendRange; //If EEPROM has no pitchbend stored
  return pitchbend;
}

FLASHMEM void storePitchBendRange(byte pitchbend){
  EEPROM.update(EEPROM_PITCHBEND, pitchbend);
}

FLASHMEM float getModWheelDepth() {
  byte mw = EEPROM.read(EEPROM_MODWHEEL_DEPTH);
  if (mw < 1 || mw > 10) return modWheelDepth; //If EEPROM has no mod wheel depth stored
  return mw / 10.0f;
}

FLASHMEM void storeModWheelDepth(float mwDepth){
  byte mw =  mwDepth * 10;
  EEPROM.update(EEPROM_MODWHEEL_DEPTH, mw);
}

FLASHMEM int getMIDIOutCh() {
  byte mc = EEPROM.read(EEPROM_MIDI_OUT_CH);
  if (mc < 0 || midiOutCh > 16) mc = 0;//If EEPROM has no MIDI channel stored
  return mc;
}

FLASHMEM void storeMidiOutCh(byte channel){
  EEPROM.update(EEPROM_MIDI_OUT_CH, channel);
}

FLASHMEM boolean getEncoderDir() {
  byte ed = EEPROM.read(EEPROM_ENCODER_DIR); 
  if (ed < 0 || ed > 1)return true; //If EEPROM has no encoder direction stored
  return ed == 1 ? true : false;
}

FLASHMEM void storeEncoderDir(byte encoderDir){
  EEPROM.update(EEPROM_ENCODER_DIR, encoderDir);
}

FLASHMEM boolean getPickupEnable() {
  byte pu = EEPROM.read(EEPROM_PICKUP_ENABLE); 
  if (pu < 0 || pu > 1)return false; //If EEPROM has no pickup enable stored
  return pu == 1 ? true : false;
}

FLASHMEM void storePickupEnable(byte pickupEnable){
  EEPROM.update(EEPROM_PICKUP_ENABLE, pickupEnable);
}

FLASHMEM boolean getBassEnhanceEnable() {
  byte eh = EEPROM.read(EEPROM_BASSENHANCE_ENABLE); 
  if (eh < 0 || eh > 1)return false; //If EEPROM has no bass enhance enable stored
  return eh == 1 ? true : false;
}

FLASHMEM void storeBassEnhanceEnable(byte bassEnhanceEnable){
  EEPROM.update(EEPROM_BASSENHANCE_ENABLE, bassEnhanceEnable);
}

FLASHMEM boolean getScopeEnable() {
  byte sc = EEPROM.read(EEPROM_SCOPE_ENABLE); 
  if (sc < 0 || sc > 1)return false; //If EEPROM has no scope enable stored
  return sc == 1 ? true : false;
}

FLASHMEM void storeScopeEnable(byte ScopeEnable){
  EEPROM.update(EEPROM_SCOPE_ENABLE, ScopeEnable);
}

FLASHMEM boolean getVUEnable() {
  byte vu = EEPROM.read(EEPROM_VU_ENABLE); 
  if (vu < 0 || vu > 1)return false; //If EEPROM has no VU enable stored
  return vu == 1 ? true : false;
}

FLASHMEM void storeVUEnable(byte VUEnable){
  EEPROM.update(EEPROM_VU_ENABLE, VUEnable);
}
