/*
   MIT License

  Copyright (c) 2020-21 ElectroTechnique

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

  ElectroTechnique TSynth - Firmware Rev 2.20
  TEENSY 4.1 - 12 VOICES

  Arduino IDE Tools Settings:
    Board: "Teensy4.1"
    USB Type: "Serial + MIDI + Audio"
    CPU Speed: "600MHz"
    Optimize: "Faster"

  Performance Tests   Max CPU  Mem
  600MHz Faster        60+     96

  Includes code by:
    Dave Benn - Handling MUXs, a few other bits and original inspiration  https://www.notesandvolts.com/2019/01/teensy-synth-part-10-hardware.html
    Alexander Davis / Vince R. Pearson - Stereo ensemble chorus effect https://github.com/quarterturn/teensy3-ensemble-chorus
    Will Winder - Major refactoring and monophonic mode
    Vince Pearson - Exponential envelopes
    Github member fab672000 - General improvements to code
    Mark Tillotson - Special thanks for band-limiting the waveforms in the Audio Library

  Additional libraries:
    Agileware CircularBuffer, Adafruit_GFX (available in Arduino libraries manager)
*/
#include "Audio.h" //Using local version to override Teensyduino version
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <MIDI.h>
#include <USBHost_t36.h>
#include <TeensyThreads.h>
#include "MidiCC.h"
#include "AudioPatching.h"
#include "Constants.h"
#include "Parameters.h"
#include "PatchMgr.h"
#include "HWControls.h"
#include "EepromMgr.h"
#include "Detune.h"
#include "utils.h"
#include "Velocity.h"
#include "Voice.h"
#include "VoiceGroup.h"
// This should be included here, but it introduces a circular dependency.
// #include "ST7735Display.h"

#define PARAMETER 0 //The main page for displaying the current patch and control (parameter) changes
#define RECALL 1 //Patches list
#define SAVE 2 //Save patch page
#define REINITIALISE 3 // Reinitialise message
#define PATCH 4 // Show current patch bypassing PARAMETER
#define PATCHNAMING 5 // Patch naming page
#define DELETE 6 //Delete patch page
#define DELETEMSG 7 //Delete patch message page
#define SETTINGS 8 //Settings page
#define SETTINGSVALUE 9 //Settings page

uint32_t state = PARAMETER;

VoiceGroup voices{SharedAudio[0]};

#include "ST7735Display.h"

//USB HOST MIDI Class Compliant
USBHost myusb;
USBHub hub1(myusb);
USBHub hub2(myusb);
MIDIDevice midi1(myusb);
//MIDIDevice_BigBuffer midi1(myusb); // Try this if your MIDI Compliant controller has problems

//MIDI 5 Pin DIN
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

void changeMIDIThruMode() {
  MIDI.turnThruOn(MIDIThru);
}

#include "Settings.h"

boolean cardStatus = false;
boolean firstPatchLoaded = false;

float previousMillis = millis(); //For MIDI Clk Sync

uint32_t count = 0;//For MIDI Clk Sync
uint32_t patchNo = 1;//Current patch no
int voiceToReturn = -1; //Initialise
long earliestTime = millis(); //For voice allocation - initialise to now

FLASHMEM void setup() {
  for (uint8_t i = 0; i < NO_OF_VOICES; i++) {
    voices.add(new Voice(Oscillators[i], i));
  }

  setupDisplay();
  setUpSettings();
  setupHardware();

  AudioMemory(97);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5 * SGTL_MAXVOLUME);
  sgtl5000_1.dacVolumeRamp();
  sgtl5000_1.muteHeadphone();
  sgtl5000_1.muteLineout();
  sgtl5000_1.audioPostProcessorEnable();
  sgtl5000_1.enhanceBass(0.85, 0.87, 0, 4);//Normal level, bass level, HPF bypass (1 - on), bass cutoff freq
  sgtl5000_1.enhanceBassDisable();//Turned on from EEPROM

  cardStatus = SD.begin(BUILTIN_SDCARD);
  if (cardStatus) {
    Serial.println(F("SD card is connected"));
    //Get patch numbers and names from SD card
    loadPatches();
    if (patches.size() == 0) {
      //save an initialised patch to SD card
      savePatch("1", INITPATCH);
      loadPatches();
    }
  }
  else {
    Serial.println(F("SD card is not connected or unusable"));
    reinitialiseToPanel();
    showPatchPage("No SD", "conn'd / usable");
  }

  //Read MIDI Channel from EEPROM
  midiChannel = getMIDIChannel();
  Serial.println("MIDI Ch:" + String(midiChannel) + " (0 is Omni On)");

  //USB HOST MIDI Class Compliant
  delay(200); //Wait to turn on USB Host
  myusb.begin();
  midi1.setHandleControlChange(myControlChange);
  midi1.setHandleNoteOff(myNoteOff);
  midi1.setHandleNoteOn(myNoteOn);
  midi1.setHandlePitchChange(myPitchBend);
  midi1.setHandleProgramChange(myProgramChange);
  midi1.setHandleClock(myMIDIClock);
  midi1.setHandleStart(myMIDIClockStart);
  midi1.setHandleStop(myMIDIClockStop);
  Serial.println(F("USB HOST MIDI Class Compliant Listening"));

  //USB Client MIDI
  usbMIDI.setHandleControlChange(myControlChange);
  usbMIDI.setHandleNoteOff(myNoteOff);
  usbMIDI.setHandleNoteOn(myNoteOn);
  usbMIDI.setHandlePitchChange(myPitchBend);
  usbMIDI.setHandleProgramChange(myProgramChange);
  usbMIDI.setHandleClock(myMIDIClock);
  usbMIDI.setHandleStart(myMIDIClockStart);
  usbMIDI.setHandleStop(myMIDIClockStop);
  Serial.println(F("USB Client MIDI Listening"));

  //MIDI 5 Pin DIN
  MIDI.begin();
  MIDI.setHandleNoteOn(myNoteOn);
  MIDI.setHandleNoteOff(myNoteOff);
  MIDI.setHandlePitchBend(myPitchBend);
  MIDI.setHandleControlChange(myControlChange);
  MIDI.setHandleProgramChange(myProgramChange);
  MIDI.setHandleClock(myMIDIClock);
  MIDI.setHandleStart(myMIDIClockStart);
  MIDI.setHandleStop(myMIDIClockStop);
  Serial.println(F("MIDI In DIN Listening"));

  constant1Dc.amplitude(ONE);

  voiceMixerM.gain(0, 0.25f);
  voiceMixerM.gain(1, 0.25f);
  voiceMixerM.gain(2, 0.25f);
  voiceMixerM.gain(3, 0.25f);

  pink.amplitude(ONE);
  white.amplitude(ONE);

  voiceMixerM.gain(0, VOICEMIXERLEVEL);
  voiceMixerM.gain(1, VOICEMIXERLEVEL);
  voiceMixerM.gain(2, VOICEMIXERLEVEL);
  voiceMixerM.gain(3, VOICEMIXERLEVEL);

  //This removes dc offset (mostly from unison pulse waves) before the ensemble effect
  dcOffsetFilter.octaveControl(1.0f);
  dcOffsetFilter.frequency(12.0f);//Lower values will give clicks on note on/off

  volumeMixer.gain(0, 1.0f);
  volumeMixer.gain(1, 0);
  volumeMixer.gain(2, 0);
  volumeMixer.gain(3, 0);

  ensemble.lfoRate(fxAmt);

  volumePrevious = RE_READ; //Force volume control to be read and set to current

  //Read Pitch Bend Range from EEPROM
  pitchBendRange = getPitchBendRange();
  //Read Mod Wheel Depth from EEPROM
  modWheelDepth = getModWheelDepth();
  //Read MIDI Out Channel from EEPROM
  midiOutCh = getMIDIOutCh();
  //Read MIDI Thru mode from EEPROM
  MIDIThru = getMidiThru();
  changeMIDIThruMode();
  //Read Encoder Direction from EEPROM
  encCW = getEncoderDir();
  //Read Pick-up enable from EEPROM - experimental feature
  pickUp = getPickupEnable();
  //Read bass enhance enable from EEPROM
  if (getBassEnhanceEnable()) sgtl5000_1.enhanceBassEnable();
  //Read oscilloscope enable from EEPROM
  enableScope(getScopeEnable());
  //Read VU enable from EEPROM
  vuMeter = getVUEnable();
  //Read Filter and Amp Envelope shapes
  reloadFiltEnv();
  reloadAmpEnv();
}

