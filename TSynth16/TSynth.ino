/*
  ElectroTechnique TSynth - Firmware Rev 1.2
  TEENSY 4.1 - 16 VOICES

  Arduino IDE Tools Settings:
    Board: "Teensy4.1"
    USB Type: "Serial + MIDI + Audio"
    CPU Speed: "600MHz"
    Optimize: "Faster"

  Performance Tests   CPU  Mem
  600MHz Faster       71   104

  Includes code by:
    Dave Benn - Handling MUXs, a few other bits and original inspiration  https://www.notesandvolts.com/2019/01/teensy-synth-part-10-hardware.html
    Alexander Davis - Stereo ensemble chorus effect https://github.com/quarterturn/teensy3-ensemble-chorus
    Mark Tillotson - Special thanks for finally band-limiting the waveforms in the Audio Library

  Additional libraries:
    Agileware CircularBuffer available in Arduino libraries manager
    Replacement files are in the Modified Libraries folder and need to be placed in the teensy Audio folder.
*/
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
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
#include "Settings.h"

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

unsigned int state = PARAMETER;

const int16_t  WAVEFORM_PARABOLIC = 103;
const int16_t WAVEFORM_HARMONIC = 104;

struct VoiceAndNote {
  int note;
  long timeOn;
  int voiceOn;
};

struct VoiceAndNote voices[NO_OF_VOICES] = {{ -1, 0, 0}, { -1, 0, 0}, { -1, 0, 0}, { -1, 0, 0}, { -1, 0, 0}, { -1, 0, 0}, { -1, 0, 0}, { -1, 0, 0}, { -1, 0, 0}, { -1, 0, 0}, { -1, 0, 0}, { -1, 0, 0}, { -1, 0, 0}, { -1, 0, 0}, { -1, 0, 0}, { -1, 0, 0}};

#include "ST7735Display.h"

boolean cardStatus = false;

//USB HOST MIDI Class Compliant
USBHost myusb;
USBHub hub1(myusb);
USBHub hub2(myusb);
MIDIDevice midi1(myusb);

//MIDI 5 Pin DIN
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

int prevNote = 48;//This is for glide to use previous note to glide from
float previousMillis = millis(); //For MIDI Clk Sync

int16_t count = 0;//For MIDI Clk Sync
int16_t patchNo = 1;//Current patch no
int voiceToReturn = -1; //Initialise
long earliestTime = millis(); //For voice allocation - initialise to now

