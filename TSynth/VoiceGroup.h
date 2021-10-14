#pragma once

#include <Arduino.h>
#include <vector>
#include <stdint.h>
#include <stddef.h>
#include "Voice.h"
#include "MonoNoteHistory.h"
#include "Constants.h"
#include "AudioPatching.h"

class VoiceGroup
{
private:
    String patchName;
    uint32_t patchIndex;

    // Audio Objects
    PatchShared &shared;
    std::vector<Voice *> voices;

    // Patch Configs
    bool midiClockSignal; // midiCC clock
    bool filterLfoMidiClockSync;
    bool pitchLFOMidiClockSync;

    VoiceParams _params;
    uint8_t unisonNotesOn;
    uint8_t voicesOn;
    uint8_t monoNote;
    uint8_t monophonic;
    uint8_t waveformA;
    uint8_t waveformB;
    float pitchEnvelope;
    uint8_t pwmSource;
    float pwmAmtA;
    float pwmAmtB;
    float pwA;
    float pwB;
    float pwmRate;
    uint8_t oscFX;
    float oscLevelA;
    float oscLevelB;
    float cutoff;
    float resonance;
    float filterMixer;
    float filterEnvelope;
    float filterAttack;
    float filterDecay;
    float filterSustain;
    float filterRelease;
    float ampAttack;
    float ampDecay;
    float ampSustain;
    float ampRelease;
    float filterLfoRetrig;
    float filterLfoRate;
    float filterLfoAmt;
    uint8_t filterLfoWaveform;
    float pinkLevel;
    float whiteLevel;
    uint8_t pitchLfoWaveform;
    bool pitchLfoRetrig;
    float pitchLfoAmount;
    float pitchLfoRate;
    float modWhAmount;
    float effectAmount;
    float effectMix;

    // Used to remember active mono notes.
    MonoNoteHistory noteStack;

public:
    // TODO: This helps during refactoring, maybe it will be removed later.
    Voice *operator[](int i) const { return voices[i]; }

    VoiceGroup(PatchShared &shared_);

    uint8_t size();
    String getPatchName();
    bool getFilterLfoMidiClockSync();
    bool getPitchLfoMidiClockSync();
    inline uint32_t getPatchIndex();
    float getKeytrackingAmount();
    uint8_t getMonophonicMode();
    uint32_t getWaveformA();
    uint32_t getWaveformB();
    float getPitchEnvelope();
    uint8_t getPwmSource();
    float getPwA();
    float getPwB();
    float getPwmAmtA();
    float getPwmAmtB();
    float getPwmRate();
    uint8_t getOscFX();
    float getOscLevelA();
    float getOscLevelB();
    float getCutoff();
    float getResonance();
    float getFilterMixer();
    float getFilterEnvelope();
    float getFilterAttack();
    float getFilterDecay();
    float getFilterSustain();
    float getFilterRelease();
    float getAmpAttack();
    float getAmpDecay();
    float getAmpSustain();
    float getAmpRelease();
    bool getFilterLfoRetrig();
    float getFilterLfoRate();
    float getFilterLfoAmt();
    uint32_t getFilterLfoWaveform();
    float getPinkNoiseLevel();
    float getWhiteNoiseLevel();
    bool getPitchLfoRetrig();
    uint32_t getPitchLfoWaveform();
    float getPitchLfoAmount();
    float getPitchLfoRate();
    float getModWhAmount();
    float getEffectAmount();
    float getEffectMix();

    void setPatchName(String name);
    void setPatchIndex(uint32_t index);

    void setWaveformA(uint32_t waveform);
    void setWaveformB(uint32_t waveform);

    void setPwmRate(float value);
    void setPitchEnvelope(float value);
    void setPwmMixerALFO(float value);
    void setPwmMixerBLFO(float value);
    void setPwmMixerAPW(float value);
    void setPwmMixerBPW(float value);
    void setPwmMixerAFEnv(float value);
    void setPwmMixerBFEnv(float value);