void myNoteOn(byte channel, byte note, byte velocity) {
  //Check for out of range notes
  if (note + voices.params().oscPitchA < 0 || note + voices.params().oscPitchA > 127 || note + voices.params().oscPitchB < 0 || note + voices.params().oscPitchB > 127)
    return;

  voices.noteOn(note, velocity);
}

void myNoteOff(byte channel, byte note, byte velocity) {
  voices.noteOff(note);
}

int getLFOWaveform(int value) {
  if (value >= 0 && value < 8) {
    return WAVEFORM_SINE;
  } else if (value >= 8 && value < 30) {
    return WAVEFORM_TRIANGLE;
  } else if (value >= 30 && value < 63) {
    return WAVEFORM_SAWTOOTH_REVERSE;
  } else if (value >= 63 && value < 92) {
    return WAVEFORM_SAWTOOTH;
  } else if (value >= 92 && value < 111) {
    return WAVEFORM_SQUARE;
  } else {
    return WAVEFORM_SAMPLE_HOLD;
  }
}

FLASHMEM String getWaveformStr(int value) {
  switch (value) {
    case WAVEFORM_SILENT:
      return F("Off");
    case WAVEFORM_SAMPLE_HOLD:
      return F("Sample & Hold");
    case WAVEFORM_SINE:
      return F("Sine");
    case WAVEFORM_BANDLIMIT_SQUARE:
    case WAVEFORM_SQUARE:
      return F("Square");
    case WAVEFORM_TRIANGLE:
      return F("Triangle");
    case WAVEFORM_BANDLIMIT_SAWTOOTH:
    case WAVEFORM_SAWTOOTH:
      return F("Sawtooth");
    case WAVEFORM_SAWTOOTH_REVERSE:
      return F("Ramp");
    case WAVEFORM_BANDLIMIT_PULSE:
      return F("Var. Pulse");
    case WAVEFORM_TRIANGLE_VARIABLE:
      return F("Var. Triangle");
    case WAVEFORM_PARABOLIC:
      return F("Parabolic");
    case WAVEFORM_HARMONIC:
      return F("Harmonic");
    default:
      return F("ERR_WAVE");
  }
}

FLASHMEM int getWaveformA(int value) {
  if (value >= 0 && value < 7) {
    //This will turn the osc off
    return WAVEFORM_SILENT;
  } else if (value >= 7 && value < 23) {
    return WAVEFORM_TRIANGLE;
  } else if (value >= 23 && value < 40) {
    return WAVEFORM_BANDLIMIT_SQUARE;
  } else if (value >= 40 && value < 60) {
    return WAVEFORM_BANDLIMIT_SAWTOOTH;
  } else if (value >= 60 && value < 80) {
    return WAVEFORM_BANDLIMIT_PULSE;
  } else if (value >= 80 && value < 100) {
    return WAVEFORM_TRIANGLE_VARIABLE;
  } else if (value >= 100 && value < 120) {
    return WAVEFORM_PARABOLIC;
  } else {
    return WAVEFORM_HARMONIC;
  }
}

FLASHMEM int getWaveformB(int value) {
  if (value >= 0 && value < 7)  {
    //This will turn the osc off
    return WAVEFORM_SILENT;
  } else if (value >= 7 && value < 23) {
    return WAVEFORM_SAMPLE_HOLD;
  } else if (value >= 23 && value < 40) {
    return WAVEFORM_BANDLIMIT_SQUARE;
  } else if (value >= 40 && value < 60) {
    return WAVEFORM_BANDLIMIT_SAWTOOTH;
  } else if (value >= 60 && value < 80) {
    return WAVEFORM_BANDLIMIT_PULSE;
  } else if (value >= 80 && value < 100) {
    return WAVEFORM_TRIANGLE_VARIABLE;
  } else if (value >= 100 && value < 120) {
    return WAVEFORM_PARABOLIC;
  } else {
    return WAVEFORM_HARMONIC;
  }
}

FLASHMEM void updateUnison(uint8_t unison) {
  voices.setUnisonMode(unison);

  if (unison == 0) {
    showCurrentParameterPage("Unison", "Off");
    pinMode(UNISON_LED, OUTPUT);
    digitalWriteFast(UNISON_LED, LOW);  // LED off
  } else if (unison == 1) {
    showCurrentParameterPage("Dyn. Unison", "On");
    pinMode(UNISON_LED, OUTPUT);
    digitalWriteFast(UNISON_LED, HIGH);  // LED on
  } else {
    showCurrentParameterPage("Chd. Unison", "On");
    analogWriteFrequency(UNISON_LED, 1);
    analogWrite(UNISON_LED, 127);
  }
}

FLASHMEM void updateVolume(float vol) {
  sgtl5000_1.volume(vol * SGTL_MAXVOLUME);
  showCurrentParameterPage("Volume", vol);
}

FLASHMEM void updateGlide(float glideSpeed) {
  voices.params().glideSpeed = glideSpeed;
  showCurrentParameterPage("Glide", milliToString(glideSpeed * GLIDEFACTOR));
}

FLASHMEM void updateWaveformA(uint32_t waveform) {
  voices.setWaveformA(waveform);
  showCurrentParameterPage("1. Waveform", getWaveformStr(waveform));
}

FLASHMEM void updateWaveformB(uint32_t waveform) {
  voices.setWaveformB(waveform);
  showCurrentParameterPage("2. Waveform", getWaveformStr(waveform));
}

FLASHMEM void updatePitchA(int pitch) {
  voices.params().oscPitchA = pitch;
  voices.updateVoices();
  showCurrentParameterPage("1. Semitones", (pitch > 0 ? "+" : "") + String(pitch));
}

FLASHMEM void updatePitchB(int pitch) {
  voices.params().oscPitchB = pitch;
  voices.updateVoices();
  showCurrentParameterPage("2. Semitones", (pitch > 0 ? "+" : "") + String(pitch));
}

FLASHMEM void updateDetune(float detune, uint32_t chordDetune) {
  voices.params().detune = detune;
  voices.params().chordDetune = chordDetune;
  voices.updateVoices();

  if (voices.params().unisonMode == 2) {
    showCurrentParameterPage("Chord", CDT_STR[chordDetune]);
  } else {
    showCurrentParameterPage("Detune", String((1 - detune) * 100) + " %");
  }
}

FLASHMEM void updatePWMSource(uint8_t source) {
  voices.setPWMSource(source);

  if (source == PWMSOURCELFO) {
    showCurrentParameterPage("PWM Source", "LFO"); //Only shown when updated via MIDI
  } else {
    showCurrentParameterPage("PWM Source", "Filter Env");
  }
}

FLASHMEM void updatePWMRate(float value) {
  voices.setPwmRate(value);

  if (value == PWMRATE_PW_MODE) {
    //Set to fixed PW mode
    showCurrentParameterPage("PW Mode", "On");
  } else if (value == PWMRATE_SOURCE_FILTER_ENV) {
    //Set to Filter Env Mod source
    showCurrentParameterPage("PWM Source", "Filter Env");
  } else {
    showCurrentParameterPage("PWM Rate", String(2 * value) + " Hz"); //PWM goes through mid to maximum, sounding effectively twice as fast
  }
}

FLASHMEM void updatePWMAmount(float value) {
  //MIDI only - sets both osc PWM
  voices.overridePwmAmount(value);
  showCurrentParameterPage("PWM Amt", String(value) + " : " + String(value));
}

