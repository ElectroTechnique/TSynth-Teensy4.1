#include <Arduino.h>
#include <EEPROM.h>
#include <MIDI.h>

#define EEPROM_MIDI_CH 0
#define EEPROM_PITCHBEND 1
#define EEPROM_MODWHEEL_DEPTH 2
#define EEPROM_ENCODER_DIR 3
#define EEPROM_PICKUP_ENABLE 4
#define EEPROM_BASSENHANCE_ENABLE 5
#define EEPROM_SCOPE_ENABLE 6
#define EEPROM_MIDI_OUT_CH 7
#define EEPROM_VU_ENABLE 8
#define EEPROM_MIDI_THRU 9
#define EEPROM_AMP_ENV 10
#define EEPROM_FILT_ENV 11
#define EEPROM_GLIDE_SHAPE 12

void storeGlideShape(byte type);
void storeAmpEnv(byte type);
void storeFiltEnv(byte type);
int8_t getGlideShape();
int8_t getAmpEnv();
int8_t getFiltEnv();
int getMIDIChannel();
void storeMidiChannel(byte channel);
int getPitchBendRange();
void storePitchBendRange(byte pitchbend);
float getModWheelDepth();
void storeModWheelDepth(float mwDepth);
int getMIDIOutCh();
void storeMidiOutCh(byte channel);
midi::Thru::Mode getMidiThru();
void storeMidiThru(byte thru);
boolean getEncoderDir();
void storeEncoderDir(byte encoderDir);
boolean getPickupEnable();
void storePickupEnable(byte pickupEnable);
boolean getBassEnhanceEnable();
void storeBassEnhanceEnable(byte bassEnhanceEnable);
boolean getScopeEnable();
void storeScopeEnable(byte ScopeEnable);
boolean getVUEnable();
void storeVUEnable(byte VUEnable);