    // MIDI-CC Only
    void overridePwmAmount(float value);
    void setPWA(float valuePwA, float valuePwmAmtA);
    void setPWB(float valuePwA, float valuePwmAmtA);
    void setPWMSource(uint8_t value);
    void setWaveformMixerLevel(int channel, float level);
    void setOscModMixerA(int channel, float level);
    void setOscModMixerB(int channel, float level);
    void setOscFXCombineMode(AudioEffectDigitalCombine::combineMode mode);
    void setOscLevelA(float value);
    void setOscLevelB(float value);
    void setOscFX(uint8_t value);
    void setCutoff(float value);
    void setResonance(float value);
    void setFilterMixer(float value);
    void setFilterModMixer(int channel, float level);
    void setFilterEnvelope(float value);
    void setFilterAttack(float value);
    void setFilterDecay(float value);
    void setFilterSustain(float value);
    void setFilterRelease(float value);
    void setAmpAttack(float value);
    void setAmpDecay(float value);
    void setAmpSustain(float value);
    void setAmpRelease(float value);
    void setKeytracking(float value);
    void setFilterLfoRetrig(bool value);
    void setFilterLfoRate(float value);
    void setFilterLfoAmt(float value);
    void setFilterLfoWaveform(int waveform);
    void setPinkNoiseLevel(float value);
    void setWhiteNoiseLevel(float value);
    void setPitchLfoRetrig(bool value);
    void setPitchLfoWaveform(uint32_t waveform);
    void setPitchLfoAmount(float value);
    void setPitchLfoRate(float value);
    void setModWhAmount(float value);
    void setEffectAmount(float value);
    void setEffectMix(float value);
    void setMonophonic(uint8_t mode);
    void setUnisonMode(uint8_t mode);
    void setFilterLfoMidiClockSync(bool value);
    void setPitchLfoMidiClockSync(bool value);
    uint8_t unisonNotes();

    //
    // Configure the group
    //
    VoiceParams &params();

    //
    // Modify the group
    //

    // Splits out num voices from the current group into a new group.
    Voice* pop();
    void add(Voice *v);

    // Merges the other VoiceGroup into this one, making additional voices
    // available to this group.
    void merge(VoiceGroup other);

    //
    // Use the group
    //

    void noteOn(uint8_t note, uint8_t velocity);
    void allNotesOn(uint8_t note, int velocity, uint8_t id);
    void allNotesOff();
    void updateVoices();
    void noteOff(uint8_t note);
    void pitchBend(float amount);
    void midiClockStart();
    void midiClockStop();
    void midiClock(float frequency);
    void closeEnvelopes();

private:
    void handleMonophonicNoteOn(uint8_t note, uint8_t velocity);
    void handleMonophonicNoteOff(uint8_t note);

    // Get the oldest free voice, of none free get the oldest active voice.
    Voice *getVoice();

    // Turn off one or more notes, return the number of notes turned off.
    int noteOff(uint8_t note, bool all);

    void noteOn(uint8_t note, uint8_t velocity, bool monoRetrigger);
};