FLASHMEM void updatePWA(float valuePwA, float valuePwmAmtA) {
  voices.setPWA(valuePwA, valuePwmAmtA);

  if (voices.getPwmRate() == PWMRATE_PW_MODE) {
    if (voices.getWaveformA() == WAVEFORM_TRIANGLE_VARIABLE) {
      showCurrentParameterPage("1. PW Amt", voices.getPwA(), VAR_TRI);
    } else {
      showCurrentParameterPage("1. PW Amt", voices.getPwA(), PULSE);
    }
  } else {
    if (voices.getPwmSource() == PWMSOURCELFO) {
      //PW alters PWM LFO amount for waveform A
      showCurrentParameterPage("1. PWM Amt", "LFO " + String(voices.getPwmAmtA()));
    } else {
      //PW alters PWM Filter Env amount for waveform A
      showCurrentParameterPage("1. PWM Amt", "F. Env " + String(voices.getPwmAmtA()));
    }
  }
}

FLASHMEM void updatePWB(float valuePwB, float valuePwmAmtB) {
  voices.setPWB(valuePwB, valuePwmAmtB);

  if (voices.getPwmRate() == PWMRATE_PW_MODE)  {
    if (voices.getWaveformB() == WAVEFORM_TRIANGLE_VARIABLE) {
      showCurrentParameterPage("2. PW Amt", voices.getPwB(), VAR_TRI);
    } else {
      showCurrentParameterPage("2. PW Amt", voices.getPwB(), PULSE);
    }
  } else {
    if (voices.getPwmSource() == PWMSOURCELFO) {
      //PW alters PWM LFO amount for waveform B
      showCurrentParameterPage("2. PWM Amt", "LFO " + String(voices.getPwmAmtB()));
    } else {
      //PW alters PWM Filter Env amount for waveform B
      showCurrentParameterPage("2. PWM Amt", "F. Env " + String(voices.getPwmAmtB()));
    }
  }
}

FLASHMEM void updateOscLevelA(float value) {
  voices.setOscLevelA(value);

  switch (voices.getOscFX()) {
    case 1://XOR
      showCurrentParameterPage("Osc Mix 1:2", "   " + String(voices.getOscLevelA()) + " : " + String(voices.getOscLevelB()));
      break;
    case 2://XMod
      //osc A sounds with increasing osc B mod
      if (voices.getOscLevelA() == 1.0f && voices.getOscLevelB() <= 1.0f) {
        showCurrentParameterPage("XMod Osc 1", "Osc 2: " + String(1 - voices.getOscLevelB()));
      }
      break;
    case 0://None
      showCurrentParameterPage("Osc Mix 1:2", "   " + String(voices.getOscLevelA()) + " : " + String(voices.getOscLevelB()));
      break;
  }
}

FLASHMEM void updateOscLevelB(float value) {
  voices.setOscLevelB(value);

  switch (voices.getOscFX()) {
    case 1://XOR
      showCurrentParameterPage("Osc Mix 1:2", "   " + String(voices.getOscLevelA()) + " : " + String(voices.getOscLevelB()));
      break;
    case 2://XMod
      //osc B sounds with increasing osc A mod
      if (voices.getOscLevelB() == 1.0f && voices.getOscLevelA() < 1.0f) {
        showCurrentParameterPage("XMod Osc 2", "Osc 1: " + String(1 - voices.getOscLevelA()));
      }
      break;
    case 0://None
      showCurrentParameterPage("Osc Mix 1:2", "   " + String(voices.getOscLevelA()) + " : " + String(voices.getOscLevelB()));
      break;
  }
}

FLASHMEM void updateNoiseLevel(float value) {
  float pink = 0.0;
  float white = 0.0;
  if (value > 0) {
    pink = value;
  } else if (value < 0) {
    white = abs(value);
  }

  voices.setPinkNoiseLevel(pink);
  voices.setWhiteNoiseLevel(white);

  if (value > 0) {
    showCurrentParameterPage("Noise Level", "Pink " + String(value));
  } else if (value < 0) {
    showCurrentParameterPage("Noise Level", "White " + String(abs(value)));
  } else {
    showCurrentParameterPage("Noise Level", "Off");
  }
}

FLASHMEM void updateFilterFreq(float value) {
  voices.setCutoff(value);
  showCurrentParameterPage("Cutoff", String(int(value)) + " Hz");
}

FLASHMEM void updateFilterRes(float value) {
  voices.setResonance(value);
  showCurrentParameterPage("Resonance", value);
}

FLASHMEM void updateFilterMixer(float value) {
  voices.setFilterMixer(value);

  String filterStr;
  if (value == BANDPASS) {
    filterStr = "Band Pass";
  } else {
    //LP-HP mix mode - a notch filter
    if (value == LOWPASS) {
      filterStr = "Low Pass";
    }
    else if (value == HIGHPASS) {
      filterStr = "High Pass";
    }
    else {
      filterStr = "LP " + String(100 - int(100*value)) + " - " + String(int(100*value)) + " HP";
    }
  }

  showCurrentParameterPage("Filter Type", filterStr);
}

FLASHMEM void updateFilterEnv(float value) {
  voices.setFilterEnvelope(value);
  showCurrentParameterPage("Filter Env.", String(value));
}

FLASHMEM void updatePitchEnv(float value) {
  voices.setPitchEnvelope(value);
  showCurrentParameterPage("Pitch Env Amt", String(value));
}

FLASHMEM void updateKeyTracking(float value) {
  voices.setKeytracking(value);
  showCurrentParameterPage("Key Tracking", String(value));
}

FLASHMEM void updatePitchLFOAmt(float value) {
  voices.setPitchLfoAmount(value);
  char buf[10];
  showCurrentParameterPage("LFO Amount", dtostrf(value, 4, 3, buf));
}

FLASHMEM void updateModWheel(float value) {
  voices.setModWhAmount(value);
}

FLASHMEM void updatePitchLFORate(float value) {
  voices.setPitchLfoRate(value);
  showCurrentParameterPage("LFO Rate", String(value) + " Hz");
}

FLASHMEM void updatePitchLFOWaveform(uint32_t waveform) {
  voices.setPitchLfoWaveform(waveform);
  showCurrentParameterPage("Pitch LFO", getWaveformStr(waveform));
}

//MIDI CC only
FLASHMEM void updatePitchLFOMidiClkSync(bool value) {
  voices.setPitchLfoMidiClockSync(value);
  showCurrentParameterPage("P. LFO Sync", value ? "On" : "Off");
}

FLASHMEM void updateFilterLfoRate(float value, String timeDivStr) {
  voices.setFilterLfoRate(value);

  if (timeDivStr.length() > 0) {
    showCurrentParameterPage("LFO Time Div", timeDivStr);
  } else {
    showCurrentParameterPage("F. LFO Rate", String(value) + " Hz");
  }
}

FLASHMEM void updateFilterLfoAmt(float value) {
  voices.setFilterLfoAmt(value);
  showCurrentParameterPage("F. LFO Amt", String(value));
}

FLASHMEM void updateFilterLFOWaveform(uint32_t waveform) {
  voices.setFilterLfoWaveform(waveform);
  showCurrentParameterPage("Filter LFO", getWaveformStr(waveform));
}

FLASHMEM void updatePitchLFORetrig(bool value) {
  voices.setPitchLfoRetrig(value);
  showCurrentParameterPage("P. LFO Retrig", value ? "On" : "Off");
}

FLASHMEM void updateFilterLFORetrig(bool value) {
  voices.setFilterLfoRetrig(value);
  showCurrentParameterPage("F. LFO Retrig", voices.getFilterLfoRetrig() ? "On" : "Off");
  digitalWriteFast(RETRIG_LED, voices.getFilterLfoRetrig() ? HIGH : LOW);  // LED
}

FLASHMEM void updateFilterLFOMidiClkSync(bool value) {
  voices.setFilterLfoMidiClockSync(value);
  showCurrentParameterPage("Tempo Sync", value ? "On" : "Off");
  digitalWriteFast(TEMPO_LED, value ? HIGH : LOW);  // LED
}

FLASHMEM void updateFilterAttack(float value) {
  voices.setFilterAttack(value);
  showCurrentParameterPage("Filter Attack", milliToString(value), FILTER_ENV);
}

FLASHMEM void updateFilterDecay(float value) {
  voices.setFilterDecay(value);
  showCurrentParameterPage("Filter Decay", milliToString(value), FILTER_ENV);
}

FLASHMEM void updateFilterSustain(float value) {
  voices.setFilterSustain(value);
  showCurrentParameterPage("Filter Sustain", String(value), FILTER_ENV);
}