void setup() {
  setupDisplay();
  setUpSettings();
  setupHardware();

  AudioMemory(108);
  sgtl5000_1.enable();
  sgtl5000_1.volume(SGTL_MAXVOLUME * 0.5f); //Headphones - do not initialise to maximum, but this is re-read

  cardStatus = SD.begin(BUILTIN_SDCARD);
  if (cardStatus) {
    Serial.println("SD card is connected");
    //Get patch numbers and names from SD card
    loadPatches();
    if (patches.size() == 0) {
      //save an initialised patch to SD card
      savePatch("1", INITPATCH);
      loadPatches();
    }
  }
  else {
    Serial.println("SD card is not connected or unusable");
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
  Serial.println("USB HOST MIDI Class Compliant Listening");

  //USB Client MIDI
  usbMIDI.setHandleControlChange(myControlChange);
  usbMIDI.setHandleNoteOff(myNoteOff);
  usbMIDI.setHandleNoteOn(myNoteOn);
  usbMIDI.setHandlePitchChange(myPitchBend);
  usbMIDI.setHandleProgramChange(myProgramChange);
  usbMIDI.setHandleClock(myMIDIClock);
  usbMIDI.setHandleStart(myMIDIClockStart);
  usbMIDI.setHandleStop(myMIDIClockStop);
  Serial.println("USB Client MIDI Listening");

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
  Serial.println("MIDI In DIN Listening");

  constant1Dc.amplitude(ONE);

  noiseMixer.gain(0, ONE);
  noiseMixer.gain(1, ONE);
  noiseMixer.gain(2, 0);
  noiseMixer.gain(3, 0);

  voiceMixer1.gain(0, VOICEMIXERLEVEL);
  voiceMixer1.gain(1, VOICEMIXERLEVEL);
  voiceMixer1.gain(2, VOICEMIXERLEVEL);
  voiceMixer1.gain(3, VOICEMIXERLEVEL);

  voiceMixer2.gain(0, VOICEMIXERLEVEL);
  voiceMixer2.gain(1, VOICEMIXERLEVEL);
  voiceMixer2.gain(2, VOICEMIXERLEVEL);
  voiceMixer2.gain(3, VOICEMIXERLEVEL);

  voiceMixer3.gain(0, VOICEMIXERLEVEL);
  voiceMixer3.gain(1, VOICEMIXERLEVEL);
  voiceMixer3.gain(2, VOICEMIXERLEVEL);
  voiceMixer3.gain(3, VOICEMIXERLEVEL);

  voiceMixer4.gain(0, VOICEMIXERLEVEL);
  voiceMixer4.gain(1, VOICEMIXERLEVEL);
  voiceMixer4.gain(2, VOICEMIXERLEVEL);
  voiceMixer4.gain(3, VOICEMIXERLEVEL);

  voiceMixerM.gain(0, 0.25f);
  voiceMixerM.gain(1, 0.25f);
  voiceMixerM.gain(2, 0.25f);
  voiceMixerM.gain(3, 0.25f);

  pwmLfo.amplitude(ONE);
  pwmLfo.begin(PWMWAVEFORM);

  waveformMod1a.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod1a.begin(ONE, 440.0f, oscWaveformA);

  waveformMod1b.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod1b.begin(ONE, 440.0f, oscWaveformB);

  waveformMod2a.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod2a.begin(ONE, 440.0f, oscWaveformA);

  waveformMod2b.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod2b.begin(ONE, 440.0f, oscWaveformB);

  waveformMod3a.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod3a.begin(ONE, 440.0f, oscWaveformA);

  waveformMod3b.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod3b.begin(ONE, 440.0f, oscWaveformB);

  waveformMod4a.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod4a.begin(ONE, 440.0f, oscWaveformA);

  waveformMod4b.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod4b.begin(ONE, 440.0f, oscWaveformB);

  waveformMod5a.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod5a.begin(ONE, 440.0f, oscWaveformA);

  waveformMod5b.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod5b.begin(ONE, 440.0f, oscWaveformB);

  waveformMod6a.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod6a.begin(ONE, 440.0f, oscWaveformA);

  waveformMod6b.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod6b.begin(ONE, 440.0f, oscWaveformB);

  waveformMod7a.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod7a.begin(ONE, 440.0f, oscWaveformA);

  waveformMod7b.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod7b.begin(ONE, 440.0f, oscWaveformB);

  waveformMod8a.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod8a.begin(ONE, 440.0f, oscWaveformA);

  waveformMod8b.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod8b.begin(ONE, 440.0f, oscWaveformB);

  waveformMod9a.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod9a.begin(ONE, 440.0f, oscWaveformA);

  waveformMod9b.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod9b.begin(ONE, 440.0f, oscWaveformB);

  waveformMod10a.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod10a.begin(ONE, 440.0f, oscWaveformA);

  waveformMod10b.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod10b.begin(ONE, 440.0f, oscWaveformB);

  waveformMod11a.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod11a.begin(ONE, 440.0f, oscWaveformA);

  waveformMod11b.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod11b.begin(ONE, 440.0f, oscWaveformB);

  waveformMod12a.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod12a.begin(ONE, 440.0f, oscWaveformA);

  waveformMod12b.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod12b.begin(ONE, 440.0f, oscWaveformB);

  waveformMod13a.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod13a.begin(ONE, 440.0f, oscWaveformA);

  waveformMod13b.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod13b.begin(ONE, 440.0f, oscWaveformB);

  waveformMod14a.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod14a.begin(ONE, 440.0f, oscWaveformA);

  waveformMod14b.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod14b.begin(ONE, 440.0f, oscWaveformB);

  waveformMod15a.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod15a.begin(ONE, 440.0f, oscWaveformA);

  waveformMod15b.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod15b.begin(ONE, 440.0f, oscWaveformB);

  waveformMod16a.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod16a.begin(ONE, 440.0f, oscWaveformA);

  waveformMod16b.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod16b.begin(ONE, 440.0f, oscWaveformB);

  //Arbitary waveform needs initialising to something
  loadArbWaveformA(PARABOLIC_WAVE);
  loadArbWaveformB(PARABOLIC_WAVE);

  waveformMixer1.gain(2, ONE); //Noise
  waveformMixer1.gain(3, 0);           //Osc FX
  waveformMixer2.gain(2, ONE); //Noise
  waveformMixer2.gain(3, 0);           //Osc FX
  waveformMixer3.gain(2, ONE); //Noise
  waveformMixer3.gain(3, 0);           //Osc FX
  waveformMixer4.gain(2, ONE); //Noise
  waveformMixer4.gain(3, 0);           //Osc FX
  waveformMixer5.gain(2, ONE); //Noise
  waveformMixer5.gain(3, 0);           //Osc FX
  waveformMixer6.gain(2, ONE); //Noise
  waveformMixer6.gain(3, 0);           //Osc FX
  waveformMixer7.gain(2, ONE); //Noise
  waveformMixer7.gain(3, 0);           //Osc FX
  waveformMixer8.gain(2, ONE); //Noise
  waveformMixer8.gain(3, 0);           //Osc FX
  waveformMixer9.gain(2, ONE); //Noise
  waveformMixer9.gain(3, 0);           //Osc FX
  waveformMixer10.gain(2, ONE); //Noise
  waveformMixer10.gain(3, 0);           //Osc FX
  waveformMixer11.gain(2, ONE); //Noise
  waveformMixer11.gain(3, 0);           //Osc FX
  waveformMixer12.gain(2, ONE); //Noise
  waveformMixer12.gain(3, 0);           //Osc FX
  waveformMixer13.gain(2, ONE); //Noise
  waveformMixer13.gain(3, 0);           //Osc FX
  waveformMixer14.gain(2, ONE); //Noise
  waveformMixer14.gain(3, 0);           //Osc FX
  waveformMixer15.gain(2, ONE); //Noise
  waveformMixer15.gain(3, 0);           //Osc FX
  waveformMixer16.gain(2, ONE); //Noise
  waveformMixer16.gain(3, 0);           //Osc FX

  filterModMixer1.gain(1, ONE); //LFO
  filterModMixer1.gain(3, 0);           //Not used
  filterModMixer2.gain(1, ONE); //LFO
  filterModMixer2.gain(3, 0);           //Not used
  filterModMixer3.gain(1, ONE); //LFO
  filterModMixer3.gain(3, 0);           //Not used
  filterModMixer4.gain(1, ONE); //LFO
  filterModMixer4.gain(3, 0);           //Not used
  filterModMixer5.gain(1, ONE); //LFO
  filterModMixer5.gain(3, 0);           //Not used
  filterModMixer6.gain(1, ONE); //LFO
  filterModMixer6.gain(3, 0);           //Not used
  filterModMixer7.gain(1, ONE); //LFO
  filterModMixer7.gain(3, 0);           //Not used
  filterModMixer8.gain(1, ONE); //LFO
  filterModMixer8.gain(3, 0);           //Not used
  filterModMixer9.gain(1, ONE); //LFO
  filterModMixer9.gain(3, 0);           //Not used
  filterModMixer10.gain(1, ONE); //LFO
  filterModMixer10.gain(3, 0);           //Not used
  filterModMixer11.gain(1, ONE); //LFO
  filterModMixer11.gain(3, 0);           //Not used
  filterModMixer12.gain(1, ONE); //LFO
  filterModMixer12.gain(3, 0);           //Not used
  filterModMixer13.gain(1, ONE); //LFO
  filterModMixer13.gain(3, 0);           //Not used
  filterModMixer14.gain(1, ONE); //LFO
  filterModMixer14.gain(3, 0);           //Not used
  filterModMixer15.gain(1, ONE); //LFO
  filterModMixer15.gain(3, 0);           //Not used
  filterModMixer16.gain(1, ONE); //LFO
  filterModMixer16.gain(3, 0);           //Not used

  filterMixer1.gain(3, 0); //Not used
  filterMixer2.gain(3, 0); //Not used
  filterMixer3.gain(3, 0); //Not used
  filterMixer4.gain(3, 0); //Not used
  filterMixer5.gain(3, 0); //Not used
  filterMixer6.gain(3, 0); //Not used
  filterMixer7.gain(3, 0); //Not used
  filterMixer8.gain(3, 0); //Not used
  filterMixer9.gain(3, 0); //Not used
  filterMixer10.gain(3, 0); //Not used
  filterMixer11.gain(3, 0); //Not used
  filterMixer12.gain(3, 0); //Not used
  filterMixer13.gain(3, 0); //Not used
  filterMixer14.gain(3, 0); //Not used
  filterMixer15.gain(3, 0); //Not used
  filterMixer16.gain(3, 0); //Not used

  //This removes dc offset (mostly from unison pulse waves) before the ensemble effect
  dcOffsetFilter.octaveControl(1.0f);
  dcOffsetFilter.frequency(12.0f);//Lower values will give clicks on note on/off

  ensemble.lfoRate(fxAmt);

  effectMixerL.gain(2, 0);//Not used
  effectMixerL.gain(3, 0);//Not used
  effectMixerR.gain(2, 0);//Not used
  effectMixerR.gain(3, 0);//Not used

  volumePrevious = RE_READ; //Force volume control to be read and set to current

  //Read Key Tracking from EEPROM, this can be set individually by each patch.
  keytrackingAmount = getKeyTracking();

  //Read Pitch Bend Range from EEPROM
  pitchBendRange = getPitchBendRange();

  //Read Mod Wheel Depth from EEPROM
  modWheelDepth = getModWheelDepth();

  //Read Encoder Direction from EEPROM
  encCW = getEncoderDir();

  recallPatch(patchNo); //Load first patch

  //threads.addThread(loopThread);
}

void myNoteOn(byte channel, byte note, byte velocity) {
  //Check for out of range notes
  if (note + oscPitchA < 0 || note + oscPitchA > 127 || note + oscPitchB < 0 || note + oscPitchB > 127)
    return;

  if (oscLfoRetrig == 1) {
    pitchLfo.sync();
  }
  if (filterLfoRetrig == 1) {
    filterLfo.sync();
  }

  if (unison == 0) {
    switch (getVoiceNo(-1))  {
      case 1:
        keytracking1.amplitude(note * DIV127 * keytrackingAmount);
        voices[0].note = note;
        voices[0].timeOn = millis();
        updateVoice1();
        filterEnvelope1.noteOn();
        ampEnvelope1.noteOn();
        voices[0].voiceOn = 1;
        if (glideSpeed > 0 && note != prevNote)
        {
          glide1.amplitude((prevNote - note) * DIV12);   //Set glide to previous note frequency (limited to 1 octave max)
          glide1.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
        }
        break;
      case 2:
        keytracking2.amplitude(note * DIV127 * keytrackingAmount);
        voices[1].note = note;
        voices[1].timeOn = millis();
        updateVoice2();
        filterEnvelope2.noteOn();
        ampEnvelope2.noteOn();
        voices[1].voiceOn = 1;
        if (glideSpeed > 0 && note != prevNote)
        {
          glide2.amplitude((prevNote - note) * DIV12);   //Set glide to previous note frequency (limited to 1 octave max)
          glide2.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
        }
        break;
      case 3:
        keytracking3.amplitude(note * DIV127 * keytrackingAmount);
        voices[2].note = note;
        voices[2].timeOn = millis();
        updateVoice3();
        filterEnvelope3.noteOn();
        ampEnvelope3.noteOn();
        voices[2].voiceOn = 1;
        if (glideSpeed > 0 && note != prevNote)
        {
          glide3.amplitude((prevNote - note) * DIV12);   //Set glide to previous note frequency (limited to 1 octave max)
          glide3.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
        }
        break;
      case 4:
        keytracking4.amplitude(note * DIV127 * keytrackingAmount);
        voices[3].note = note;
        voices[3].timeOn = millis();
        updateVoice4();
        filterEnvelope4.noteOn();
        ampEnvelope4.noteOn();
        voices[3].voiceOn = 1;
        if (glideSpeed > 0 && note != prevNote)
        {
          glide4.amplitude((prevNote - note) * DIV12);   //Set glide to previous note frequency (limited to 1 octave max)
          glide4.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
        }
        break;
      case 5:
        keytracking5.amplitude(note * DIV127 * keytrackingAmount);
        voices[4].note = note;
        voices[4].timeOn = millis();
        updateVoice5();
        filterEnvelope5.noteOn();
        ampEnvelope5.noteOn();
        voices[4].voiceOn = 1;
        if (glideSpeed > 0 && note != prevNote)
        {
          glide5.amplitude((prevNote - note) * DIV12);   //Set glide to previous note frequency (limited to 1 octave max)
          glide5.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
        }
        break;
      case 6:
        keytracking6.amplitude(note * DIV127 * keytrackingAmount);
        voices[5].note = note;
        voices[5].timeOn = millis();
        updateVoice6();
        filterEnvelope6.noteOn();
        ampEnvelope6.noteOn();
        voices[5].voiceOn = 1;
        if (glideSpeed > 0 && note != prevNote)
        {
          glide6.amplitude((prevNote - note) * DIV12);   //Set glide to previous note frequency (limited to 1 octave max)
          glide6.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
        }
        break;
      case 7:
        keytracking7.amplitude(note * DIV127 * keytrackingAmount);
        voices[6].note = note;
        voices[6].timeOn = millis();
        updateVoice7();
        filterEnvelope7.noteOn();
        ampEnvelope7.noteOn();
        voices[6].voiceOn = 1;
        if (glideSpeed > 0 && note != prevNote)
        {
          glide7.amplitude((prevNote - note) * DIV12);   //Set glide to previous note frequency (limited to 1 octave max)
          glide7.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
        }
        break;
      case 8:
        keytracking8.amplitude(note * DIV127 * keytrackingAmount);
        voices[7].note = note;
        voices[7].timeOn = millis();
        updateVoice8();
        filterEnvelope8.noteOn();
        ampEnvelope8.noteOn();
        voices[7].voiceOn = 1;
        if (glideSpeed > 0 && note != prevNote)
        {
          glide8.amplitude((prevNote - note) * DIV12);   //Set glide to previous note frequency (limited to 1 octave max)
          glide8.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
        }
        break;
      case 9:
        keytracking9.amplitude(note * DIV127 * keytrackingAmount);
        voices[8].note = note;
        voices[8].timeOn = millis();
        updateVoice9();
        filterEnvelope9.noteOn();
        ampEnvelope9.noteOn();
        voices[8].voiceOn = 1;
        if (glideSpeed > 0 && note != prevNote)
        {
          glide9.amplitude((prevNote - note) * DIV12);   //Set glide to previous note frequency (limited to 1 octave max)
          glide9.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
        }
        break;
      case 10:
        keytracking10.amplitude(note * DIV127 * keytrackingAmount);
        voices[9].note = note;
        voices[9].timeOn = millis();
        updateVoice10();
        filterEnvelope10.noteOn();
        ampEnvelope10.noteOn();
        voices[9].voiceOn = 1;
        if (glideSpeed > 0 && note != prevNote)
        {
          glide10.amplitude((prevNote - note) * DIV12);   //Set glide to previous note frequency (limited to 1 octave max)
          glide10.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
        }
        break;
      case 11:
        keytracking11.amplitude(note * DIV127 * keytrackingAmount);
        voices[10].note = note;
        voices[10].timeOn = millis();
        updateVoice11();
        filterEnvelope11.noteOn();
        ampEnvelope11.noteOn();
        voices[10].voiceOn = 1;
        if (glideSpeed > 0 && note != prevNote)
        {
          glide11.amplitude((prevNote - note) * DIV12);   //Set glide to previous note frequency (limited to 1 octave max)
          glide11.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
        }
        break;
      case 12:
        keytracking12.amplitude(note * DIV127 * keytrackingAmount);
        voices[11].note = note;
        voices[11].timeOn = millis();
        updateVoice12();
        filterEnvelope12.noteOn();
        ampEnvelope12.noteOn();
        voices[11].voiceOn = 1;
        if (glideSpeed > 0 && note != prevNote)
        {
          glide12.amplitude((prevNote - note) * DIV12);   //Set glide to previous note frequency (limited to 1 octave max)
          glide12.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
        }
        break;
      case 13:
        keytracking13.amplitude(note * DIV127 * keytrackingAmount);
        voices[12].note = note;
        voices[12].timeOn = millis();
        updateVoice13();
        filterEnvelope13.noteOn();
        ampEnvelope13.noteOn();
        voices[12].voiceOn = 1;
        if (glideSpeed > 0 && note != prevNote)
        {
          glide13.amplitude((prevNote - note) * DIV12);   //Set glide to previous note frequency (limited to 1 octave max)
          glide13.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
        }
        break;
      case 14:
        keytracking14.amplitude(note * DIV127 * keytrackingAmount);
        voices[13].note = note;
        voices[13].timeOn = millis();
        updateVoice14();
        filterEnvelope14.noteOn();
        ampEnvelope14.noteOn();
        voices[13].voiceOn = 1;
        if (glideSpeed > 0 && note != prevNote)
        {
          glide14.amplitude((prevNote - note) * DIV12);   //Set glide to previous note frequency (limited to 1 octave max)
          glide14.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
        }
        break;
      case 15:
        keytracking15.amplitude(note * DIV127 * keytrackingAmount);
        voices[14].note = note;
        voices[14].timeOn = millis();
        updateVoice15();
        filterEnvelope15.noteOn();
        ampEnvelope15.noteOn();
        voices[14].voiceOn = 1;
        if (glideSpeed > 0 && note != prevNote)
        {
          glide15.amplitude((prevNote - note) * DIV12);   //Set glide to previous note frequency (limited to 1 octave max)
          glide15.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
        }
        break;
      case 16:
        keytracking16.amplitude(note * DIV127 * keytrackingAmount);
        voices[15].note = note;
        voices[15].timeOn = millis();
        updateVoice16();
        filterEnvelope16.noteOn();
        ampEnvelope16.noteOn();
        voices[15].voiceOn = 1;
        if (glideSpeed > 0 && note != prevNote)
        {
          glide16.amplitude((prevNote - note) * DIV12);   //Set glide to previous note frequency (limited to 1 octave max)
          glide16.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
        }
        break;
    }
  }
  else  {
    //UNISON MODE
    keytracking1.amplitude(note * DIV127 * keytrackingAmount);
    keytracking2.amplitude(note * DIV127 * keytrackingAmount);
    keytracking3.amplitude(note * DIV127 * keytrackingAmount);
    keytracking4.amplitude(note * DIV127 * keytrackingAmount);
    keytracking5.amplitude(note * DIV127 * keytrackingAmount);
    keytracking6.amplitude(note * DIV127 * keytrackingAmount);
    keytracking7.amplitude(note * DIV127 * keytrackingAmount);
    keytracking8.amplitude(note * DIV127 * keytrackingAmount);
    keytracking9.amplitude(note * DIV127 * keytrackingAmount);
    keytracking10.amplitude(note * DIV127 * keytrackingAmount);
    keytracking11.amplitude(note * DIV127 * keytrackingAmount);
    keytracking12.amplitude(note * DIV127 * keytrackingAmount);
    keytracking13.amplitude(note * DIV127 * keytrackingAmount);
    keytracking14.amplitude(note * DIV127 * keytrackingAmount);
    keytracking15.amplitude(note * DIV127 * keytrackingAmount);
    keytracking16.amplitude(note * DIV127 * keytrackingAmount);
    voices[0].note = note;
    voices[0].timeOn = millis();
    updateVoice1();
    voices[1].note = note;
    voices[1].timeOn = millis();
    updateVoice2();
    voices[2].note = note;
    voices[2].timeOn = millis();
    updateVoice3();
    voices[3].note = note;
    voices[3].timeOn = millis();
    updateVoice4();
    voices[4].note = note;
    voices[4].timeOn = millis();
    updateVoice5();
    voices[5].note = note;
    voices[5].timeOn = millis();
    updateVoice6();
    voices[6].note = note;
    voices[6].timeOn = millis();
    updateVoice7();
    voices[7].note = note;
    voices[7].timeOn = millis();
    updateVoice8();
    voices[8].note = note;
    voices[8].timeOn = millis();
    updateVoice9();
    voices[9].note = note;
    voices[9].timeOn = millis();
    updateVoice10();
    voices[10].note = note;
    voices[10].timeOn = millis();
    updateVoice11();
    voices[11].note = note;
    voices[11].timeOn = millis();
    updateVoice12();
    voices[12].note = note;
    voices[12].timeOn = millis();
    updateVoice13();
    voices[13].note = note;
    voices[13].timeOn = millis();
    updateVoice14();
    voices[14].note = note;
    voices[14].timeOn = millis();
    updateVoice15();
    voices[15].note = note;
    voices[15].timeOn = millis();
    updateVoice16();

    filterEnvelope1.noteOn();
    filterEnvelope2.noteOn();
    filterEnvelope3.noteOn();
    filterEnvelope4.noteOn();
    filterEnvelope5.noteOn();
    filterEnvelope6.noteOn();
    filterEnvelope7.noteOn();
    filterEnvelope8.noteOn();
    filterEnvelope9.noteOn();
    filterEnvelope10.noteOn();
    filterEnvelope11.noteOn();
    filterEnvelope12.noteOn();
    filterEnvelope13.noteOn();
    filterEnvelope14.noteOn();
    filterEnvelope15.noteOn();
    filterEnvelope16.noteOn();

    ampEnvelope1.noteOn();
    ampEnvelope2.noteOn();
    ampEnvelope3.noteOn();
    ampEnvelope4.noteOn();
    ampEnvelope5.noteOn();
    ampEnvelope6.noteOn();
    ampEnvelope7.noteOn();
    ampEnvelope8.noteOn();
    ampEnvelope9.noteOn();
    ampEnvelope10.noteOn();
    ampEnvelope11.noteOn();
    ampEnvelope12.noteOn();
    ampEnvelope13.noteOn();
    ampEnvelope14.noteOn();
    ampEnvelope15.noteOn();
    ampEnvelope16.noteOn();

    voices[0].voiceOn = 1;
    voices[1].voiceOn = 1;
    voices[2].voiceOn = 1;
    voices[3].voiceOn = 1;
    voices[4].voiceOn = 1;
    voices[5].voiceOn = 1;
    voices[6].voiceOn = 1;
    voices[7].voiceOn = 1;
    voices[8].voiceOn = 1;
    voices[9].voiceOn = 1;
    voices[10].voiceOn = 1;
    voices[11].voiceOn = 1;
    voices[12].voiceOn = 1;
    voices[13].voiceOn = 1;
    voices[14].voiceOn = 1;
    voices[15].voiceOn = 1;

    if (glideSpeed > 0 && note != prevNote) {
      glide1.amplitude((prevNote - note) * DIV12);   //Set glide to previous note frequency (limited to 1 octave max)
      glide1.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
      glide2.amplitude((prevNote - note) * DIV12);   //Set glide to previous note frequency (limited to 1 octave max)
      glide2.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
      glide3.amplitude((prevNote - note) * DIV12);   //Set glide to previous note frequency (limited to 1 octave max)
      glide3.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
      glide4.amplitude((prevNote - note) * DIV12);   //Set glide to previous note frequency (limited to 1 octave max)
      glide4.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
      glide5.amplitude((prevNote - note) * DIV12);   //Set glide to previous note frequency (limited to 1 octave max)
      glide5.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
      glide6.amplitude((prevNote - note) * DIV12);   //Set glide to previous note frequency (limited to 1 octave max)
      glide6.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
      glide7.amplitude((prevNote - note) * DIV12);   //Set glide to previous note frequency (limited to 1 octave max)
      glide7.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
      glide8.amplitude((prevNote - note) * DIV12);   //Set glide to previous note frequency (limited to 1 octave max)
      glide8.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
      glide9.amplitude((prevNote - note) * DIV12);   //Set glide to previous note frequency (limited to 1 octave max)
      glide9.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
      glide10.amplitude((prevNote - note) * DIV12);   //Set glide to previous note frequency (limited to 1 octave max)
      glide10.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
      glide11.amplitude((prevNote - note) * DIV12);   //Set glide to previous note frequency (limited to 1 octave max)
      glide11.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
      glide12.amplitude((prevNote - note) * DIV12);   //Set glide to previous note frequency (limited to 1 octave max)
      glide12.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
      glide13.amplitude((prevNote - note) * DIV12);   //Set glide to previous note frequency (limited to 1 octave max)
      glide13.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
      glide14.amplitude((prevNote - note) * DIV12);   //Set glide to previous note frequency (limited to 1 octave max)
      glide14.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
      glide15.amplitude((prevNote - note) * DIV12);   //Set glide to previous note frequency (limited to 1 octave max)
      glide15.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
      glide16.amplitude((prevNote - note) * DIV12);   //Set glide to previous note frequency (limited to 1 octave max)
      glide16.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
    }
  }
}

void myNoteOff(byte channel, byte note, byte velocity) {
  if (unison == 0) {
    switch (getVoiceNo(note)) {
      case 1:
        filterEnvelope1.noteOff();
        ampEnvelope1.noteOff();
        prevNote = voices[0].note;
        voices[0].voiceOn = 0;
        break;
      case 2:
        filterEnvelope2.noteOff();
        ampEnvelope2.noteOff();
        prevNote = voices[1].note;
        voices[1].voiceOn = 0;
        break;
      case 3:
        filterEnvelope3.noteOff();
        ampEnvelope3.noteOff();
        prevNote = voices[2].note;
        voices[2].voiceOn = 0;
        break;
      case 4:
        filterEnvelope4.noteOff();
        ampEnvelope4.noteOff();
        prevNote = voices[3].note;
        voices[3].voiceOn = 0;
        break;
      case 5:
        filterEnvelope5.noteOff();
        ampEnvelope5.noteOff();
        prevNote = voices[4].note;
        voices[4].voiceOn = 0;
        break;
      case 6:
        filterEnvelope6.noteOff();
        ampEnvelope6.noteOff();
        prevNote = voices[5].note;
        voices[5].voiceOn = 0;
        break;
      case 7:
        filterEnvelope7.noteOff();
        ampEnvelope7.noteOff();
        prevNote = voices[6].note;
        voices[6].voiceOn = 0;
        break;
      case 8:
        filterEnvelope8.noteOff();
        ampEnvelope8.noteOff();
        prevNote = voices[7].note;
        voices[7].voiceOn = 0;
        break;
      case 9:
        filterEnvelope9.noteOff();
        ampEnvelope9.noteOff();
        prevNote = voices[8].note;
        voices[8].voiceOn = 0;
        break;
      case 10:
        filterEnvelope10.noteOff();
        ampEnvelope10.noteOff();
        prevNote = voices[9].note;
        voices[9].voiceOn = 0;
        break;
      case 11:
        filterEnvelope11.noteOff();
        ampEnvelope11.noteOff();
        prevNote = voices[10].note;
        voices[10].voiceOn = 0;
        break;
      case 12:
        filterEnvelope12.noteOff();
        ampEnvelope12.noteOff();
        prevNote = voices[11].note;
        voices[11].voiceOn = 0;
        break;
      case 13:
        filterEnvelope13.noteOff();
        ampEnvelope13.noteOff();
        prevNote = voices[12].note;
        voices[12].voiceOn = 0;
        break;
      case 14:
        filterEnvelope14.noteOff();
        ampEnvelope14.noteOff();
        prevNote = voices[13].note;
        voices[13].voiceOn = 0;
        break;
      case 15:
        filterEnvelope15.noteOff();
        ampEnvelope15.noteOff();
        prevNote = voices[14].note;
        voices[14].voiceOn = 0;
        break;
      case 16:
        filterEnvelope16.noteOff();
        ampEnvelope16.noteOff();
        prevNote = voices[15].note;
        voices[15].voiceOn = 0;
        break;
    }
  }
  else {
    //UNISON MODE
    //If statement prevents the previous different note
    //ending the current note when released
    if (voices[0].note == note)allNotesOff();
    prevNote = note;
  }
}

void allNotesOff() {
  filterEnvelope1.noteOff();
  ampEnvelope1.noteOff();
  filterEnvelope2.noteOff();
  ampEnvelope2.noteOff();
  filterEnvelope3.noteOff();
  ampEnvelope3.noteOff();
  filterEnvelope4.noteOff();
  ampEnvelope4.noteOff();
  filterEnvelope5.noteOff();
  ampEnvelope5.noteOff();
  filterEnvelope6.noteOff();
  ampEnvelope6.noteOff();
  filterEnvelope7.noteOff();
  ampEnvelope7.noteOff();
  filterEnvelope8.noteOff();
  ampEnvelope8.noteOff();
  filterEnvelope9.noteOff();
  ampEnvelope9.noteOff();
  filterEnvelope10.noteOff();
  ampEnvelope10.noteOff();
  filterEnvelope11.noteOff();
  ampEnvelope11.noteOff();
  filterEnvelope12.noteOff();
  ampEnvelope12.noteOff();
  filterEnvelope13.noteOff();
  ampEnvelope13.noteOff();
  filterEnvelope14.noteOff();
  ampEnvelope14.noteOff();
  filterEnvelope15.noteOff();
  ampEnvelope15.noteOff();
  filterEnvelope16.noteOff();
  ampEnvelope16.noteOff();

  voices[0].voiceOn = 0;
  voices[1].voiceOn = 0;
  voices[2].voiceOn = 0;
  voices[3].voiceOn = 0;
  voices[4].voiceOn = 0;
  voices[5].voiceOn = 0;
  voices[6].voiceOn = 0;
  voices[7].voiceOn = 0;
  voices[8].voiceOn = 0;
  voices[9].voiceOn = 0;
  voices[10].voiceOn = 0;
  voices[11].voiceOn = 0;
  voices[12].voiceOn = 0;
  voices[13].voiceOn = 0;
  voices[14].voiceOn = 0;
  voices[15].voiceOn = 0;
}

int getVoiceNo(int note) {
  voiceToReturn = -1;      //Initialise
  earliestTime = millis(); //Initialise to now
  if (note == -1) {
    //NoteOn() - Get the oldest free voice (recent voices may be still on release stage)
    for (int i = 0; i < NO_OF_VOICES; i++)
    {
      if (voices[i].voiceOn == 0)
      {
        if (voices[i].timeOn < earliestTime)
        {
          earliestTime = voices[i].timeOn;
          voiceToReturn = i;
        }
      }
    }
    if (voiceToReturn == -1) {
      //No free voices, need to steal oldest sounding voice
      earliestTime = millis(); //Reinitialise
      for (int i = 0; i < NO_OF_VOICES; i++)
      {
        if (voices[i].timeOn < earliestTime)
        {
          earliestTime = voices[i].timeOn;
          voiceToReturn = i;
        }
      }
    }
    return voiceToReturn + 1;
  }
  else {
    //NoteOff() - Get voice number from note
    for (int i = 0; i < NO_OF_VOICES; i++)
    {
      if (voices[i].note == note && voices[i].voiceOn == 1)
      {
        return i + 1;
      }
    }
  }
  //Shouldn't get here, return voice 1
  return 1;
}

void updateVoice1() {

  waveformMod1a.frequency(NOTEFREQS[voices[0].note + oscPitchA]);
  if (unison == 1)
  {
    waveformMod1b.frequency(NOTEFREQS[voices[0].note + oscPitchB] * (detune + ((1 - detune) * 0.03f)));
  }
  else {
    waveformMod1b.frequency(NOTEFREQS[voices[0].note + oscPitchB] * detune);
  }
}

void updateVoice2() {

  if (unison == 1)
  {
    waveformMod2a.frequency(NOTEFREQS[voices[1].note + oscPitchA] * (detune + ((1 - detune) * 0.06f)));
    waveformMod2b.frequency(NOTEFREQS[voices[1].note + oscPitchB] * (detune + ((1 - detune) * 0.1f)));
  }
  else
  {
    waveformMod2a.frequency(NOTEFREQS[voices[1].note + oscPitchA]);
    waveformMod2b.frequency(NOTEFREQS[voices[1].note + oscPitchB] * detune);
  }
}

void updateVoice3() {

  if (unison == 1)
  {
    waveformMod3a.frequency(NOTEFREQS[voices[2].note + oscPitchA] * (detune + ((1 - detune) * 0.13f)));
    waveformMod3b.frequency(NOTEFREQS[voices[2].note + oscPitchB] * (detune + ((1 - detune) * 0.16f)));
  }
  else
  {
    waveformMod3a.frequency(NOTEFREQS[voices[2].note + oscPitchA]);
    waveformMod3b.frequency(NOTEFREQS[voices[2].note + oscPitchB] * detune);
  }
}
void updateVoice4() {

  if (unison == 1)
  {
    waveformMod4a.frequency(NOTEFREQS[voices[3].note + oscPitchA] * (detune + ((1 - detune) * 0.19f)));
    waveformMod4b.frequency(NOTEFREQS[voices[3].note + oscPitchB] * (detune + ((1 - detune) * 0.23f)));
  }
  else
  {
    waveformMod4a.frequency(NOTEFREQS[voices[3].note + oscPitchA]);
    waveformMod4b.frequency(NOTEFREQS[voices[3].note + oscPitchB] * detune);
  }
}

void updateVoice5() {

  if (unison == 1)
  {
    waveformMod5a.frequency(NOTEFREQS[voices[4].note + oscPitchA] * (detune + ((1 - detune) * 0.26f)));
    waveformMod5b.frequency(NOTEFREQS[voices[4].note + oscPitchB] * (detune + ((1 - detune) * 0.29f)));
  }
  else
  {
    waveformMod5a.frequency(NOTEFREQS[voices[4].note + oscPitchA]);
    waveformMod5b.frequency(NOTEFREQS[voices[4].note + oscPitchB] * detune);
  }
}

void updateVoice6() {

  if (unison == 1)
  {
    waveformMod6a.frequency(NOTEFREQS[voices[5].note + oscPitchA] * (detune + ((1 - detune) * 0.32f)));
    waveformMod6b.frequency(NOTEFREQS[voices[5].note + oscPitchB] * (detune + ((1 - detune) * 0.35f)));
  }
  else
  {
    waveformMod6a.frequency(NOTEFREQS[voices[5].note + oscPitchA]);
    waveformMod6b.frequency(NOTEFREQS[voices[5].note + oscPitchB] * detune);
  }

}

void updateVoice7() {

  if (unison == 1)
  {
    waveformMod7a.frequency(NOTEFREQS[voices[6].note + oscPitchA] * (detune + ((1 - detune) * 0.39f)));
    waveformMod7b.frequency(NOTEFREQS[voices[6].note + oscPitchB] * (detune + ((1 - detune) * 0.42f)));
  }
  else
  {
    waveformMod7a.frequency(NOTEFREQS[voices[6].note + oscPitchA]);
    waveformMod7b.frequency(NOTEFREQS[voices[6].note + oscPitchB] * detune);
  }
}

void updateVoice8() {

  if (unison == 1)
  {
    waveformMod8a.frequency(NOTEFREQS[voices[7].note + oscPitchA] * (detune + ((1 - detune) * 0.45f)));
    waveformMod8b.frequency(NOTEFREQS[voices[7].note + oscPitchB] * (detune + ((1 - detune) * 0.48f)));
  }
  else
  {
    waveformMod8a.frequency(NOTEFREQS[voices[7].note + oscPitchA]);
    waveformMod8b.frequency(NOTEFREQS[voices[7].note + oscPitchB] * detune);
  }
}

void updateVoice9() {

  if (unison == 1)
  {
    waveformMod9a.frequency(NOTEFREQS[voices[8].note + oscPitchA] * (detune + ((1 - detune) * 0.52f)));
    waveformMod9b.frequency(NOTEFREQS[voices[8].note + oscPitchB] * (detune + ((1 - detune) * 0.55f)));
  }
  else
  {
    waveformMod9a.frequency(NOTEFREQS[voices[8].note + oscPitchA]);
    waveformMod9b.frequency(NOTEFREQS[voices[8].note + oscPitchB] * detune);
  }
}

void updateVoice10() {

  if (unison == 1)
  {
    waveformMod10a.frequency(NOTEFREQS[voices[9].note + oscPitchA] * (detune + ((1 - detune) * 0.58f)));
    waveformMod10b.frequency(NOTEFREQS[voices[9].note + oscPitchB] * (detune + ((1 - detune) * 0.61f)));
  }
  else
  {
    waveformMod10a.frequency(NOTEFREQS[voices[9].note + oscPitchA]);
    waveformMod10b.frequency(NOTEFREQS[voices[9].note + oscPitchB] * detune);
  }
}

void updateVoice11() {

  if (unison == 1)
  {
    waveformMod11a.frequency(NOTEFREQS[voices[10].note + oscPitchA] * (detune + ((1 - detune) * 0.65f)));
    waveformMod11b.frequency(NOTEFREQS[voices[10].note + oscPitchB] * (detune + ((1 - detune) * 0.68f)));
  }
  else
  {
    waveformMod11a.frequency(NOTEFREQS[voices[10].note + oscPitchA]);
    waveformMod11b.frequency(NOTEFREQS[voices[10].note + oscPitchB] * detune);
  }
}

void updateVoice12() {

  if (unison == 1)
  {
    waveformMod12a.frequency(NOTEFREQS[voices[11].note + oscPitchA] * (detune + ((1 - detune) * 0.71f)));
    waveformMod12b.frequency(NOTEFREQS[voices[11].note + oscPitchB] * (detune + ((1 - detune) * 0.74f)));
  }
  else
  {
    waveformMod12a.frequency(NOTEFREQS[voices[11].note + oscPitchA]);
    waveformMod12b.frequency(NOTEFREQS[voices[11].note + oscPitchB] * detune);
  }
}

void updateVoice13() {

  if (unison == 1)
  {
    waveformMod13a.frequency(NOTEFREQS[voices[12].note + oscPitchA] * (detune + ((1 - detune) * 0.77f)));
    waveformMod13b.frequency(NOTEFREQS[voices[12].note + oscPitchB] * (detune + ((1 - detune) * 0.81f)));
  }
  else
  {
    waveformMod13a.frequency(NOTEFREQS[voices[12].note + oscPitchA]);
    waveformMod13b.frequency(NOTEFREQS[voices[12].note + oscPitchB] * detune);
  }
}

void updateVoice14() {

  if (unison == 1)
  {
    waveformMod14a.frequency(NOTEFREQS[voices[13].note + oscPitchA] * (detune + ((1 - detune) * 0.84f)));
    waveformMod14b.frequency(NOTEFREQS[voices[13].note + oscPitchB] * (detune + ((1 - detune) * 0.87f)));
  }
  else
  {
    waveformMod14a.frequency(NOTEFREQS[voices[13].note + oscPitchA]);
    waveformMod14b.frequency(NOTEFREQS[voices[13].note + oscPitchB] * detune);
  }
}

void updateVoice15() {

  if (unison == 1)
  {
    waveformMod15a.frequency(NOTEFREQS[voices[14].note + oscPitchA] * (detune + ((1 - detune) * 0.9f)));
    waveformMod15b.frequency(NOTEFREQS[voices[14].note + oscPitchB] * (detune + ((1 - detune) * 0.94f)));
  }
  else
  {
    waveformMod15a.frequency(NOTEFREQS[voices[14].note + oscPitchA]);
    waveformMod15b.frequency(NOTEFREQS[voices[14].note + oscPitchB] * detune);
  }
}

void updateVoice16() {

  if (unison == 1)
  {
    waveformMod16a.frequency(NOTEFREQS[voices[15].note + oscPitchA] * (detune + (1 - detune) * 0.97f));
  }
  else
  {
    waveformMod16a.frequency(NOTEFREQS[voices[15].note + oscPitchA]);
  }
  waveformMod16b.frequency(NOTEFREQS[voices[15].note + oscPitchB] * detune);
}

int getLFOWaveform(int value) {
  if (value >= 0 && value < 8)
  {
    return WAVEFORM_SINE;
  }
  else if (value >= 8 && value < 30)
  {
    return WAVEFORM_TRIANGLE;
  }
  else if (value >= 30 && value < 63)
  {
    return WAVEFORM_SAWTOOTH_REVERSE;
  }
  else if (value >= 63 && value < 92)
  {
    return WAVEFORM_SAWTOOTH;
  }
  else if (value >= 92 && value < 111)
  {
    return WAVEFORM_SQUARE;
  }
  else
  {
    return WAVEFORM_SAMPLE_HOLD;
  }
}

String getWaveformStr(int value) {
  switch (value)
  {
    case WAVEFORM_SILENT:
      return "Off";
    case WAVEFORM_SAMPLE_HOLD:
      return "Sample & Hold";
    case WAVEFORM_SINE:
      return "Sine";
    case WAVEFORM_BANDLIMIT_SQUARE:
    case WAVEFORM_SQUARE:
      return "Square";
    case WAVEFORM_TRIANGLE:
      return "Triangle";
    case WAVEFORM_BANDLIMIT_SAWTOOTH:
    case WAVEFORM_SAWTOOTH:
      return "Sawtooth";
    case WAVEFORM_SAWTOOTH_REVERSE:
      return "Ramp";
    case WAVEFORM_BANDLIMIT_PULSE:
      return "Var. Pulse";
    case WAVEFORM_TRIANGLE_VARIABLE:
      return "Var. Triangle";
    case WAVEFORM_PARABOLIC:
      return "Parabolic";
    case WAVEFORM_HARMONIC:
      return "Harmonic";
    default:
      return "ERR_WAVE";
  }
}

void loadArbWaveformA(const int16_t * wavedata) {
  waveformMod1a.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod2a.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod3a.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod4a.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod5a.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod6a.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod7a.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod8a.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod9a.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod10a.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod11a.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod12a.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod13a.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod14a.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod15a.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod16a.arbitraryWaveform(wavedata, AWFREQ);
}

void loadArbWaveformB(const int16_t * wavedata) {
  waveformMod1b.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod2b.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod3b.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod4b.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod5b.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod6b.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod7b.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod8b.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod9b.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod10b.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod11b.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod12b.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod13b.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod14b.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod15b.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod16b.arbitraryWaveform(wavedata, AWFREQ);
}

float getLFOTempoRate(int value) {
  lfoTempoValue = LFOTEMPO[value];
  return lfoSyncFreq * LFOTEMPO[value];
}

int getWaveformA(int value) {
  if (value >= 0 && value < 7)
  {
    //This will turn the osc off
    return WAVEFORM_SILENT;
  }
  else if (value >= 7 && value < 23)
  {
    return WAVEFORM_TRIANGLE;
  }
  else if (value >= 23 && value < 40)
  {
    return WAVEFORM_BANDLIMIT_SQUARE;
  }
  else if (value >= 40 && value < 60)
  {
    return WAVEFORM_BANDLIMIT_SAWTOOTH;
  }
  else if (value >= 60 && value < 80)
  {
    return WAVEFORM_BANDLIMIT_PULSE;
  }
  else if (value >= 80 && value < 100)
  {
    return WAVEFORM_TRIANGLE_VARIABLE;
  }
  else if (value >= 100 && value < 120)
  {
    return WAVEFORM_PARABOLIC;
  }
  else
  {
    return WAVEFORM_HARMONIC;
  }
}

int getWaveformB(int value)
{
  if (value >= 0 && value < 7)
  {
    //This will turn the osc off
    return WAVEFORM_SILENT;
  }
  else if (value >= 7 && value < 23)
  {
    return WAVEFORM_SAMPLE_HOLD;
  }
  else if (value >= 23 && value < 40)
  {
    return WAVEFORM_BANDLIMIT_SQUARE;
  }
  else if (value >= 40 && value < 60)
  {
    return WAVEFORM_BANDLIMIT_SAWTOOTH;
  }
  else if (value >= 60 && value < 80)
  {
    return WAVEFORM_BANDLIMIT_PULSE;
  }
  else if (value >= 80 && value < 100)
  {
    return WAVEFORM_TRIANGLE_VARIABLE;
  }
  else if (value >= 100 && value < 120)
  {
    return WAVEFORM_PARABOLIC;
  }
  else
  {
    return WAVEFORM_HARMONIC;
  }
}

int getPitch(int value)
{
  return PITCH[value];
}

void setPwmMixerALFO(float value) {
  pwMixer1a.gain(0, value);
  pwMixer2a.gain(0, value);
  pwMixer3a.gain(0, value);
  pwMixer4a.gain(0, value);
  pwMixer5a.gain(0, value);
  pwMixer6a.gain(0, value);
  pwMixer7a.gain(0, value);
  pwMixer8a.gain(0, value);
  pwMixer9a.gain(0, value);
  pwMixer10a.gain(0, value);
  pwMixer11a.gain(0, value);
  pwMixer12a.gain(0, value);
  pwMixer13a.gain(0, value);
  pwMixer14a.gain(0, value);
  pwMixer15a.gain(0, value);
  pwMixer16a.gain(0, value);
  showCurrentParameterPage("1. PWM LFO", String(value));
}

void setPwmMixerBLFO(float value) {
  pwMixer1b.gain(0, value);
  pwMixer2b.gain(0, value);
  pwMixer3b.gain(0, value);
  pwMixer4b.gain(0, value);
  pwMixer5b.gain(0, value);
  pwMixer6b.gain(0, value);
  pwMixer7b.gain(0, value);
  pwMixer8b.gain(0, value);
  pwMixer9b.gain(0, value);
  pwMixer10b.gain(0, value);
  pwMixer11b.gain(0, value);
  pwMixer12b.gain(0, value);
  pwMixer13b.gain(0, value);
  pwMixer14b.gain(0, value);
  pwMixer15b.gain(0, value);
  pwMixer16b.gain(0, value);
  showCurrentParameterPage("2. PWM LFO", String(value));
}

void setPwmMixerAPW(float value) {
  pwMixer1a.gain(1, value);
  pwMixer2a.gain(1, value);
  pwMixer3a.gain(1, value);
  pwMixer4a.gain(1, value);
  pwMixer5a.gain(1, value);
  pwMixer6a.gain(1, value);
  pwMixer7a.gain(1, value);
  pwMixer8a.gain(1, value);
  pwMixer9a.gain(1, value);
  pwMixer10a.gain(1, value);
  pwMixer11a.gain(1, value);
  pwMixer12a.gain(1, value);
  pwMixer13a.gain(1, value);
  pwMixer14a.gain(1, value);
  pwMixer15a.gain(1, value);
  pwMixer16a.gain(1, value);
}

void setPwmMixerBPW(float value) {
  pwMixer1b.gain(1, value);
  pwMixer2b.gain(1, value);
  pwMixer3b.gain(1, value);
  pwMixer4b.gain(1, value);
  pwMixer5b.gain(1, value);
  pwMixer6b.gain(1, value);
  pwMixer7b.gain(1, value);
  pwMixer8b.gain(1, value);
  pwMixer9b.gain(1, value);
  pwMixer10b.gain(1, value);
  pwMixer11b.gain(1, value);
  pwMixer12b.gain(1, value);
  pwMixer13b.gain(1, value);
  pwMixer14b.gain(1, value);
  pwMixer15b.gain(1, value);
  pwMixer16b.gain(1, value);
}

void setPwmMixerAFEnv(float value) {
  pwMixer1a.gain(2, value);
  pwMixer2a.gain(2, value);
  pwMixer3a.gain(2, value);
  pwMixer4a.gain(2, value);
  pwMixer5a.gain(2, value);
  pwMixer6a.gain(2, value);
  pwMixer7a.gain(2, value);
  pwMixer8a.gain(2, value);
  pwMixer9a.gain(2, value);
  pwMixer10a.gain(2, value);
  pwMixer11a.gain(2, value);
  pwMixer12a.gain(2, value);
  pwMixer13a.gain(2, value);
  pwMixer14a.gain(2, value);
  pwMixer15a.gain(2, value);
  pwMixer16a.gain(2, value);
  showCurrentParameterPage("1. PWM F Env", String(value));
}

void setPwmMixerBFEnv(float value) {
  pwMixer1b.gain(2, value);
  pwMixer2b.gain(2, value);
  pwMixer3b.gain(2, value);
  pwMixer4b.gain(2, value);
  pwMixer5b.gain(2, value);
  pwMixer6b.gain(2, value);
  pwMixer7b.gain(2, value);
  pwMixer8b.gain(2, value);
  pwMixer9b.gain(2, value);
  pwMixer10b.gain(2, value);
  pwMixer11b.gain(2, value);
  pwMixer12b.gain(2, value);
  pwMixer13b.gain(2, value);
  pwMixer14b.gain(2, value);
  pwMixer15b.gain(2, value);
  pwMixer16b.gain(2, value);
  showCurrentParameterPage("2. PWM F Env", String(value));
}

void updateUnison() {
  if (unison == 0) {
    allNotesOff();//Avoid hanging notes
    showCurrentParameterPage("Unison", "Off");
    digitalWriteFast(UNISON_LED, LOW);  // LED off
  }
  else
  {
    showCurrentParameterPage("Unison", "On");
    digitalWriteFast(UNISON_LED, HIGH);  // LED on
  }
}

void updateVolume(float vol)
{
  showCurrentParameterPage("Volume", vol);
}

void updateGlide() {
  if (glideSpeed * GLIDEFACTOR < 1000) {
    showCurrentParameterPage("Glide", String(int(glideSpeed * GLIDEFACTOR)) + " ms");
  }
  else {
    showCurrentParameterPage("Glide", String((glideSpeed * GLIDEFACTOR) / 1000) + " s");
  }
}

void updateWaveformA() {
  int newWaveform = oscWaveformA;//To allow Arbitrary waveforms
  if (oscWaveformA == WAVEFORM_PARABOLIC) {
    loadArbWaveformA(PARABOLIC_WAVE);
    newWaveform = WAVEFORM_ARBITRARY;
  }
  if (oscWaveformA == WAVEFORM_HARMONIC) {
    loadArbWaveformA(HARMONIC_WAVE);
    newWaveform = WAVEFORM_ARBITRARY;
  }

  waveformMod1a.begin(newWaveform);
  waveformMod2a.begin(newWaveform);
  waveformMod3a.begin(newWaveform);
  waveformMod4a.begin(newWaveform);
  waveformMod5a.begin(newWaveform);
  waveformMod6a.begin(newWaveform);
  waveformMod7a.begin(newWaveform);
  waveformMod8a.begin(newWaveform);
  waveformMod9a.begin(newWaveform);
  waveformMod10a.begin(newWaveform);
  waveformMod11a.begin(newWaveform);
  waveformMod12a.begin(newWaveform);
  waveformMod13a.begin(newWaveform);
  waveformMod14a.begin(newWaveform);
  waveformMod15a.begin(newWaveform);
  waveformMod16a.begin(newWaveform);
  showCurrentParameterPage("1. Waveform", getWaveformStr(oscWaveformA));
}

void updateWaveformB() {
  int newWaveform = oscWaveformB;//To allow Arbitrary waveforms
  if (oscWaveformB == WAVEFORM_PARABOLIC) {
    loadArbWaveformB(PARABOLIC_WAVE);
    newWaveform = WAVEFORM_ARBITRARY;
  }
  if (oscWaveformB == WAVEFORM_HARMONIC) {
    loadArbWaveformB(PPG_WAVE);
    newWaveform = WAVEFORM_ARBITRARY;
  }

  waveformMod1b.begin(newWaveform);
  waveformMod2b.begin(newWaveform);
  waveformMod3b.begin(newWaveform);
  waveformMod4b.begin(newWaveform);
  waveformMod5b.begin(newWaveform);
  waveformMod6b.begin(newWaveform);
  waveformMod7b.begin(newWaveform);
  waveformMod8b.begin(newWaveform);
  waveformMod9b.begin(newWaveform);
  waveformMod10b.begin(newWaveform);
  waveformMod11b.begin(newWaveform);
  waveformMod12b.begin(newWaveform);
  waveformMod13b.begin(newWaveform);
  waveformMod14b.begin(newWaveform);
  waveformMod15b.begin(newWaveform);
  waveformMod16b.begin(newWaveform);
  showCurrentParameterPage("2. Waveform", getWaveformStr(oscWaveformB));
}

void updatePitchA() {
  updatesAllVoices();
  showCurrentParameterPage("1. Semitones", (oscPitchA > 0 ? "+" : "") + String(oscPitchA));
}

void updatePitchB() {
  updatesAllVoices();
  showCurrentParameterPage("2. Semitones", (oscPitchB > 0 ? "+" : "") + String(oscPitchB));
}

void updateDetune() {
  updatesAllVoices();
  showCurrentParameterPage("Detune", String((1 - detune) * 100) + " %");
}

void updatesAllVoices() {
  updateVoice1();
  updateVoice2();
  updateVoice3();
  updateVoice4();
  updateVoice5();
  updateVoice6();
  updateVoice7();
  updateVoice8();
  updateVoice9();
  updateVoice10();
  updateVoice11();
  updateVoice12();
  updateVoice13();
  updateVoice14();
  updateVoice15();
  updateVoice16();
}

void updatePWMSource() {
  if (pwmSource == PWMSOURCELFO)
  {
    setPwmMixerAFEnv(0);
    setPwmMixerBFEnv(0);
    if (pwmRate > -5)
    {
      setPwmMixerALFO(pwmAmtA);
      setPwmMixerBLFO(pwmAmtB);
    }
    showCurrentParameterPage("PWM Source", "LFO"); //Only shown when updated via MIDI
  }
  else
  {
    setPwmMixerALFO(0);
    setPwmMixerBLFO(0);
    if (pwmRate > -5)
    {
      setPwmMixerAFEnv(pwmAmtA);
      setPwmMixerBFEnv(pwmAmtB);
    }
    showCurrentParameterPage("PWM Source", "Filter Env");
  }
}

void updatePWMRate() {
  pwmLfo.frequency(pwmRate);
  if (pwmRate < -5)
  {
    //Set to fixed PW mode
    setPwmMixerALFO(0);//LFO Source off
    setPwmMixerBLFO(0);
    setPwmMixerAFEnv(0);//Filter Env Source off
    setPwmMixerBFEnv(0);
    setPwmMixerAPW(1);//Manually adjustable pulse width on
    setPwmMixerBPW(1);
    showCurrentParameterPage("PW Mode", "On");
  }
  else if (pwmRate > -10 && pwmRate < 0) {
    //Set to Filter Env Mod source
    pwmSource = PWMSOURCEFENV;
    updatePWMSource();
    //Filter env mod - pwmRate does nothing
    setPwmMixerALFO(0);
    setPwmMixerBLFO(0);
    setPwmMixerAFEnv(pwmAmtA);
    setPwmMixerBFEnv(pwmAmtB);
    showCurrentParameterPage("PWM Source", "Filter Env");
  } else {

    pwmSource = PWMSOURCELFO;
    updatePWMSource();
    setPwmMixerAPW(0);
    setPwmMixerBPW(0);
    showCurrentParameterPage("PWM Rate", String(pwmRate) + " Hz");
  }
}

void updatePWMAmount() {
  //MIDI only - sets both osc
  pwA = 0;
  pwB = 0;
  setPwmMixerALFO(pwmAmtA);
  setPwmMixerBLFO(pwmAmtB);
  showCurrentParameterPage("PWM Amt", String(pwmAmtA) + " " + String(pwmAmtB));
}

void updatePWA() {
  if (pwmRate < -5)
  {
    //if PWM amount is around zero, fixed PW is enabled
    setPwmMixerALFO(0);
    setPwmMixerBLFO(0);
    setPwmMixerAFEnv(0);
    setPwmMixerBFEnv(0);
    setPwmMixerAPW(1);
    setPwmMixerBPW(1);
    if (oscWaveformA == WAVEFORM_TRIANGLE_VARIABLE)
    {
      showCurrentParameterPage("1. PW Amt", pwA, VAR_TRI);
    }
    else
    {
      showCurrentParameterPage("1. PW Amt", pwA, PULSE);
    }
  }
  else
  {
    setPwmMixerAPW(0);
    setPwmMixerBPW(0);
    if (pwmSource == PWMSOURCELFO)
    {
      //PW alters PWM LFO amount for waveform A
      setPwmMixerALFO(pwmAmtA);
      showCurrentParameterPage("1. PWM Amt", "LFO " + String(pwmAmtA));
    }
    else
    {
      //PW alters PWM Filter Env amount for waveform A
      setPwmMixerAFEnv(pwmAmtA);
      showCurrentParameterPage("1. PWM Amt", "F. Env " + String(pwmAmtA));
    }
  }
  pwa.amplitude(pwA);
}

void updatePWB()
{
  if (pwmRate < -5)
  {
    //if PWM amount is around zero, fixed PW is enabled
    setPwmMixerALFO(0);
    setPwmMixerBLFO(0);
    setPwmMixerAFEnv(0);
    setPwmMixerBFEnv(0);
    setPwmMixerAPW(1);
    setPwmMixerBPW(1);
    if (oscWaveformB == WAVEFORM_TRIANGLE_VARIABLE)
    {
      showCurrentParameterPage("2. PW Amt", pwB, VAR_TRI);
    }
    else
    {
      showCurrentParameterPage("2. PW Amt", pwB, PULSE);
    }
  }
  else
  {
    setPwmMixerAPW(0);
    setPwmMixerBPW(0);
    if (pwmSource == PWMSOURCELFO)
    {
      //PW alters PWM LFO amount for waveform B
      setPwmMixerBLFO(pwmAmtB);
      showCurrentParameterPage("2. PWM Amt", "LFO " + String(pwmAmtB));
    }
    else
    {
      //PW alters PWM Filter Env amount for waveform B
      setPwmMixerBFEnv(pwmAmtB);
      showCurrentParameterPage("2. PWM Amt", "F. Env " + String(pwmAmtB));
    }
  }
  pwb.amplitude(pwB);
}

void updateOscLevelA()
{
  waveformMixer1.gain(0, oscALevel);
  waveformMixer2.gain(0, oscALevel);
  waveformMixer3.gain(0, oscALevel);
  waveformMixer4.gain(0, oscALevel);
  waveformMixer5.gain(0, oscALevel);
  waveformMixer6.gain(0, oscALevel);
  waveformMixer7.gain(0, oscALevel);
  waveformMixer8.gain(0, oscALevel);
  waveformMixer9.gain(0, oscALevel);
  waveformMixer10.gain(0, oscALevel);
  waveformMixer11.gain(0, oscALevel);
  waveformMixer12.gain(0, oscALevel);
  waveformMixer13.gain(0, oscALevel);
  waveformMixer14.gain(0, oscALevel);
  waveformMixer15.gain(0, oscALevel);
  waveformMixer16.gain(0, oscALevel);
  if (oscFX == 1)
  {
    waveformMixer1.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer2.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer3.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer4.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer5.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer6.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer7.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer8.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer9.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer10.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer11.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer12.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer13.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer14.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer15.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer16.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
  }
  showCurrentParameterPage("Osc Levels", "   " + String(oscALevel) + " - " + String(oscBLevel));
}

void updateOscLevelB() {
  waveformMixer1.gain(1, oscBLevel);
  waveformMixer2.gain(1, oscBLevel);
  waveformMixer3.gain(1, oscBLevel);
  waveformMixer4.gain(1, oscBLevel);
  waveformMixer5.gain(1, oscBLevel);
  waveformMixer6.gain(1, oscBLevel);
  waveformMixer7.gain(1, oscBLevel);
  waveformMixer8.gain(1, oscBLevel);
  waveformMixer9.gain(1, oscBLevel);
  waveformMixer10.gain(1, oscBLevel);
  waveformMixer11.gain(1, oscBLevel);
  waveformMixer12.gain(1, oscBLevel);
  waveformMixer13.gain(1, oscBLevel);
  waveformMixer14.gain(1, oscBLevel);
  waveformMixer15.gain(1, oscBLevel);
  waveformMixer16.gain(1, oscBLevel);
  if (oscFX == 1) {
    waveformMixer1.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer2.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer3.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer4.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer5.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer6.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer7.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer8.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer9.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer10.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer11.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer12.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer13.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer14.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer15.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer16.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
  }
  showCurrentParameterPage("Osc Levels", "   " + String(oscALevel) + " - " + String(oscBLevel));
}

void updateNoiseLevel()
{
  if (noiseLevel > 0)
  {
    pink.amplitude(noiseLevel);
    white.amplitude(0.0f);
    showCurrentParameterPage("Noise Level", "Pink " + String(noiseLevel));
  }
  else if (noiseLevel < 0)
  {
    pink.amplitude(0.0f);
    white.amplitude(abs(noiseLevel));
    showCurrentParameterPage("Noise Level", "White " + String(abs(noiseLevel)));
  }
  else
  {
    pink.amplitude(noiseLevel);
    white.amplitude(noiseLevel);
    showCurrentParameterPage("Noise Level", "Off");
  }
}

void updateFilterFreq()
{
  filter1.frequency(filterFreq);
  filter2.frequency(filterFreq);
  filter3.frequency(filterFreq);
  filter4.frequency(filterFreq);
  filter5.frequency(filterFreq);
  filter6.frequency(filterFreq);
  filter7.frequency(filterFreq);
  filter8.frequency(filterFreq);
  filter9.frequency(filterFreq);
  filter10.frequency(filterFreq);
  filter11.frequency(filterFreq);
  filter12.frequency(filterFreq);
  filter13.frequency(filterFreq);
  filter14.frequency(filterFreq);
  filter15.frequency(filterFreq);
  filter16.frequency(filterFreq);

  if (filterFreq > 2500) {
    filterOctave = 2.0f;//Allows more accurate filter cutoff
  } else if (filterFreq < 60) {
    filterOctave = 7.0f; //Allows deeper bass
  } else {
    filterOctave = 2.0f + ((2560.0f - filterFreq) / 500.0f);//In between
  }

  filter1.octaveControl(filterOctave);
  filter2.octaveControl(filterOctave);
  filter3.octaveControl(filterOctave);
  filter4.octaveControl(filterOctave);
  filter5.octaveControl(filterOctave);
  filter6.octaveControl(filterOctave);
  filter7.octaveControl(filterOctave);
  filter8.octaveControl(filterOctave);
  filter9.octaveControl(filterOctave);
  filter10.octaveControl(filterOctave);
  filter11.octaveControl(filterOctave);
  filter12.octaveControl(filterOctave);
  filter13.octaveControl(filterOctave);
  filter14.octaveControl(filterOctave);
  filter15.octaveControl(filterOctave);
  filter16.octaveControl(filterOctave);

  showCurrentParameterPage("Cutoff", String(int(filterFreq)) + " Hz");
}

void updateFilterRes()
{
  filter1.resonance(filterRes);
  filter2.resonance(filterRes);
  filter3.resonance(filterRes);
  filter4.resonance(filterRes);
  filter5.resonance(filterRes);
  filter6.resonance(filterRes);
  filter7.resonance(filterRes);
  filter8.resonance(filterRes);
  filter9.resonance(filterRes);
  filter10.resonance(filterRes);
  filter11.resonance(filterRes);
  filter12.resonance(filterRes);
  filter13.resonance(filterRes);
  filter14.resonance(filterRes);
  filter15.resonance(filterRes);
  filter16.resonance(filterRes);
  showCurrentParameterPage("Resonance", filterRes);
}

void updateFilterMixer()
{
  float LP = 1.0f;
  float BP = 0;
  float HP = 0;
  String filterStr;
  if (filterMix == LINEAR_FILTERMIXER[127])
  {
    //BP mode
    LP = 0;
    BP = 1.0f;
    HP = 0;
    filterStr = "Band Pass";
  }
  else
  {
    //LP-HP mix mode - a notch filter
    LP = 1.0f - filterMix;
    BP = 0;
    HP = filterMix;
    if (filterMix == LINEAR_FILTERMIXER[0])
    {
      filterStr = "Low Pass";
    }
    else if (filterMix == LINEAR_FILTERMIXER[125])
    {
      filterStr = "High Pass";
    }
    else
    {
      filterStr = "LP " + String(100 - filterMixStr) + " - " + String(filterMixStr) + " HP";
    }
  }
  filterMixer1.gain(0, LP);
  filterMixer1.gain(1, BP);
  filterMixer1.gain(2, HP);
  filterMixer2.gain(0, LP);
  filterMixer2.gain(1, BP);
  filterMixer2.gain(2, HP);
  filterMixer3.gain(0, LP);
  filterMixer3.gain(1, BP);
  filterMixer3.gain(2, HP);
  filterMixer4.gain(0, LP);
  filterMixer4.gain(1, BP);
  filterMixer4.gain(2, HP);
  filterMixer5.gain(0, LP);
  filterMixer5.gain(1, BP);
  filterMixer5.gain(2, HP);
  filterMixer6.gain(0, LP);
  filterMixer6.gain(1, BP);
  filterMixer6.gain(2, HP);
  filterMixer7.gain(0, LP);
  filterMixer7.gain(1, BP);
  filterMixer7.gain(2, HP);
  filterMixer8.gain(0, LP);
  filterMixer8.gain(1, BP);
  filterMixer8.gain(2, HP);
  filterMixer9.gain(0, LP);
  filterMixer9.gain(1, BP);
  filterMixer9.gain(2, HP);
  filterMixer10.gain(0, LP);
  filterMixer10.gain(1, BP);
  filterMixer10.gain(2, HP);
  filterMixer11.gain(0, LP);
  filterMixer11.gain(1, BP);
  filterMixer11.gain(2, HP);
  filterMixer12.gain(0, LP);
  filterMixer12.gain(1, BP);
  filterMixer12.gain(2, HP);
  filterMixer13.gain(0, LP);
  filterMixer13.gain(1, BP);
  filterMixer13.gain(2, HP);
  filterMixer14.gain(0, LP);
  filterMixer14.gain(1, BP);
  filterMixer14.gain(2, HP);
  filterMixer15.gain(0, LP);
  filterMixer15.gain(1, BP);
  filterMixer15.gain(2, HP);
  filterMixer16.gain(0, LP);
  filterMixer16.gain(1, BP);
  filterMixer16.gain(2, HP);
  showCurrentParameterPage("Filter Type", filterStr);
}

void updateFilterEnv()
{
  filterModMixer1.gain(0, filterEnv);
  filterModMixer2.gain(0, filterEnv);
  filterModMixer3.gain(0, filterEnv);
  filterModMixer4.gain(0, filterEnv);
  filterModMixer5.gain(0, filterEnv);
  filterModMixer6.gain(0, filterEnv);
  filterModMixer7.gain(0, filterEnv);
  filterModMixer8.gain(0, filterEnv);
  filterModMixer9.gain(0, filterEnv);
  filterModMixer10.gain(0, filterEnv);
  filterModMixer11.gain(0, filterEnv);
  filterModMixer12.gain(0, filterEnv);
  filterModMixer13.gain(0, filterEnv);
  filterModMixer14.gain(0, filterEnv);
  filterModMixer15.gain(0, filterEnv);
  filterModMixer16.gain(0, filterEnv);
  showCurrentParameterPage("Filter Env.", String(filterEnv));
}

void updatePitchEnv()
{
  oscModMixer1a.gain(1, pitchEnv);
  oscModMixer1b.gain(1, pitchEnv);
  oscModMixer2a.gain(1, pitchEnv);
  oscModMixer2b.gain(1, pitchEnv);
  oscModMixer3a.gain(1, pitchEnv);
  oscModMixer3b.gain(1, pitchEnv);
  oscModMixer4a.gain(1, pitchEnv);
  oscModMixer4b.gain(1, pitchEnv);
  oscModMixer5a.gain(1, pitchEnv);
  oscModMixer5b.gain(1, pitchEnv);
  oscModMixer6a.gain(1, pitchEnv);
  oscModMixer6b.gain(1, pitchEnv);
  oscModMixer7a.gain(1, pitchEnv);
  oscModMixer7b.gain(1, pitchEnv);
  oscModMixer8a.gain(1, pitchEnv);
  oscModMixer8b.gain(1, pitchEnv);
  oscModMixer9a.gain(1, pitchEnv);
  oscModMixer9b.gain(1, pitchEnv);
  oscModMixer10a.gain(1, pitchEnv);
  oscModMixer10b.gain(1, pitchEnv);
  oscModMixer11a.gain(1, pitchEnv);
  oscModMixer11b.gain(1, pitchEnv);
  oscModMixer12a.gain(1, pitchEnv);
  oscModMixer12b.gain(1, pitchEnv);
  oscModMixer13a.gain(1, pitchEnv);
  oscModMixer13b.gain(1, pitchEnv);
  oscModMixer14a.gain(1, pitchEnv);
  oscModMixer14b.gain(1, pitchEnv);
  oscModMixer15a.gain(1, pitchEnv);
  oscModMixer15b.gain(1, pitchEnv);
  oscModMixer16a.gain(1, pitchEnv);
  oscModMixer16b.gain(1, pitchEnv);
  showCurrentParameterPage("Pitch Env Amt", String(pitchEnv));
}

void updateKeyTracking()
{
  filterModMixer1.gain(2, keytrackingAmount);
  filterModMixer2.gain(2, keytrackingAmount);
  filterModMixer3.gain(2, keytrackingAmount);
  filterModMixer4.gain(2, keytrackingAmount);
  filterModMixer5.gain(2, keytrackingAmount);
  filterModMixer6.gain(2, keytrackingAmount);
  filterModMixer7.gain(2, keytrackingAmount);
  filterModMixer8.gain(2, keytrackingAmount);
  filterModMixer9.gain(2, keytrackingAmount);
  filterModMixer10.gain(2, keytrackingAmount);
  filterModMixer11.gain(2, keytrackingAmount);
  filterModMixer12.gain(2, keytrackingAmount);
  filterModMixer13.gain(2, keytrackingAmount);
  filterModMixer14.gain(2, keytrackingAmount);
  filterModMixer15.gain(2, keytrackingAmount);
  filterModMixer16.gain(2, keytrackingAmount);
  showCurrentParameterPage("Key Tracking", String(keytrackingAmount));
}

void updateOscLFOAmt()
{
  pitchLfo.amplitude(oscLfoAmt);
  char buf[10];
  showCurrentParameterPage("LFO Amount", dtostrf(oscLfoAmt, 4, 3, buf));
}

void updateModWheel()
{
  pitchLfo.amplitude(oscLfoAmt);
}

void updatePitchLFORate()
{
  pitchLfo.frequency(oscLfoRate);
  showCurrentParameterPage("LFO Rate", String(oscLfoRate) + " Hz");
}

void updatePitchLFOWaveform()
{
  pitchLfo.begin(oscLFOWaveform);
  showCurrentParameterPage("Pitch LFO", getWaveformStr(oscLFOWaveform));
}

//MIDI CC only
void updatePitchLFOMidiClkSync()
{
  showCurrentParameterPage("P. LFO Sync", oscLFOMidiClkSync == 1 ? "On" : "Off");
}

void updateFilterLfoRate()
{
  filterLfo.frequency(filterLfoRate);
  if (filterLFOMidiClkSync)
  {
    showCurrentParameterPage("LFO Time Div", filterLFOTimeDivStr);
  }
  else
  {
    showCurrentParameterPage("F. LFO Rate", String(filterLfoRate) + " Hz");
  }
}

void updateFilterLfoAmt()
{
  filterLfo.amplitude(filterLfoAmt);
  showCurrentParameterPage("F. LFO Amt", String(filterLfoAmt));
}

void updateFilterLFOWaveform()
{
  filterLfo.begin(filterLfoWaveform);
  showCurrentParameterPage("Filter LFO", getWaveformStr(filterLfoWaveform));
}

void updatePitchLFORetrig()
{
  showCurrentParameterPage("P. LFO Retrig", oscLfoRetrig == 1 ? "On" : "Off");
}

void updateFilterLFORetrig()
{
  showCurrentParameterPage("F. LFO Retrig", filterLfoRetrig == 1 ? "On" : "Off");
  digitalWriteFast(RETRIG_LED, filterLfoRetrig == 1 ? HIGH : LOW);  // LED
}

void updateFilterLFOMidiClkSync()
{
  showCurrentParameterPage("Tempo Sync", filterLFOMidiClkSync == 1 ? "On" : "Off");
  digitalWriteFast(TEMPO_LED, filterLFOMidiClkSync == 1 ? HIGH : LOW);  // LED
}

void updateFilterAttack()
{
  filterEnvelope1.attack(filterAttack);
  filterEnvelope2.attack(filterAttack);
  filterEnvelope3.attack(filterAttack);
  filterEnvelope4.attack(filterAttack);
  filterEnvelope5.attack(filterAttack);
  filterEnvelope6.attack(filterAttack);
  filterEnvelope7.attack(filterAttack);
  filterEnvelope8.attack(filterAttack);
  filterEnvelope9.attack(filterAttack);
  filterEnvelope10.attack(filterAttack);
  filterEnvelope11.attack(filterAttack);
  filterEnvelope12.attack(filterAttack);
  filterEnvelope13.attack(filterAttack);
  filterEnvelope14.attack(filterAttack);
  filterEnvelope15.attack(filterAttack);
  filterEnvelope16.attack(filterAttack);
  if (filterAttack < 1000)
  {
    showCurrentParameterPage("Filter Attack", String(int(filterAttack)) + " ms", FILTER_ENV);
  }
  else
  {
    showCurrentParameterPage("Filter Attack", String(filterAttack * 0.001f) + " s", FILTER_ENV);
  }
}

void updateFilterDecay()
{
  filterEnvelope1.decay(filterDecay);
  filterEnvelope2.decay(filterDecay);
  filterEnvelope3.decay(filterDecay);
  filterEnvelope4.decay(filterDecay);
  filterEnvelope5.decay(filterDecay);
  filterEnvelope6.decay(filterDecay);
  filterEnvelope7.decay(filterDecay);
  filterEnvelope8.decay(filterDecay);
  filterEnvelope9.decay(filterDecay);
  filterEnvelope10.decay(filterDecay);
  filterEnvelope11.decay(filterDecay);
  filterEnvelope12.decay(filterDecay);
  filterEnvelope13.decay(filterDecay);
  filterEnvelope14.decay(filterDecay);
  filterEnvelope15.decay(filterDecay);
  filterEnvelope16.decay(filterDecay);
  if (filterDecay < 1000)
  {
    showCurrentParameterPage("Filter Decay", String(int(filterDecay)) + " ms", FILTER_ENV);
  }
  else
  {
    showCurrentParameterPage("Filter Decay", String(filterDecay * 0.001f) + " s", FILTER_ENV);
  }
}

void updateFilterSustain()
{
  filterEnvelope1.sustain(filterSustain);
  filterEnvelope2.sustain(filterSustain);
  filterEnvelope3.sustain(filterSustain);
  filterEnvelope4.sustain(filterSustain);
  filterEnvelope5.sustain(filterSustain);
  filterEnvelope6.sustain(filterSustain);
  filterEnvelope7.sustain(filterSustain);
  filterEnvelope8.sustain(filterSustain);
  filterEnvelope9.sustain(filterSustain);
  filterEnvelope10.sustain(filterSustain);
  filterEnvelope11.sustain(filterSustain);
  filterEnvelope12.sustain(filterSustain);
  filterEnvelope13.sustain(filterSustain);
  filterEnvelope14.sustain(filterSustain);
  filterEnvelope15.sustain(filterSustain);
  filterEnvelope16.sustain(filterSustain);
  showCurrentParameterPage("Filter Sustain", String(filterSustain), FILTER_ENV);
}

void updateFilterRelease()
{
  filterEnvelope1.release(filterRelease);
  filterEnvelope2.release(filterRelease);
  filterEnvelope3.release(filterRelease);
  filterEnvelope4.release(filterRelease);
  filterEnvelope5.release(filterRelease);
  filterEnvelope6.release(filterRelease);
  filterEnvelope7.release(filterRelease);
  filterEnvelope8.release(filterRelease);
  filterEnvelope9.release(filterRelease);
  filterEnvelope10.release(filterRelease);
  filterEnvelope11.release(filterRelease);
  filterEnvelope12.release(filterRelease);
  filterEnvelope13.release(filterRelease);
  filterEnvelope14.release(filterRelease);
  filterEnvelope15.release(filterRelease);
  filterEnvelope16.release(filterRelease);
  if (filterRelease < 1000)
  {
    showCurrentParameterPage("Filter Release", String(int(filterRelease)) + " ms", FILTER_ENV);
  }
  else
  {
    showCurrentParameterPage("Filter Release", String(filterRelease * 0.001) + " s", FILTER_ENV);
  }
}

void updateAttack()
{
  ampEnvelope1.attack(ampAttack);
  ampEnvelope2.attack(ampAttack);
  ampEnvelope3.attack(ampAttack);
  ampEnvelope4.attack(ampAttack);
  ampEnvelope5.attack(ampAttack);
  ampEnvelope6.attack(ampAttack);
  ampEnvelope7.attack(ampAttack);
  ampEnvelope8.attack(ampAttack);
  ampEnvelope9.attack(ampAttack);
  ampEnvelope10.attack(ampAttack);
  ampEnvelope11.attack(ampAttack);
  ampEnvelope12.attack(ampAttack);
  ampEnvelope13.attack(ampAttack);
  ampEnvelope14.attack(ampAttack);
  ampEnvelope15.attack(ampAttack);
  ampEnvelope16.attack(ampAttack);
  if (ampAttack < 1000)
  {
    showCurrentParameterPage("Attack", String(int(ampAttack)) + " ms", AMP_ENV);
  }
  else
  {
    showCurrentParameterPage("Attack", String(ampAttack * 0.001) + " s", AMP_ENV);
  }
}

void updateDecay()
{
  ampEnvelope1.decay(ampDecay);
  ampEnvelope2.decay(ampDecay);
  ampEnvelope3.decay(ampDecay);
  ampEnvelope4.decay(ampDecay);
  ampEnvelope5.decay(ampDecay);
  ampEnvelope6.decay(ampDecay);
  ampEnvelope7.decay(ampDecay);
  ampEnvelope8.decay(ampDecay);
  ampEnvelope9.decay(ampDecay);
  ampEnvelope10.decay(ampDecay);
  ampEnvelope11.decay(ampDecay);
  ampEnvelope12.decay(ampDecay);
  ampEnvelope13.decay(ampDecay);
  ampEnvelope14.decay(ampDecay);
  ampEnvelope15.decay(ampDecay);
  ampEnvelope16.decay(ampDecay);
  if (ampDecay < 1000)
  {
    showCurrentParameterPage("Decay", String(int(ampDecay)) + " ms", AMP_ENV);
  }
  else
  {
    showCurrentParameterPage("Decay", String(ampDecay * 0.001) + " s", AMP_ENV);
  }
}

void updateSustain()
{
  ampEnvelope1.sustain(ampSustain);
  ampEnvelope2.sustain(ampSustain);
  ampEnvelope3.sustain(ampSustain);
  ampEnvelope4.sustain(ampSustain);
  ampEnvelope5.sustain(ampSustain);
  ampEnvelope6.sustain(ampSustain);
  ampEnvelope7.sustain(ampSustain);
  ampEnvelope8.sustain(ampSustain);
  ampEnvelope9.sustain(ampSustain);
  ampEnvelope10.sustain(ampSustain);
  ampEnvelope11.sustain(ampSustain);
  ampEnvelope12.sustain(ampSustain);
  ampEnvelope13.sustain(ampSustain);
  ampEnvelope14.sustain(ampSustain);
  ampEnvelope15.sustain(ampSustain);
  ampEnvelope16.sustain(ampSustain);
  showCurrentParameterPage("Sustain", String(ampSustain), AMP_ENV);
}

void updateRelease()
{
  ampEnvelope1.release(ampRelease);
  ampEnvelope2.release(ampRelease);
  ampEnvelope3.release(ampRelease);
  ampEnvelope4.release(ampRelease);
  ampEnvelope5.release(ampRelease);
  ampEnvelope6.release(ampRelease);
  ampEnvelope7.release(ampRelease);
  ampEnvelope8.release(ampRelease);
  ampEnvelope9.release(ampRelease);
  ampEnvelope10.release(ampRelease);
  ampEnvelope11.release(ampRelease);
  ampEnvelope12.release(ampRelease);
  ampEnvelope13.release(ampRelease);
  ampEnvelope14.release(ampRelease);
  ampEnvelope15.release(ampRelease);
  ampEnvelope16.release(ampRelease);
  if (ampRelease < 1000)
  {
    showCurrentParameterPage("Release", String(int(ampRelease)) + " ms", AMP_ENV);
  }
  else
  {
    showCurrentParameterPage("Release", String(ampRelease * 0.001) + " s", AMP_ENV);
  }
}

void updateOscFX()
{
  if (oscFX == 1)
  {
    oscFX1.setCombineMode(AudioEffectDigitalCombine::XOR);
    oscFX2.setCombineMode(AudioEffectDigitalCombine::XOR);
    oscFX3.setCombineMode(AudioEffectDigitalCombine::XOR);
    oscFX4.setCombineMode(AudioEffectDigitalCombine::XOR);
    oscFX5.setCombineMode(AudioEffectDigitalCombine::XOR);
    oscFX6.setCombineMode(AudioEffectDigitalCombine::XOR);
    oscFX7.setCombineMode(AudioEffectDigitalCombine::XOR);
    oscFX8.setCombineMode(AudioEffectDigitalCombine::XOR);
    oscFX9.setCombineMode(AudioEffectDigitalCombine::XOR);
    oscFX10.setCombineMode(AudioEffectDigitalCombine::XOR);
    oscFX11.setCombineMode(AudioEffectDigitalCombine::XOR);
    oscFX12.setCombineMode(AudioEffectDigitalCombine::XOR);
    oscFX13.setCombineMode(AudioEffectDigitalCombine::XOR);
    oscFX14.setCombineMode(AudioEffectDigitalCombine::XOR);
    oscFX15.setCombineMode(AudioEffectDigitalCombine::XOR);
    oscFX16.setCombineMode(AudioEffectDigitalCombine::XOR);
    waveformMixer1.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer2.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer3.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer4.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer5.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer6.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer7.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer8.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer9.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer10.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer11.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer12.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer13.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer14.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer15.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer16.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    showCurrentParameterPage("Osc FX", "On");
    digitalWriteFast(OSC_FX_LED, HIGH);  // LED on
  }
  else
  {
    oscFX1.setCombineMode(AudioEffectDigitalCombine::OFF);
    oscFX2.setCombineMode(AudioEffectDigitalCombine::OFF);
    oscFX3.setCombineMode(AudioEffectDigitalCombine::OFF);
    oscFX4.setCombineMode(AudioEffectDigitalCombine::OFF);
    oscFX5.setCombineMode(AudioEffectDigitalCombine::OFF);
    oscFX6.setCombineMode(AudioEffectDigitalCombine::OFF);
    oscFX7.setCombineMode(AudioEffectDigitalCombine::OFF);
    oscFX8.setCombineMode(AudioEffectDigitalCombine::OFF);
    oscFX9.setCombineMode(AudioEffectDigitalCombine::OFF);
    oscFX10.setCombineMode(AudioEffectDigitalCombine::OFF);
    oscFX11.setCombineMode(AudioEffectDigitalCombine::OFF);
    oscFX12.setCombineMode(AudioEffectDigitalCombine::OFF);
    oscFX13.setCombineMode(AudioEffectDigitalCombine::OFF);
    oscFX14.setCombineMode(AudioEffectDigitalCombine::OFF);
    oscFX15.setCombineMode(AudioEffectDigitalCombine::OFF);
    oscFX16.setCombineMode(AudioEffectDigitalCombine::OFF);
    waveformMixer1.gain(3, 0); //Osc FX
    waveformMixer2.gain(3, 0); //Osc FX
    waveformMixer3.gain(3, 0); //Osc FX
    waveformMixer4.gain(3, 0); //Osc FX
    waveformMixer5.gain(3, 0); //Osc FX
    waveformMixer6.gain(3, 0); //Osc FX
    waveformMixer7.gain(3, 0); //Osc FX
    waveformMixer8.gain(3, 0); //Osc FX
    waveformMixer9.gain(3, 0); //Osc FX
    waveformMixer10.gain(3, 0); //Osc FX
    waveformMixer11.gain(3, 0); //Osc FX
    waveformMixer12.gain(3, 0); //Osc FX
    waveformMixer13.gain(3, 0); //Osc FX
    waveformMixer14.gain(3, 0); //Osc FX
    waveformMixer15.gain(3, 0); //Osc FX
    waveformMixer16.gain(3, 0); //Osc FX
    showCurrentParameterPage("Osc FX", "Off");
    digitalWriteFast(OSC_FX_LED, LOW);  // LED off
  }
}

void updateFXAmt()
{
  ensemble.lfoRate(fxAmt);
  showCurrentParameterPage("Effect Amt", String(fxAmt) + " Hz");
}

void updateFXMix()
{
  effectMixerL.gain(0, 1.0f - fxMix); //Dry
  effectMixerL.gain(1, fxMix);       //Wet
  effectMixerR.gain(0, 1.0f - fxMix); //Dry
  effectMixerR.gain(1, fxMix);       //Wet
  showCurrentParameterPage("Effect Mix", String(fxMix));
}

void updatePatchname()
{
  showPatchPage(String(patchNo), patchName);
}

void myPitchBend(byte channel, int bend)
{
  pitchBend.amplitude(bend * 0.5f * pitchBendRange * DIV12 * DIV8192); //)0.5 to give 1oct max - spread of mod is 2oct
}

void myControlChange(byte channel, byte control, byte value)
{
  //Serial.println("MIDI: " + String(control) + " : " + String(value));
  switch (control)
  {
    case CCvolume:
      sgtl5000_1.volume(SGTL_MAXVOLUME * LINEAR[value]); //Headphones
      //sgtl5000_1.lineOutLevel(31 - (18 * LINEAR[value])); //Line out, weird inverted values
      updateVolume(LINEAR[value]);
      break;

    case CCunison:
      value > 0 ? unison = 1 : unison = 0;
      updateUnison();
      break;

    case CCglide:
      glideSpeed = POWER[value];
      updateGlide();
      break;

    case CCpitchenv:
      pitchEnv = LINEARCENTREZERO[value] * OSCMODMIXERMAX;
      updatePitchEnv();
      break;

    case CCoscwaveformA:
      oscWaveformA = getWaveformA(value);
      updateWaveformA();
      break;

    case CCoscwaveformB:
      oscWaveformB = getWaveformB(value);
      updateWaveformB();
      break;

    case CCpitchA:
      oscPitchA = getPitch(value);
      updatePitchA();
      break;

    case CCpitchB:
      oscPitchB = getPitch(value);
      updatePitchB();
      break;

    case CCdetune:
      detune = 1.0f - (MAXDETUNE * LINEAR[value]);
      updateDetune();
      break;

    case CCpwmSource:
      value > 0 ? pwmSource = PWMSOURCEFENV : pwmSource = PWMSOURCELFO;
      updatePWMSource();
      break;

    case CCpwmRate:
      //Uses combination of PWMRate, PWa and PWb
      pwmRate = PWMRATE[value];
      updatePWMRate();
      break;

    case CCpwmAmt:
      //NO FRONT PANEL CONTROL - MIDI CC ONLY
      //Total PWM amount for both oscillators
      pwmAmtA = LINEAR[value];
      pwmAmtB = LINEAR[value];
      updatePWMAmount();
      break;

    case CCpwA:
      pwA = LINEARCENTREZERO[value]; //Bipolar
      pwmAmtA = LINEAR[value];
      updatePWA();
      break;

    case CCpwB:
      pwB = LINEARCENTREZERO[value]; //Bipolar
      pwmAmtB = LINEAR[value];
      updatePWB();
      break;

    case CCoscLevelA:
      oscALevel = LINEAR[value];
      updateOscLevelA();
      break;

    case CCoscLevelB:
      oscBLevel = LINEAR[value];
      updateOscLevelB();
      break;

    case CCnoiseLevel:
      noiseLevel = LINEARCENTREZERO[value];
      updateNoiseLevel();
      break;

    case CCfilterfreq:
      //Pick up
      //if (pickUp && (filterfreqPrevValue <  value - PICKUPTOLERANCE || filterfreqPrevValue >  value + PICKUPTOLERANCE))return;
      filterFreq = FILTERFREQS[value];
      updateFilterFreq();
      // filterfreqPrevValue = value;
      break;

    case CCfilterres:
      //Pick up
      filterRes = (13.9f * POWER[value]) + 1.1f; //If <1.1 there is noise at high cutoff freq
      updateFilterRes();
      break;

    case CCfiltermixer:
      //Pick up
      filterMix = LINEAR_FILTERMIXER[value];
      filterMixStr = LINEAR_FILTERMIXERSTR[value];
      updateFilterMixer();
      break;

    case CCfilterenv:
      //Pick up
      filterEnv = LINEARCENTREZERO[value] * FILTERMODMIXERMAX; //Bipolar
      updateFilterEnv();
      break;

    case CCkeytracking:
      keytrackingAmount = KEYTRACKINGAMT[value];
      updateKeyTracking();
      break;

    case CCmodwheel:
      oscLfoAmt = POWER[value] * modWheelDepth; //Variable LFO amount from mod wheel - Settings Option
      updateModWheel();
      break;

    case CCosclfoamt:
      //Pick up
      oscLfoAmt = POWER[value];
      updateOscLFOAmt();
      break;

    case CCoscLfoRate:
      //Pick up
      if (oscLFOMidiClkSync == 1)
      {
        oscLfoRate = getLFOTempoRate(value);
        oscLFOTimeDivStr = LFOTEMPOSTR[value];
      }
      else
      {
        oscLfoRate = LFOMAXRATE * POWER[value];
      }
      updatePitchLFORate();
      break;

    case CCoscLfoWaveform:
      oscLFOWaveform = getLFOWaveform(value);
      updatePitchLFOWaveform();
      break;

    case CCosclforetrig:
      value > 0 ? oscLfoRetrig = 1 : oscLfoRetrig = 0;
      updatePitchLFORetrig();
      break;

    case CCfilterLFOMidiClkSync:
      value > 0 ? filterLFOMidiClkSync = 1 : filterLFOMidiClkSync = 0;
      updateFilterLFOMidiClkSync();
      break;

    case CCfilterlforate:
      //Pick up
      if (filterLFOMidiClkSync == 1)
      {
        filterLfoRate = getLFOTempoRate(value);
        filterLFOTimeDivStr = LFOTEMPOSTR[value];
      }
      else
      {
        filterLfoRate = LFOMAXRATE * POWER[value];
      }
      updateFilterLfoRate();
      break;

    case CCfilterlfoamt:
      //Pick up
      filterLfoAmt = LINEAR[value] * FILTERMODMIXERMAX;
      updateFilterLfoAmt();
      break;

    case CCfilterlfowaveform:
      filterLfoWaveform = getLFOWaveform(value);
      updateFilterLFOWaveform();
      break;

    case CCfilterlforetrig:
      value > 0 ? filterLfoRetrig = 1 : filterLfoRetrig = 0;
      updateFilterLFORetrig();
      break;

    //MIDI Only
    case CCoscLFOMidiClkSync:
      value > 0 ? oscLFOMidiClkSync = 1 : oscLFOMidiClkSync = 0;
      updatePitchLFOMidiClkSync();
      break;

    case CCfilterattack:
      filterAttack = ENVTIMES[value];
      updateFilterAttack();
      break;

    case CCfilterdecay:
      filterDecay = ENVTIMES[value];
      updateFilterDecay();
      break;

    case CCfiltersustain:
      filterSustain = LINEAR[value];
      updateFilterSustain();
      break;

    case CCfilterrelease:
      filterRelease = ENVTIMES[value];
      updateFilterRelease();
      break;

    case CCampattack:
      ampAttack = ENVTIMES[value];
      updateAttack();
      break;

    case CCampdecay:
      ampDecay = ENVTIMES[value];
      updateDecay();
      break;

    case CCampsustain:
      ampSustain = LINEAR[value];
      updateSustain();
      break;

    case CCamprelease:
      ampRelease = ENVTIMES[value];
      updateRelease();
      break;

    case CCringmod:
      value > 0 ? oscFX = 1 : oscFX = 0;
      updateOscFX();
      break;

    case CCfxamt:
      //Pick up
      fxAmt = ENSEMBLE_LFO[value];
      updateFXAmt();
      break;

    case CCfxmix:
      //Pick up
      fxMix = LINEAR[value];
      updateFXMix();
      break;

    case CCallnotesoff:
      allNotesOff();
      break;
  }
}

void myProgramChange(byte channel, byte program) {
  state = PATCH;
  patchNo = program + 1;
  recallPatch(patchNo);
  Serial.print("MIDI Pgm Change:");
  Serial.println(patchNo);
  state = PARAMETER;
}

void myMIDIClockStart()
{
  MIDIClkSignal = true;
  //Resync LFOs when MIDI Clock starts.
  //When there's a jump to a different
  //part of a track, such as in a DAW, the DAW must have same
  //rhythmic quantisation as Tempo Div.
  if (oscLFOMidiClkSync == 1)
  {
    pitchLfo.sync();
  }
  if (filterLFOMidiClkSync == 1)
  {
    filterLfo.sync();
  }
}

void myMIDIClockStop()
{
  MIDIClkSignal = false;
}

void myMIDIClock()
{
  //This recalculates the LFO frequencies if the tempo changes (MIDI cLock is 24ppq)
  if ((oscLFOMidiClkSync == 1 || filterLFOMidiClkSync == 1) && count > 23)
  {
    MIDIClkSignal = !MIDIClkSignal;
    float timeNow = millis();
    midiClkTimeInterval = (timeNow - previousMillis);
    lfoSyncFreq = 1000.0f / midiClkTimeInterval;
    previousMillis = timeNow;
    if (oscLFOMidiClkSync == 1)pitchLfo.frequency(lfoSyncFreq * lfoTempoValue); //MIDI CC only
    if (filterLFOMidiClkSync == 1)filterLfo.frequency(lfoSyncFreq * lfoTempoValue);
    count = 0;
  }
  if (count < 24) count++; //prevent eventual overflow
}

void recallPatch(int patchNo)
{
  allNotesOff();
  File patchFile = SD.open(String(patchNo).c_str());
  if (!patchFile)
  {
    Serial.println("File not found");
  }
  else
  {
    String data[NO_OF_PARAMS]; //Array of data read in
    recallPatchData(patchFile, data);
    setCurrentPatchData(data);
    patchFile.close();
  }
}

void setCurrentPatchData(String data[]) {
  patchName = data[0];
  oscALevel = data[1].toFloat();
  oscBLevel = data[2].toFloat();
  noiseLevel = data[3].toFloat();
  unison = data[4].toInt();
  oscFX = data[5].toInt();
  detune = data[6].toFloat();
  lfoSyncFreq = data[7].toInt();
  midiClkTimeInterval = data[8].toInt();
  lfoTempoValue = data[9].toFloat();
  keytrackingAmount = data[10].toFloat();
  glideSpeed = data[11].toFloat();
  oscPitchA = data[12].toFloat();
  oscPitchB = data[13].toFloat();
  oscWaveformA = data[14].toInt();
  oscWaveformB = data[15].toInt();
  pwmSource = data[16].toInt();
  pwmAmtA = data[17].toFloat();
  pwmAmtB = data[18].toFloat();
  pwmRate = data[19].toFloat();
  pwA = data[20].toFloat();
  pwB = data[21].toFloat();
  filterRes = data[22].toFloat();
  filterFreq = data[23].toFloat();
  filterMix = data[24].toFloat();
  filterEnv = data[25].toFloat();
  oscLfoAmt = data[26].toFloat();
  oscLfoRate = data[27].toFloat();
  oscLFOWaveform = data[28].toFloat();
  oscLfoRetrig = data[29].toInt();
  oscLFOMidiClkSync = data[30].toFloat(); //MIDI CC Only
  filterLfoRate = data[31].toFloat();
  filterLfoRetrig = data[32].toInt();
  filterLFOMidiClkSync = data[33].toFloat();
  filterLfoAmt = data[34].toFloat();
  filterLfoWaveform = data[35].toFloat();
  filterAttack = data[36].toFloat();
  filterDecay = data[37].toFloat();
  filterSustain = data[38].toFloat();
  filterRelease = data[39].toFloat();
  ampAttack = data[40].toFloat();
  ampDecay = data[41].toFloat();
  ampSustain = data[42].toFloat();
  ampRelease = data[43].toFloat();
  fxAmt = data[44].toFloat();
  fxMix = data[45].toFloat();
  pitchEnv = data[46].toFloat();

  updatePatchname();
  updateUnison();
  updateWaveformA();
  updateWaveformB();
  updatePitchA();
  updatePitchB();
  updateDetune();
  updatePWMSource();
  updatePWMAmount();
  updatePWA();
  updatePWB();
  updatePWMRate();
  updateOscLevelA();
  updateOscLevelB();
  updateNoiseLevel();
  updateFilterFreq();
  updateFilterRes();
  updateFilterMixer();
  updateFilterEnv();
  updateKeyTracking();
  updateOscLFOAmt();
  updatePitchLFORate();
  updatePitchLFOWaveform();
  updatePitchLFOMidiClkSync();
  updateFilterLfoRate();
  updateFilterLfoAmt();
  updateFilterLFOWaveform();
  updateFilterLFOMidiClkSync();
  updateFilterLFORetrig();
  updateFilterAttack();
  updateFilterDecay();
  updateFilterSustain();
  updateFilterRelease();
  updateAttack();
  updateDecay();
  updateSustain();
  updateRelease();
  updateOscFX();
  updateFXAmt();
  updateFXMix();
  updatePitchEnv();
  Serial.print("Set Patch: ");
  Serial.println(patchName);
}

String getCurrentPatchData()
{
  return patchName + "," + String(oscALevel) + "," + String(oscBLevel) + "," + String(noiseLevel) + "," + String(unison) + "," + String(oscFX) + "," + String(detune) + "," + String(lfoSyncFreq) + "," + String(midiClkTimeInterval) + "," + String(lfoTempoValue) + "," + String(keytrackingAmount) + "," + String(glideSpeed) + "," + String(oscPitchA) + "," + String(oscPitchB) + "," + String(oscWaveformA) + "," + String(oscWaveformB) + "," +
         String(pwmSource) + "," + String(pwmAmtA) + "," + String(pwmAmtB) + "," + String(pwmRate) + "," + String(pwA) + "," + String(pwB) + "," + String(filterRes) + "," + String(filterFreq) + "," + String(filterMix) + "," + String(filterEnv) + "," + String(oscLfoAmt) + "," + String(oscLfoRate) + "," + String(oscLFOWaveform) + "," + String(oscLfoRetrig) + "," + String(oscLFOMidiClkSync) + "," + String(filterLfoRate) + "," +
         filterLfoRetrig + "," + filterLFOMidiClkSync + "," + filterLfoAmt + "," + filterLfoWaveform + "," + filterAttack + "," + filterDecay + "," + filterSustain + "," + filterRelease + "," + ampAttack + "," + ampDecay + "," + ampSustain + "," + ampRelease + "," +
         String(fxAmt) + "," + String(fxMix) + "," + String(pitchEnv);
}

void checkMux() {
  delayMicroseconds(ADC_DELAY);
  mux1Read = analogRead(MUX1_S);
  if (mux1Read > (mux1ValuesPrev[muxInput] + QUANTISE_FACTOR) || mux1Read < (mux1ValuesPrev[muxInput] - QUANTISE_FACTOR))
  {
    mux1ValuesPrev[muxInput] = mux1Read;
    mux1Read = (mux1Read >> 3); //Change range to 0-127

    switch (muxInput)
    {
      case MUX1_noiseLevel:
        myControlChange(midiChannel, CCnoiseLevel, mux1Read);
        break;
      case MUX1_pitchLfoRate:
        myControlChange(midiChannel, CCoscLfoRate, mux1Read);
        break;
      case MUX1_pitchLfoWaveform:
        myControlChange(midiChannel, CCoscLfoWaveform, mux1Read);
        break;
      case MUX1_pitchLfoAmount:
        myControlChange(midiChannel, CCosclfoamt, mux1Read);
        break;
      case MUX1_detune:
        myControlChange(midiChannel, CCdetune, mux1Read);
        break;
      case MUX1_oscMix:
        myControlChange(midiChannel, CCoscLevelA, OSCMIXA[mux1Read]);
        myControlChange(midiChannel, CCoscLevelB, OSCMIXB[mux1Read]);
        break;
      case MUX1_filterAttack:
        myControlChange(midiChannel, CCfilterattack, mux1Read);
        break;
      case MUX1_filterDecay:
        myControlChange(midiChannel, CCfilterdecay, mux1Read);
        break;
      case MUX1_pwmAmountA:
        myControlChange(midiChannel, CCpwA, mux1Read);
        break;
      case MUX1_waveformA:
        myControlChange(midiChannel, CCoscwaveformA, mux1Read);
        break;
      case MUX1_pitchA:
        myControlChange(midiChannel, CCpitchA, mux1Read);
        break;
      case MUX1_pwmAmountB:
        myControlChange(midiChannel, CCpwB, mux1Read);
        break;
      case MUX1_waveformB:
        myControlChange(midiChannel, CCoscwaveformB, mux1Read);
        break;
      case MUX1_pitchB:
        myControlChange(midiChannel, CCpitchB, mux1Read);
        break;
      case MUX1_pwmRate:
        myControlChange(midiChannel, CCpwmRate, mux1Read);
        break;
      case MUX1_pitchEnv:
        myControlChange(midiChannel, CCpitchenv, mux1Read);
        break;
    }
  }
  delayMicroseconds(ADC_DELAY);
  mux2Read = analogRead(MUX2_S);
  if (mux2Read > (mux2ValuesPrev[muxInput] + QUANTISE_FACTOR) || mux2Read < (mux2ValuesPrev[muxInput] - QUANTISE_FACTOR))
  {
    mux2ValuesPrev[muxInput] = mux2Read;
    mux2Read = (mux2Read >> 3); //Change range to 0-127

    switch (muxInput)
    {
      case MUX2_attack:
        myControlChange(midiChannel, CCampattack, mux2Read);
        break;
      case MUX2_decay:
        myControlChange(midiChannel, CCampdecay, mux2Read);
        break;
      case MUX2_sustain:
        myControlChange(midiChannel, CCampsustain, mux2Read);
        break;
      case MUX2_release:
        myControlChange(midiChannel, CCamprelease, mux2Read);
        break;
      case MUX2_filterLFOAmount:
        myControlChange(midiChannel, CCfilterlfoamt, mux2Read);
        break;
      case MUX2_FXMix:
        myControlChange(midiChannel, CCfxmix, mux2Read);
        break;
      case MUX2_FXAmount:
        myControlChange(midiChannel, CCfxamt, mux2Read);
        break;
      case MUX2_glide:
        myControlChange(midiChannel, CCglide, mux2Read);
        break;
      case MUX2_filterEnv:
        myControlChange(midiChannel, CCfilterenv, mux2Read);
        break;
      case MUX2_filterRelease:
        myControlChange(midiChannel, CCfilterrelease, mux2Read);
        break;
      case MUX2_filterSustain:
        myControlChange(midiChannel, CCfiltersustain, mux2Read);
        break;
      case MUX2_filterType:
        myControlChange(midiChannel, CCfiltermixer, mux2Read);
        break;
      case MUX2_resonance:
        myControlChange(midiChannel, CCfilterres, mux2Read);
        break;
      case MUX2_cutoff:
        myControlChange(midiChannel, CCfilterfreq, mux2Read);
        break;
      case MUX2_filterLFORate:
        myControlChange(midiChannel, CCfilterlforate, mux2Read);
        break;
      case MUX2_filterLFOWaveform:
        myControlChange(midiChannel, CCfilterlfowaveform, mux2Read);
        break;
    }
  }
  muxInput++;
  if (muxInput >= MUXCHANNELS) {
    muxInput = 0;
    checkVolumePot();//Check here
  }
  digitalWriteFast(MUX_0, muxInput & B0001);
  digitalWriteFast(MUX_1, muxInput & B0010);
  digitalWriteFast(MUX_2, muxInput & B0100);
  digitalWriteFast(MUX_3, muxInput & B1000);
}

void checkVolumePot() {
  delayMicroseconds(ADC_DELAY);
  volumeRead = analogRead(VOLUME_POT);
  if (volumeRead > (volumePrevious + QUANTISE_FACTOR) || volumeRead < (volumePrevious - QUANTISE_FACTOR))
  {
    volumePrevious = volumeRead;
    volumeRead = (volumeRead >> 3); //Change range to 0-127
    myControlChange(midiChannel, CCvolume, volumeRead);
  }
}

void checkSwitches() {
  unisonSwitch.update();
  if (unisonSwitch.fallingEdge())
  {
    unison = !unison;
    myControlChange(midiChannel, CCunison, unison);
  }

  oscFXSwitch.update();
  if (oscFXSwitch.fallingEdge())
  {
    oscFX = !oscFX;
    myControlChange(midiChannel, CCringmod, oscFX);
  }

  filterLFORetrigSwitch.update();
  if (filterLFORetrigSwitch.fallingEdge())
  {
    filterLfoRetrig = !filterLfoRetrig;
    myControlChange(midiChannel, CCfilterlforetrig, filterLfoRetrig);
  }

  tempoSwitch.update();
  if (tempoSwitch.fallingEdge()) {
    filterLFOMidiClkSync = !filterLFOMidiClkSync;
    myControlChange(midiChannel, CCfilterLFOMidiClkSync, filterLFOMidiClkSync);
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
      switch (state)
      {
        case PARAMETER:
          if (patches.size() < PATCHES_LIMIT)
          {
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
    }
    else
    {
      del = false;
    }
  }

  settingsButton.update();
  if (settingsButton.read() == LOW && settingsButton.duration() > HOLD_DURATION)
  {
    //If recall held, set current patch to match current hardware state
    //Reinitialise all hardware values to force them to be re-read if different
    state = REINITIALISE;
    reinitialiseToPanel();
    settingsButton.write(HIGH); //Come out of this state
    reini = true;           //Hack
  }
  else if (settingsButton.risingEdge())
  { //cannot be fallingEdge because holding button won't work
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
    }
    else {
      reini = false;
    }
  }

  backButton.update();
  if (backButton.read() == LOW && backButton.duration() > HOLD_DURATION)
  {
    //If Back button held, Panic - all notes off
    allNotesOff();
    backButton.write(HIGH); //Come out of this state
    panic = true;           //Hack
  }
  else if (backButton.risingEdge())
  { //cannot be fallingEdge because holding button won't work
    if (!panic)
    {
      switch (state)
      {
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
    }
    else
    {
      panic = false;
    }
  }

  //Encoder switch
  recallButton.update();
  if (recallButton.read() == LOW && recallButton.duration() > HOLD_DURATION)
  {
    //If Recall button held, return to current patch setting
    //which clears any changes made
    state = PATCH;
    //Recall the current patch
    patchNo = patches.first().patchNo;
    recallPatch(patchNo);
    state = PARAMETER;
    recallButton.write(HIGH); //Come out of this state
    recall = true;            //Hack
  }
  else if (recallButton.risingEdge()) {
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
          if (patches.size() > 1)
          {
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
    }
    else
    {
      recall = false;
    }
  }
}

void reinitialiseToPanel()
{
  //This sets the current patch to be the same as the current hardware panel state - all the pots
  //The four button controls stay the same state
  //This reinialises the previous hardware values to force a re-read
  muxInput = 0;
  for (int i = 0; i < MUXCHANNELS; i++)
  {
    mux1ValuesPrev[i] = RE_READ;
    mux2ValuesPrev[i] = RE_READ;
  }
  volumePrevious = RE_READ;
  patchName = INITPATCHNAME;
  showPatchPage("Initial", "Panel Settings");
}

void checkEncoder()
{
  //Encoder works with relative inc and dec values
  //Detent encoder goes up in 4 steps, hence +/-3

  long encRead = encoder.read();
  if ((encCW && encRead > encPrevious + 3) || (!encCW && encRead < encPrevious - 3) )
  {
    switch (state)
    {
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
        if (settingsOptions.first().value[settingsValueIndex + 1] != '\0')
          showSettingsPage(settingsOptions.first().option, settingsOptions.first().value[++settingsValueIndex], SETTINGSVALUE);
        break;
    }
    encPrevious = encRead;
  }
  else if ((encCW && encRead < encPrevious - 3) || (!encCW && encRead > encPrevious + 3))
  {
    switch (state)
    {
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

void CPUMonitor() {
  //  Serial.print("RAM:");
  //  Serial.print(freeMemory());
  Serial.print(" CPU:");
  Serial.print(AudioProcessorUsage());
  Serial.print(" (");
  Serial.print(AudioProcessorUsageMax());
  Serial.print(")");
  Serial.print("  MEM:");
  Serial.println(AudioMemoryUsageMax());
  delay(5);//Too fast for Serial Monitor
}

void loop() {
  myusb.Task();
  midi1.read(midiChannel);   //USB HOST MIDI Class Compliant
  //  cli();
  //  usbMIDI.read(midiChannel); //USB Client MIDI
  // sei();
  MIDI.read(midiChannel);    //MIDI 5 Pin DIN
  checkMux();
  checkSwitches();
  checkEncoder();
 // CPUMonitor();
}