inline uint8_t  VoiceGroup::size()                      { return this->voices.size(); }
inline String   VoiceGroup::getPatchName()              { return this->patchName; }
inline bool     VoiceGroup::getFilterLfoMidiClockSync() { return this->filterLfoMidiClockSync; }
inline bool     VoiceGroup::getPitchLfoMidiClockSync()  { return this->pitchLFOMidiClockSync; }
inline uint32_t VoiceGroup::getPatchIndex()             { return this->patchIndex; }
inline float    VoiceGroup::getKeytrackingAmount()      { return this->_params.keytrackingAmount; }
inline uint8_t  VoiceGroup::getMonophonicMode()         { return this->monophonic; }
inline uint32_t VoiceGroup::getWaveformA()              { return this->waveformA; }
inline uint32_t VoiceGroup::getWaveformB()              { return this->waveformB; }
inline float    VoiceGroup::getPitchEnvelope()          { return this->pitchEnvelope; }
inline uint8_t  VoiceGroup::getPwmSource()              { return this->pwmSource; }
inline float    VoiceGroup::getPwA()                    { return this->pwA; }
inline float    VoiceGroup::getPwB()                    { return this->pwB; }
inline float    VoiceGroup::getPwmAmtA()                { return this->pwmAmtA; }
inline float    VoiceGroup::getPwmAmtB()                { return this->pwmAmtB; }
inline float    VoiceGroup::getPwmRate()                { return this->pwmRate; }
inline uint8_t  VoiceGroup::getOscFX()                  { return this->oscFX; }
inline float    VoiceGroup::getOscLevelA()              { return this->oscLevelA; }
inline float    VoiceGroup::getOscLevelB()              { return this->oscLevelB; }
inline float    VoiceGroup::getCutoff()                 { return this->cutoff; }
inline float    VoiceGroup::getResonance()              { return this->resonance; }
inline float    VoiceGroup::getFilterMixer()            { return this->filterMixer; }
inline float    VoiceGroup::getFilterEnvelope()         { return this->filterEnvelope; }
inline float    VoiceGroup::getFilterAttack()           { return this->filterAttack; }
inline float    VoiceGroup::getFilterDecay()            { return this->filterDecay; }
inline float    VoiceGroup::getFilterSustain()          { return this->filterSustain; }
inline float    VoiceGroup::getFilterRelease()          { return this->filterRelease; }
inline float    VoiceGroup::getAmpAttack()              { return this->ampAttack; }
inline float    VoiceGroup::getAmpDecay()               { return this->ampDecay; }
inline float    VoiceGroup::getAmpSustain()             { return this->ampSustain; }
inline float    VoiceGroup::getAmpRelease()             { return this->ampRelease; }
inline bool     VoiceGroup::getFilterLfoRetrig()        { return this->filterLfoRetrig; }
inline float    VoiceGroup::getFilterLfoRate()          { return this->filterLfoRate; }
inline float    VoiceGroup::getFilterLfoAmt()           { return this->filterLfoAmt; }
inline uint32_t VoiceGroup::getFilterLfoWaveform()      { return this->filterLfoWaveform; }
inline float    VoiceGroup::getPinkNoiseLevel()         { return this->pinkLevel; }
inline float    VoiceGroup::getWhiteNoiseLevel()        { return this->whiteLevel; }
inline bool     VoiceGroup::getPitchLfoRetrig()         { return this->pitchLfoRetrig; }
inline uint32_t VoiceGroup::getPitchLfoWaveform()       { return this->pitchLfoWaveform; }
inline float    VoiceGroup::getPitchLfoAmount()         { return this->pitchLfoAmount; }
inline float    VoiceGroup::getPitchLfoRate()           { return this->pitchLfoRate; }
inline float    VoiceGroup::getModWhAmount()            { return this->modWhAmount; }
inline float    VoiceGroup::getEffectAmount()           { return this->effectAmount; }
inline float    VoiceGroup::getEffectMix()              { return this->effectMix; }

inline void VoiceGroup::setPatchName(String name)
{
    this->patchName = name;
}

inline void VoiceGroup::setPatchIndex(uint32_t index)
{
    this->patchIndex = index;
}

inline void VoiceGroup::setFilterLfoMidiClockSync(bool value)
{
    filterLfoMidiClockSync = value;
}

inline void VoiceGroup::setPitchLfoMidiClockSync(bool value)
{
    pitchLFOMidiClockSync = value;
}

inline uint8_t VoiceGroup::unisonNotes()
{
    return this->unisonNotesOn;
}

inline VoiceParams& VoiceGroup::params()
{
    return _params;
}