FLASHMEM void updateFilterRelease(float value) {
  voices.setFilterRelease(value);
  showCurrentParameterPage("Filter Release", milliToString(value), FILTER_ENV);
}

FLASHMEM void updateAttack(float value) {
  voices.setAmpAttack(value);
  showCurrentParameterPage("Attack", milliToString(value), AMP_ENV);
}

FLASHMEM void updateDecay(float value) {
  voices.setAmpDecay(value);
  showCurrentParameterPage("Decay", milliToString(value), AMP_ENV);
}

FLASHMEM void updateSustain(float value) {
  voices.setAmpSustain(value);
  showCurrentParameterPage("Sustain", String(value), AMP_ENV);
}

FLASHMEM void updateRelease(float value) {
  voices.setAmpRelease(value);
  showCurrentParameterPage("Release", milliToString(value), AMP_ENV);
}

FLASHMEM void updateOscFX(uint8_t value) {
  voices.setOscFX(value);
  if (value == 2) {
    showCurrentParameterPage("Osc FX", "On - X Mod");
    analogWriteFrequency(OSC_FX_LED, 1);
    analogWrite(OSC_FX_LED, 127);
  } else if (value == 1) {
    showCurrentParameterPage("Osc FX", "On - XOR");
    pinMode(OSC_FX_LED, OUTPUT);
    digitalWriteFast(OSC_FX_LED, HIGH);  // LED on
  } else {
    showCurrentParameterPage("Osc FX", "Off");
    pinMode(OSC_FX_LED, OUTPUT);
    digitalWriteFast(OSC_FX_LED, LOW);  // LED off
  }
}

FLASHMEM void updateFXAmt() {
  ensemble.lfoRate(fxAmt);
  showCurrentParameterPage("Effect Amt", String(fxAmt) + " Hz");
}

FLASHMEM void updateFXMix() {
  effectMixerL.gain(0, 1.0f - fxMix); //Dry
  effectMixerL.gain(1, fxMix);       //Wet
  effectMixerR.gain(0, 1.0f - fxMix); //Dry
  effectMixerR.gain(1, fxMix);       //Wet
  showCurrentParameterPage("Effect Mix", String(fxMix));
}

FLASHMEM void updatePatch(String name, uint32_t index) {
  voices.setPatchName(name);
  voices.setPatchIndex(index);
  showPatchPage(String(index), name);
}

void myPitchBend(byte channel, int bend) {
  // 0.5 to give 1oct max - spread of mod is 2oct
  voices.pitchBend(bend * 0.5f * pitchBendRange * DIV12 * DIV8192);
}

void myControlChange(byte channel, byte control, byte value) {
  switch (control) {
    case CCvolume:
      updateVolume(LINEAR[value]);
      break;
    case CCunison:
      updateUnison(inRangeOrDefault<int>(value, 2, 0, 2));
      break;

    case CCglide:
      updateGlide(POWER[value]);
      break;

    case CCpitchenv:
      updatePitchEnv(LINEARCENTREZERO[value] * OSCMODMIXERMAX);
      break;

    case CCoscwaveformA:
      updateWaveformA(getWaveformA(value));
      break;

    case CCoscwaveformB:
      updateWaveformB(getWaveformB(value));
     break;
     
     case CCpitchA:
      updatePitchA(PITCH[value]);
      break;    
     
    case CCpitchB:
      updatePitchB(PITCH[value]);
      break;

    case CCdetune:
      updateDetune(1.0f - (MAXDETUNE * POWER[value]), value);
      break;

    case CCpwmSource:
      updatePWMSource(value > 0 ? PWMSOURCEFENV : PWMSOURCELFO);
      break;

    case CCpwmRate:
      //Uses combination of PWMRate, PWa and PWb
      updatePWMRate(PWMRATE[value]);
      break;

    case CCpwmAmt:
      //NO FRONT PANEL CONTROL - MIDI CC ONLY
      //Total PWM amount for both oscillators
      updatePWMAmount(LINEAR[value]);
      break;

    case CCpwA:
      updatePWA(LINEARCENTREZERO[value], LINEAR[value]);
      break;

    case CCpwB:
      updatePWB(LINEARCENTREZERO[value], LINEAR[value]);
      break;

    case CCoscLevelA:
      updateOscLevelA(LINEAR[value]);
      break;

    case CCoscLevelB:
      updateOscLevelB(LINEAR[value]);
      break;

    case CCnoiseLevel:
      updateNoiseLevel(LINEARCENTREZERO[value]);
      break;

    case CCfilterfreq:
      //Pick up
      if (!pickUpActive && pickUp && (filterfreqPrevValue <  FILTERFREQS256[(value - TOLERANCE) * 2] || filterfreqPrevValue >  FILTERFREQS256[(value - TOLERANCE) * 2])) return; //PICK-UP

      //MIDI is 7 bit, 128 values and needs to choose alternate filterfreqs(8 bit) by multiplying by 2
      updateFilterFreq(FILTERFREQS256[value * 2]);
      filterfreqPrevValue = FILTERFREQS256[value * 2];//PICK-UP
      break;

    case CCfilterres:
      //Pick up
      if (!pickUpActive && pickUp && (resonancePrevValue <  ((13.9f * POWER[value - TOLERANCE]) + 1.1f) || resonancePrevValue >  ((13.9f * POWER[value + TOLERANCE]) + 1.1f))) return; //PICK-UP
      
      //If <1.1 there is noise at high cutoff freq
      updateFilterRes((13.9f * POWER[value]) + 1.1f);
      resonancePrevValue = (13.9f * POWER[value]) + 1.1f;//PICK-UP
      break;

    case CCfiltermixer:
      //Pick up
      if (!pickUpActive && pickUp && (filterMixPrevValue <  LINEAR_FILTERMIXER[value - TOLERANCE] || filterMixPrevValue >  LINEAR_FILTERMIXER[value + TOLERANCE])) return; //PICK-UP

      updateFilterMixer(LINEAR_FILTERMIXER[value]);
      filterMixPrevValue = LINEAR_FILTERMIXER[value];//PICK-UP
      break;

    case CCfilterenv:
      updateFilterEnv(LINEARCENTREZERO[value] * FILTERMODMIXERMAX);
      break;

    case CCkeytracking:
      updateKeyTracking(KEYTRACKINGAMT[value]);
      break;

    case CCmodwheel:
      //Variable LFO amount from mod wheel - Settings Option
      updateModWheel(POWER[value] * modWheelDepth);
      break;

    case CCosclfoamt:
      //Pick up
      if (!pickUpActive && pickUp && (oscLfoAmtPrevValue <  POWER[value - TOLERANCE] || oscLfoAmtPrevValue >  POWER[value + TOLERANCE])) return; //PICK-UP

      updatePitchLFOAmt(POWER[value]);
      oscLfoAmtPrevValue = POWER[value];//PICK-UP
      break;

    case CCoscLfoRate: {
       //Pick up
       if (!pickUpActive && pickUp && (oscLfoRatePrevValue <  LFOMAXRATE * POWER[value - TOLERANCE] || oscLfoRatePrevValue > LFOMAXRATE * POWER[value + TOLERANCE])) return; //PICK-UP
      
      float rate = 0.0;
      if (voices.getPitchLfoMidiClockSync()) {
        // TODO: MIDI Tempo stuff remains global?
        lfoTempoValue = LFOTEMPO[value];
         oscLFOTimeDivStr = LFOTEMPOSTR[value];
        rate = lfoSyncFreq * LFOTEMPO[value];
       }
       else {
        rate = LFOMAXRATE * POWER[value];
       }
      updatePitchLFORate(rate);
      oscLfoRatePrevValue = rate;//PICK-UP
       break;
    }

    case CCoscLfoWaveform:
      updatePitchLFOWaveform(getLFOWaveform(value));
      break;

    case CCosclforetrig:
      updatePitchLFORetrig(value > 0);
      break;

    case CCfilterLFOMidiClkSync:
      updateFilterLFOMidiClkSync(value > 0);
      break;

    case CCfilterlforate: {
      //Pick up
      if (!pickUpActive && pickUp && (filterLfoRatePrevValue <  LFOMAXRATE * POWER[value - TOLERANCE] || filterLfoRatePrevValue > LFOMAXRATE * POWER[value + TOLERANCE])) return; //PICK-UP

      float rate;
      String timeDivStr = "";
      if (voices.getFilterLfoMidiClockSync()) {
        lfoTempoValue = LFOTEMPO[value];
        rate = lfoSyncFreq * LFOTEMPO[value];
        timeDivStr = LFOTEMPOSTR[value];
      } else {
        rate = LFOMAXRATE * POWER[value];
      }

      updateFilterLfoRate(rate, timeDivStr);
      filterLfoRatePrevValue = rate;//PICK-UP
      break;
    }

    case CCfilterlfoamt:
      //Pick up
      if (!pickUpActive && pickUp && (filterLfoAmtPrevValue <  LINEAR[value - TOLERANCE] * FILTERMODMIXERMAX || filterLfoAmtPrevValue >  LINEAR[value + TOLERANCE] * FILTERMODMIXERMAX)) return; //PICK-UP

      updateFilterLfoAmt(LINEAR[value] * FILTERMODMIXERMAX);
      filterLfoAmtPrevValue = LINEAR[value] * FILTERMODMIXERMAX;//PICK-UP
      break;

    case CCfilterlfowaveform:
      updateFilterLFOWaveform(getLFOWaveform(value));
      break;

    case CCfilterlforetrig:
      updateFilterLFORetrig(value > 0);
      break;

    //MIDI Only
    case CCoscLFOMidiClkSync:
      updatePitchLFOMidiClkSync(value > 0);
      break;

    case CCfilterattack:
      updateFilterAttack(ENVTIMES[value]);
      break;

    case CCfilterdecay:
      updateFilterDecay(ENVTIMES[value]);
      break;

    case CCfiltersustain:
      updateFilterSustain(LINEAR[value]);
      break;

    case CCfilterrelease:
      updateFilterRelease(ENVTIMES[value]);
      break;

    case CCampattack:
      updateAttack(ENVTIMES[value]);
      break;

    case CCampdecay:
      updateDecay(ENVTIMES[value]);
      break;

    case CCampsustain:
      updateSustain(LINEAR[value]);
      break;

    case CCamprelease:
      updateRelease(ENVTIMES[value]);
      break;

    case CCoscfx:
      updateOscFX(inRangeOrDefault<int>(value, 2, 0, 2));
      break;

    case CCfxamt:
      //Pick up
      if (!pickUpActive && pickUp && (fxAmtPrevValue <  ENSEMBLE_LFO[value - TOLERANCE] || fxAmtPrevValue >  ENSEMBLE_LFO[value + TOLERANCE])) return; //PICK-UP
      fxAmt = ENSEMBLE_LFO[value];
      updateFXAmt();
      fxAmtPrevValue = fxAmt;//PICK-UP
      break;

    case CCfxmix:
      //Pick up
      if (!pickUpActive && pickUp && (fxMixPrevValue <  LINEAR[value - TOLERANCE] || fxMixPrevValue >  LINEAR[value + TOLERANCE])) return; //PICK-UP
      fxMix = LINEAR[value];
      updateFXMix();
      fxMixPrevValue = fxMix;//PICK-UP
      break;

    case CCallnotesoff:
      voices.allNotesOff();
      break;
  }
}

FLASHMEM void myProgramChange(byte channel, byte program) {
  state = PATCH;
  patchNo = program + 1;
  recallPatch(patchNo);
  Serial.print(F("MIDI Pgm Change:"));
  Serial.println(patchNo);
  state = PARAMETER;
}

FLASHMEM void myMIDIClockStart() {
  MIDIClkSignal = true;
  //Resync LFOs when MIDI Clock starts.
  //When there's a jump to a different
  //part of a track, such as in a DAW, the DAW must have same
  //rhythmic quantisation as Tempo Div.

  // TODO: Apply to all voices. Maybe check channel?
  voices.midiClockStart();
}

FLASHMEM void myMIDIClockStop() {
  MIDIClkSignal = false;
}

FLASHMEM void myMIDIClock() {
  //This recalculates the LFO frequencies if the tempo changes (MIDI cLock is 24ppq)
  if (count > 23) {
    // TODO: Most of this needs to move into the VoiceGroup

    MIDIClkSignal = !MIDIClkSignal;
    float timeNow = millis();
    midiClkTimeInterval = (timeNow - previousMillis);
    lfoSyncFreq = 1000.0f / midiClkTimeInterval;
    previousMillis = timeNow;
    voices.midiClock(lfoSyncFreq * lfoTempoValue);
    count = 0;
  }

  count++;
}

FLASHMEM void recallPatch(int patchNo) {
  voices.allNotesOff();
  voices.closeEnvelopes();
  File patchFile = SD.open(String(patchNo).c_str());
  if (!patchFile) {
    Serial.println(F("File not found"));
  } else {
    String data[NO_OF_PARAMS]; //Array of data read in
    recallPatchData(patchFile, data);
    setCurrentPatchData(data);
    patchFile.close();
  }
}

FLASHMEM void setCurrentPatchData(String data[]) {
  updatePatch(data[0], patchNo);
  updateOscLevelA(data[1].toFloat());
  updateOscLevelB(data[2].toFloat());
  updateNoiseLevel(data[3].toFloat());
  updateUnison(data[4].toInt());
  updateOscFX(data[5].toInt());
  updateDetune(data[6].toFloat(), data[48].toInt());
  // Why is this MIDI Clock stuff part of the patch??
  lfoSyncFreq = data[7].toInt();
  midiClkTimeInterval = data[8].toInt();
  lfoTempoValue = data[9].toFloat();
  updateKeyTracking(data[10].toFloat());
  updateGlide(data[11].toFloat());
  updatePitchA(data[12].toFloat());
  updatePitchB(data[13].toFloat());
  updateWaveformA(data[14].toInt());
  updateWaveformB(data[15].toInt());
  updatePWMSource(data[16].toInt());
  updatePWA(data[20].toFloat(), data[17].toFloat());
  updatePWA(data[21].toFloat(), data[18].toFloat());
  updatePWMRate(data[19].toFloat());
  updateFilterRes(data[22].toFloat());
  resonancePrevValue = data[22].toFloat();//Pick-up
  updateFilterFreq(data[23].toFloat());
  filterfreqPrevValue = data[23].toInt(); //Pick-up
  updateFilterMixer(data[24].toFloat());
  filterMixPrevValue = data[24].toFloat(); //Pick-up
  updateFilterEnv(data[25].toFloat());
  updatePitchLFOAmt(data[26].toFloat());
  oscLfoAmtPrevValue = data[26].toFloat();//PICK-UP
  updatePitchLFORate(data[27].toFloat());
  oscLfoRatePrevValue = data[27].toFloat();//PICK-UP
  updatePitchLFOWaveform(data[28].toInt());
  updatePitchLFORetrig(data[29].toInt() > 0);
  updatePitchLFOMidiClkSync(data[30].toInt() > 0); // MIDI CC Only
  updateFilterLfoRate(data[31].toFloat(), "");
  filterLfoRatePrevValue = data[31].toFloat();//PICK-UP
  updateFilterLFORetrig(data[32].toInt() > 0);
  updateFilterLFOMidiClkSync(data[33].toInt() > 0);
  updateFilterLfoAmt(data[34].toFloat());
  filterLfoAmtPrevValue = data[34].toFloat();//PICK-UP
  updateFilterLFOWaveform(data[35].toFloat());
  updateFilterAttack(data[36].toFloat());
  updateFilterDecay(data[37].toFloat());
  updateFilterSustain(data[38].toFloat());
  updateFilterRelease(data[39].toFloat());
  updateAttack(data[40].toFloat());
  updateDecay(data[41].toFloat());
  updateSustain(data[42].toFloat());
  updateRelease(data[43].toFloat());
  fxAmt = data[44].toFloat();
  fxAmtPrevValue = fxAmt;//PICK-UP
  fxMix = data[45].toFloat();
  fxMixPrevValue = fxMix;//PICK-UP
  updatePitchEnv(data[46].toFloat());
  velocitySens = data[47].toFloat();
  voices.setMonophonic(data[49].toInt());
  //  SPARE1 = data[50].toFloat();
  //  SPARE2 = data[51].toFloat();

  updateFXAmt();
  updateFXMix();
  Serial.print(F("Set Patch: "));
  Serial.println(data[0]);
}

FLASHMEM String getCurrentPatchData() {
  auto p = voices.params();
  return patchName + "," + String(voices.getOscLevelA()) + "," + String(voices.getOscLevelB()) + "," + String(voices.getPinkNoiseLevel() - voices.getWhiteNoiseLevel()) + "," + String(p.unisonMode) + "," + String(voices.getOscFX()) + "," + String(p.detune, 5) + "," + String(lfoSyncFreq) + "," + String(midiClkTimeInterval) + "," + String(lfoTempoValue) + "," + String(voices.getKeytrackingAmount()) + "," + String(p.glideSpeed, 5) + "," + String(p.oscPitchA) + "," + String(p.oscPitchB) + "," + String(voices.getWaveformA()) + "," + String(voices.getWaveformB()) + "," +
         String(voices.getPwmSource()) + "," + String(voices.getPwmAmtA()) + "," + String(voices.getPwmAmtB()) + "," + String(voices.getPwmRate()) + "," + String(voices.getPwA()) + "," + String(voices.getPwB()) + "," + String(voices.getResonance()) + "," + String(voices.getCutoff()) + "," + String(voices.getFilterMixer()) + "," + String(voices.getFilterEnvelope()) + "," + String(voices.getPitchLfoAmount(), 5) + "," + String(voices.getPitchLfoRate(), 5) + "," + String(voices.getPitchLfoWaveform()) + "," + String(int(voices.getPitchLfoRetrig())) + "," + String(int(voices.getPitchLfoMidiClockSync())) + "," + String(voices.getFilterLfoRate(), 5) + "," +
         voices.getFilterLfoRetrig() + "," + voices.getFilterLfoMidiClockSync() + "," + voices.getFilterLfoAmt() + "," + voices.getFilterLfoWaveform() + "," + voices.getFilterAttack() + "," + voices.getFilterDecay() + "," + voices.getFilterSustain() + "," + voices.getFilterRelease() + "," + voices.getAmpAttack() + "," + voices.getAmpDecay() + "," + voices.getAmpSustain() + "," + voices.getAmpRelease() + "," +
         String(fxAmt) + "," + String(fxMix) + "," + String(voices.getPitchEnvelope()) + "," + String(velocitySens) + "," + String(p.chordDetune) + "," + String(voices.getMonophonicMode()) + "," + String(0.0f) + "," + String(0.0f);
}

void checkMux() {
  mux1Read = adc->adc1->analogRead(MUX1_S);
  if (mux1Read > (mux1ValuesPrev[muxInput] + QUANTISE_FACTOR) || mux1Read < (mux1ValuesPrev[muxInput] - QUANTISE_FACTOR)) {
    mux1ValuesPrev[muxInput] = mux1Read;
    mux1Read = (mux1Read >> 5); //Change range to 0-127
    switch (muxInput) {
      case MUX1_noiseLevel:
        midiCCOut(CCnoiseLevel, mux1Read);
        myControlChange(midiChannel, CCnoiseLevel, mux1Read);
        break;
      case MUX1_pitchLfoRate:
        midiCCOut(CCoscLfoRate, mux1Read);
        myControlChange(midiChannel, CCoscLfoRate, mux1Read);
        break;
      case MUX1_pitchLfoWaveform:
        midiCCOut(CCoscLfoWaveform, mux1Read);
        myControlChange(midiChannel, CCoscLfoWaveform, mux1Read);
        break;
      case MUX1_pitchLfoAmount:
        midiCCOut(CCosclfoamt, mux1Read);
        myControlChange(midiChannel, CCosclfoamt, mux1Read);
        break;
      case MUX1_detune:
        midiCCOut(CCdetune, mux1Read);
        myControlChange(midiChannel, CCdetune, mux1Read);
        break;
      case MUX1_oscMix:
        midiCCOut(CCoscLevelA, mux1Read);
        midiCCOut(CCoscLevelB, mux1Read);
        myControlChange(midiChannel, CCoscLevelA, OSCMIXA[mux1Read]);
        myControlChange(midiChannel, CCoscLevelB, OSCMIXB[mux1Read]);
        break;
      case MUX1_filterAttack:
        midiCCOut(CCfilterattack, mux1Read);
        myControlChange(midiChannel, CCfilterattack, mux1Read);
        break;
      case MUX1_filterDecay:
        midiCCOut(CCfilterdecay, mux1Read);
        myControlChange(midiChannel, CCfilterdecay, mux1Read);
        break;
      case MUX1_pwmAmountA:
        midiCCOut(CCpwA, mux1Read);
        myControlChange(midiChannel, CCpwA, mux1Read);
        break;
      case MUX1_waveformA:
        midiCCOut(CCoscwaveformA, mux1Read);
        myControlChange(midiChannel, CCoscwaveformA, mux1Read);
        break;
      case MUX1_pitchA:
        midiCCOut(CCpitchA, mux1Read);
        myControlChange(midiChannel, CCpitchA, mux1Read);
        break;
      case MUX1_pwmAmountB:
        midiCCOut(CCpwB, mux1Read);
        myControlChange(midiChannel, CCpwB, mux1Read);
        break;
      case MUX1_waveformB:
        midiCCOut(CCoscwaveformB, mux1Read);
        myControlChange(midiChannel, CCoscwaveformB, mux1Read);
        break;
      case MUX1_pitchB:
        midiCCOut(CCpitchB, mux1Read);
        myControlChange(midiChannel, CCpitchB, mux1Read);
        break;
      case MUX1_pwmRate:
        midiCCOut(CCpwmRate, mux1Read);
        myControlChange(midiChannel, CCpwmRate, mux1Read);
        break;
      case MUX1_pitchEnv:
        midiCCOut(CCpitchenv, mux1Read);
        myControlChange(midiChannel, CCpitchenv, mux1Read);
        break;
    }
  }
  mux2Read = adc->adc1->analogRead(MUX2_S);
  if (mux2Read > (mux2ValuesPrev[muxInput] + QUANTISE_FACTOR) || mux2Read < (mux2ValuesPrev[muxInput] - QUANTISE_FACTOR)) {
    mux2ValuesPrev[muxInput] = mux2Read;
    if (muxInput != MUX2_cutoff) mux2Read = (mux2Read >> 5); //Change range to 0-127
    switch (muxInput) {
      case MUX2_attack:
        midiCCOut(CCampattack, mux2Read);
        myControlChange(midiChannel, CCampattack, mux2Read);
        break;
      case MUX2_decay:
        midiCCOut(CCampdecay, mux2Read);
        myControlChange(midiChannel, CCampdecay, mux2Read);
        break;
      case MUX2_sustain:
        midiCCOut(CCampsustain, mux2Read);
        myControlChange(midiChannel, CCampsustain, mux2Read);
        break;
      case MUX2_release:
        midiCCOut(CCamprelease, mux2Read);
        myControlChange(midiChannel, CCamprelease, mux2Read);
        break;
      case MUX2_filterLFOAmount:
        midiCCOut(CCfilterlfoamt, mux2Read);
        myControlChange(midiChannel, CCfilterlfoamt, mux2Read);
        break;
      case MUX2_FXMix:
        midiCCOut(CCfxmix, mux2Read);
        myControlChange(midiChannel, CCfxmix, mux2Read);
        break;
      case MUX2_FXAmount:
        midiCCOut(CCfxamt, mux2Read);
        myControlChange(midiChannel, CCfxamt, mux2Read);
        break;
      case MUX2_glide:
        midiCCOut(CCglide, mux2Read);
        myControlChange(midiChannel, CCglide, mux2Read);
        break;
      case MUX2_filterEnv:
        midiCCOut(CCfilterenv, mux2Read);
        myControlChange(midiChannel, CCfilterenv, mux2Read);
        break;
      case MUX2_filterRelease:
        midiCCOut(CCfilterrelease, mux2Read);
        myControlChange(midiChannel, CCfilterrelease, mux2Read);
        break;
      case MUX2_filterSustain:
        midiCCOut(CCfiltersustain, mux2Read);
        myControlChange(midiChannel, CCfiltersustain, mux2Read);
        break;
      case MUX2_filterType:
        midiCCOut(CCfiltermixer, mux2Read);
        myControlChange(midiChannel, CCfiltermixer, mux2Read);
        break;
      case MUX2_resonance:
        midiCCOut(CCfilterres, mux2Read);
        myControlChange(midiChannel, CCfilterres, mux2Read);
        break;
      case MUX2_cutoff:
        //Special case - Filter Cutoff is 8 bit, 256 values for smoother changes
        mux2Read = (mux2Read >> 4);
        if (!pickUpActive && pickUp && (filterfreqPrevValue <  FILTERFREQS256[mux2Read - TOLERANCE] || filterfreqPrevValue >  FILTERFREQS256[mux2Read + TOLERANCE])) return; //PICK-UP
        
        updateFilterFreq(FILTERFREQS256[mux2Read]);
        filterfreqPrevValue = FILTERFREQS256[mux2Read];//PICK-UP
        midiCCOut(CCfilterfreq, mux2Read >> 1);
        break;
      case MUX2_filterLFORate:
        midiCCOut(CCfilterlforate, mux2Read);
        myControlChange(midiChannel, CCfilterlforate, mux2Read);
        break;
      case MUX2_filterLFOWaveform:
        midiCCOut(CCfilterlfowaveform, mux2Read);
        myControlChange(midiChannel, CCfilterlfowaveform, mux2Read);
        break;
    }
  }
  muxInput++;
  if (muxInput >= MUXCHANNELS) {
    muxInput = 0;
    checkVolumePot();//Check here
    if (!firstPatchLoaded) {
      recallPatch(patchNo); //Load first patch after all controls read
      firstPatchLoaded = true;
      sgtl5000_1.unmuteHeadphone();
      sgtl5000_1.unmuteLineout();
    }
  }
  digitalWriteFast(MUX_0, muxInput & B0001);
  digitalWriteFast(MUX_1, muxInput & B0010);
  digitalWriteFast(MUX_2, muxInput & B0100);
  digitalWriteFast(MUX_3, muxInput & B1000);
}

void checkVolumePot() {
  volumeRead = adc->adc0->analogRead(VOLUME_POT);
  if (volumeRead > (volumePrevious + QUANTISE_FACTOR) || volumeRead < (volumePrevious - QUANTISE_FACTOR))  {
    volumePrevious = volumeRead;
    volumeRead = (volumeRead >> 5); //Change range to 0-127
    myControlChange(midiChannel, CCvolume, volumeRead);
  }
}

void checkSwitches() {
  unisonSwitch.update();
  if (unisonSwitch.read() == LOW && unisonSwitch.duration() > HOLD_DURATION) {
    //If unison held, switch to unison 2
    midiCCOut(CCunison, 2);
    myControlChange(midiChannel, CCunison, 2);
    unisonSwitch.write(HIGH); //Come out of this state
    unison2 = true;           //Hack
  } else  if (unisonSwitch.fallingEdge()) {
    if (!unison2) {
      uint8_t next = voices.params().unisonMode > 0 ? 0 : 1;
      midiCCOut(CCunison, next);
      myControlChange(midiChannel, CCunison, next);
    } else {
      unison2 = false;
    }
  }

  oscFXSwitch.update();
  if (oscFXSwitch.read() == LOW && oscFXSwitch.duration() > HOLD_DURATION) {
    //If oscFX held, switch to oscFX 2
    midiCCOut(CCoscfx, 2);
    myControlChange(midiChannel, CCoscfx, 2);
    oscFXSwitch.write(HIGH); //Come out of this state
    oscFXMode = true;//Hack
  } else if (oscFXSwitch.fallingEdge()) {
    if (!oscFXMode) {
      uint8_t value = voices.getOscFX() > 0 ? 0 : 1;
      midiCCOut(CCoscfx, value);
      myControlChange(midiChannel, CCoscfx, value);
    } else {
      oscFXMode = false;
    }
  }

  filterLFORetrigSwitch.update();
  if (filterLFORetrigSwitch.fallingEdge()) {
    bool value = !voices.getFilterLfoRetrig();
    midiCCOut(CCfilterlforetrig, value);
    myControlChange(midiChannel, CCfilterlforetrig, value);
  }

  tempoSwitch.update();
  if (tempoSwitch.fallingEdge()) {
    bool value = !voices.getFilterLfoMidiClockSync();
    midiCCOut(CCfilterLFOMidiClkSync, value);
    myControlChange(midiChannel, CCfilterLFOMidiClkSync, value);
  }

  saveButton.update();
  if (saveButton.read() == LOW && saveButton.duration() > HOLD_DURATION) {
    switch (state) {
      case PARAMETER:
      case PATCH:
        state = DELETE;
        saveButton.write(HIGH); //Come out of this state
        del = true;             //Hack
        break;
    }
  }
  else if (saveButton.risingEdge()) {
    if (!del) {
      switch (state) {
        case PARAMETER:
          if (patches.size() < PATCHES_LIMIT)  {
            resetPatchesOrdering(); //Reset order of patches from first patch
            patches.push({patches.size() + 1, INITPATCHNAME});
            state = SAVE;
          }
          break;
        case SAVE:
          //Save as new patch with INITIALPATCH name or overwrite existing keeping name - bypassing patch renaming
          patchName = patches.last().patchName;
          state = PATCH;
          savePatch(String(patches.last().patchNo).c_str(), getCurrentPatchData());
          showPatchPage(patches.last().patchNo, patches.last().patchName);
          patchNo = patches.last().patchNo;
          loadPatches(); //Get rid of pushed patch if it wasn't saved
          setPatchesOrdering(patchNo);
          renamedPatch = "";
          state = PARAMETER;
          break;
        case PATCHNAMING:
          if (renamedPatch.length() > 0) patchName = renamedPatch;//Prevent empty strings
          state = PATCH;
          savePatch(String(patches.last().patchNo).c_str(), getCurrentPatchData());
          showPatchPage(patches.last().patchNo, patchName);
          patchNo = patches.last().patchNo;
          loadPatches(); //Get rid of pushed patch if it wasn't saved
          setPatchesOrdering(patchNo);
          renamedPatch = "";
          state = PARAMETER;
          break;
      }
    } else {
      del = false;
    }
  }

  settingsButton.update();
  if (settingsButton.read() == LOW && settingsButton.duration() > HOLD_DURATION) {
    //If recall held, set current patch to match current hardware state
    //Reinitialise all hardware values to force them to be re-read if different
    state = REINITIALISE;
    reinitialiseToPanel();
    settingsButton.write(HIGH); //Come out of this state
    reini = true;           //Hack
  } else if (settingsButton.risingEdge())  {
    //cannot be fallingEdge because holding button won't work
    if (!reini) {
      switch (state) {
        case PARAMETER:
          settingsValueIndex = getCurrentIndex(settingsOptions.first().currentIndex);
          showSettingsPage(settingsOptions.first().option, settingsOptions.first().value[settingsValueIndex], SETTINGS);
          state = SETTINGS;
          break;
        case SETTINGS:
          settingsOptions.push(settingsOptions.shift());
          settingsValueIndex = getCurrentIndex(settingsOptions.first().currentIndex);
          showSettingsPage(settingsOptions.first().option, settingsOptions.first().value[settingsValueIndex], SETTINGS);
        case SETTINGSVALUE:
          //Same as pushing Recall - store current settings item and go back to options
          settingsHandler(settingsOptions.first().value[settingsValueIndex], settingsOptions.first().handler);
          showSettingsPage(settingsOptions.first().option, settingsOptions.first().value[settingsValueIndex], SETTINGS);
          state = SETTINGS;
          break;
      }
    } else {
      reini = false;
    }
  }

  backButton.update();
  if (backButton.read() == LOW && backButton.duration() > HOLD_DURATION) {
    //If Back button held, Panic - all notes off
    voices.allNotesOff();
    voices.closeEnvelopes();
    backButton.write(HIGH); //Come out of this state
    panic = true;           //Hack
  }
  else if (backButton.risingEdge())  {
    //cannot be fallingEdge because holding button won't work
    if (!panic) {
      switch (state) {
        case RECALL:
          setPatchesOrdering(patchNo);
          state = PARAMETER;
          break;
        case SAVE:
          renamedPatch = "";
          state = PARAMETER;
          loadPatches();//Remove patch that was to be saved
          setPatchesOrdering(patchNo);
          break;
        case PATCHNAMING:
          charIndex = 0;
          renamedPatch = "";
          state = SAVE;
          break;
        case DELETE:
          setPatchesOrdering(patchNo);
          state = PARAMETER;
          break;
        case SETTINGS:
          state = PARAMETER;
          break;
        case SETTINGSVALUE:
          settingsValueIndex = getCurrentIndex(settingsOptions.first().currentIndex);
          showSettingsPage(settingsOptions.first().option, settingsOptions.first().value[settingsValueIndex], SETTINGS);
          state = SETTINGS;
          break;
      }
    } else {
      panic = false;
    }
  }

  //Encoder switch
  recallButton.update();
  if (recallButton.read() == LOW && recallButton.duration() > HOLD_DURATION) {
    //If Recall button held, return to current patch setting
    //which clears any changes made
    state = PATCH;
    //Recall the current patch
    patchNo = patches.first().patchNo;
    recallPatch(patchNo);
    state = PARAMETER;
    recallButton.write(HIGH); //Come out of this state
    recall = true;            //Hack
  } else if (recallButton.risingEdge()) {
    if (!recall) {
      switch (state) {
        case PARAMETER:
          state = RECALL;//show patch list
          break;
        case RECALL:
          state = PATCH;
          //Recall the current patch
          patchNo = patches.first().patchNo;
          recallPatch(patchNo);
          state = PARAMETER;
          break;
        case SAVE:
          showRenamingPage(patches.last().patchName);
          patchName  = patches.last().patchName;
          state = PATCHNAMING;
          break;
        case PATCHNAMING:
          if (renamedPatch.length() < 12) //actually 12 chars
          {
            renamedPatch.concat(String(currentCharacter));
            charIndex = 0;
            currentCharacter = CHARACTERS[charIndex];
            showRenamingPage(renamedPatch);
          }
          break;
        case DELETE:
          //Don't delete final patch
          if (patches.size() > 1) {
            state = DELETEMSG;
            patchNo = patches.first().patchNo;//PatchNo to delete from SD card
            patches.shift();//Remove patch from circular buffer
            deletePatch(String(patchNo).c_str());//Delete from SD card
            loadPatches();//Repopulate circular buffer to start from lowest Patch No
            renumberPatchesOnSD();
            loadPatches();//Repopulate circular buffer again after delete
            patchNo = patches.first().patchNo;//Go back to 1
            recallPatch(patchNo);//Load first patch
          }
          state = PARAMETER;
          break;
        case SETTINGS:
          //Choose this option and allow value choice
          settingsValueIndex = getCurrentIndex(settingsOptions.first().currentIndex);
          showSettingsPage(settingsOptions.first().option, settingsOptions.first().value[settingsValueIndex], SETTINGSVALUE);
          state = SETTINGSVALUE;
          break;
        case SETTINGSVALUE:
          //Store current settings item and go back to options
          settingsHandler(settingsOptions.first().value[settingsValueIndex], settingsOptions.first().handler);
          showSettingsPage(settingsOptions.first().option, settingsOptions.first().value[settingsValueIndex], SETTINGS);
          state = SETTINGS;
          break;
      }
    } else {
      recall = false;
    }
  }
}

FLASHMEM void reinitialiseToPanel() {
  //This sets the current patch to be the same as the current hardware panel state - all the pots
  //The four button controls stay the same state
  //This reinialises the previous hardware values to force a re-read
  muxInput = 0;
  for (int i = 0; i < MUXCHANNELS; i++) {
    mux1ValuesPrev[i] = RE_READ;
    mux2ValuesPrev[i] = RE_READ;
  }
  volumePrevious = RE_READ;
  patchName = INITPATCHNAME;
  showPatchPage("Initial", "Panel Settings");
}

void checkEncoder() {
  //Encoder works with relative inc and dec values
  //Detent encoder goes up in 4 steps, hence +/-3
  long encRead = encoder.read();
  if ((encCW && encRead > encPrevious + 3) || (!encCW && encRead < encPrevious - 3) )  {
    switch (state) {
      case PARAMETER:
        state = PATCH;
        patches.push(patches.shift());
        patchNo = patches.first().patchNo;
        recallPatch(patchNo);
        state = PARAMETER;
        break;
      case RECALL:
        patches.push(patches.shift());
        break;
      case SAVE:
        patches.push(patches.shift());
        break;
      case PATCHNAMING:
        if (charIndex == TOTALCHARS) charIndex = 0;//Wrap around
        currentCharacter = CHARACTERS[charIndex++];
        showRenamingPage(renamedPatch + currentCharacter);
        break;
      case DELETE:
        patches.push(patches.shift());
        break;
      case SETTINGS:
        settingsOptions.push(settingsOptions.shift());
        settingsValueIndex = getCurrentIndex(settingsOptions.first().currentIndex);
        showSettingsPage(settingsOptions.first().option, settingsOptions.first().value[settingsValueIndex] , SETTINGS);
        break;
      case SETTINGSVALUE:
        if (strcmp(settingsOptions.first().value[settingsValueIndex + 1],"\0") !=0)
          showSettingsPage(settingsOptions.first().option, settingsOptions.first().value[++settingsValueIndex], SETTINGSVALUE);
        break;
    }
    encPrevious = encRead;
  } else if ((encCW && encRead < encPrevious - 3) || (!encCW && encRead > encPrevious + 3)) {
    switch (state) {
      case PARAMETER:
        state = PATCH;
        patches.unshift(patches.pop());
        patchNo = patches.first().patchNo;
        recallPatch(patchNo);
        state = PARAMETER;
        break;
      case RECALL:
        patches.unshift(patches.pop());
        break;
      case SAVE:
        patches.unshift(patches.pop());
        break;
      case PATCHNAMING:
        if (charIndex == -1)
          charIndex = TOTALCHARS - 1;
        currentCharacter = CHARACTERS[charIndex--];
        showRenamingPage(renamedPatch + currentCharacter);
        break;
      case DELETE:
        patches.unshift(patches.pop());
        break;
      case SETTINGS:
        settingsOptions.unshift(settingsOptions.pop());
        settingsValueIndex = getCurrentIndex(settingsOptions.first().currentIndex);
        showSettingsPage(settingsOptions.first().option, settingsOptions.first().value[settingsValueIndex], SETTINGS);
        break;
      case SETTINGSVALUE:
        if (settingsValueIndex > 0)
          showSettingsPage(settingsOptions.first().option, settingsOptions.first().value[--settingsValueIndex], SETTINGSVALUE);
        break;
    }
    encPrevious = encRead;
  }
}

void midiCCOut(byte cc, byte value) {
  if (midiOutCh > 0) {
    usbMIDI.sendControlChange(cc, value, midiOutCh);
    midi1.sendControlChange(cc, value, midiOutCh);
    if (MIDIThru == midi::Thru::Off) MIDI.sendControlChange(cc, value, midiOutCh); //MIDI DIN is set to Out
  }
}

void CPUMonitor() {
  Serial.print(F(" CPU:"));
  Serial.print(AudioProcessorUsage());
  Serial.print(F(" ("));
  Serial.print(AudioProcessorUsageMax());
  Serial.print(F(")"));
  Serial.print(F("  MEM:"));
  Serial.println(AudioMemoryUsageMax());
  delayMicroseconds(500);
}

void loop() {
  //USB HOST MIDI Class Compliant
  myusb.Task();
  midi1.read(midiChannel);
  //USB Client MIDI
  usbMIDI.read(midiChannel);
  //MIDI 5 Pin DIN
  MIDI.read(midiChannel);
  checkMux();
  checkSwitches();
  checkEncoder();
  //CPUMonitor();
}
