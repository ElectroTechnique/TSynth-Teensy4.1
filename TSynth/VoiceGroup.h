#ifndef TSYNTH_VOICE_GROUP_H
#define TSYNTH_VOICE_GROUP_H

#include <Arduino.h>
#include <vector>
#include <stdint.h>
#include <stddef.h>
#include "Voice.h"

#define VG_FOR_EACH_OSC(CMD) VG_FOR_EACH_VOICE(voices[i]->patch().CMD)
#define VG_FOR_EACH_VOICE(CMD) for (uint8_t i = 0; i < voices.size(); i++){ CMD; }

// These are here because of a Settings.h circular dependency.
#define MONOPHONIC_OFF 0
#define MONOPHONIC_LAST 1
#define MONOPHONIC_FIRST 2
#define MONOPHONIC_HIGHEST 3
#define MONOPHONIC_LOWEST 4
// Legato isn't supported, the envelope state from the previous note needs to transfer to the new note
#define MONOPHONIC_LEGATO 5

const static uint32_t WAVEFORM_PARABOLIC = 103;
const static uint32_t WAVEFORM_HARMONIC = 104;

class VoiceGroup {
    private:
    String patchName;
    uint32_t patchIndex;

    std::vector<Voice*> voices;
    VoiceParams _params;
    uint8_t notesOn;
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

    struct noteStackData {
        uint8_t note;
        uint8_t velocity;
    };

    noteStackData noteStack[NO_OF_VOICES];
    uint8_t top = 0;

    public:
    VoiceGroup(): 
            patchName(""),
            patchIndex(0),
            notesOn(0),
            monophonic(0),
            waveformA(WAVEFORM_SQUARE),
            waveformB(WAVEFORM_SQUARE),
            pitchEnvelope(0.0),
            pwmSource(PWMSOURCELFO),
            pwmAmtA(1.0),
            pwmAmtB(1.0),
            pwA(0.0),
            pwB(0.0),
            pwmRate(0.5),
            oscFX(0),
            oscLevelA(1.0),
            oscLevelB(1.0),
            cutoff(12000.0),
            resonance(1.1),
            filterMixer(0.0),
            filterEnvelope(0.0),
            filterAttack(100.0),
            filterDecay(350.0),
            filterSustain(0.7),
            filterRelease(300.0),
            ampAttack(10.0),
            ampDecay(35.0),
            ampSustain(1.0),
            ampRelease(300.0)

        {
        _params.keytrackingAmount = 0.5; //Half - MIDI CC & settings option
        _params.mixerLevel = 0.0;
        _params.prevNote = 48;
        _params.glideSpeed = 0.0;
        _params.unisonMode = 0;
        _params.chordDetune = 0;
        _params.detune = 0;
        _params.oscPitchA = 0;
        _params.oscPitchB = 12;
    }

    inline uint8_t size()           { return this->voices.size(); }
    inline String getPatchName()    { return this->patchName; }
    inline uint32_t getPatchIndex() { return this->patchIndex; }
    uint32_t getWaveformA()         { return waveformA; }
    uint32_t getWaveformB()         { return waveformB; }
    float getPitchEnvelope()        { return pitchEnvelope; }
    uint8_t getPwmSource()          { return pwmSource; }
    float getPwA()                  { return pwA; }
    float getPwB()                  { return pwB; }
    float getPwmAmtA()              { return pwmAmtA; }
    float getPwmAmtB()              { return pwmAmtB; }
    float getPwmRate()              { return pwmRate; }
    uint8_t getOscFX()              { return oscFX; }
    float getOscLevelA()            { return oscLevelA; }
    float getOscLevelB()            { return oscLevelB; }
    float getCutoff()               { return cutoff; }
    float getResonance()            { return resonance; }
    float getFilterMixer()          { return filterMixer; }
    float getFilterEnvelope()       { return filterEnvelope; }
    float getFilterAttack()         { return filterAttack; }
    float getFilterDecay()          { return filterDecay; }
    float getFilterSustain()        { return filterSustain; }
    float getFilterRelease()        { return filterRelease; }
    float getAmpAttack()            { return ampAttack; }
    float getAmpDecay()             { return ampDecay; }
    float getAmpSustain()           { return ampSustain; }
    float getAmpRelease()           { return ampRelease; }

    inline void setPatchName(String name) {
        this->patchName = name;
    }

    inline void setPatchIndex(uint32_t index) {
        this->patchIndex = index;
    }

    void setWaveformA(uint32_t waveform) {
        if (waveformA == waveform) return;
        waveformA = waveform;

        int temp = waveform;
        if (waveform == WAVEFORM_PARABOLIC) {
            VG_FOR_EACH_OSC(waveformMod_a.arbitraryWaveform(PARABOLIC_WAVE, AWFREQ));
            temp = WAVEFORM_ARBITRARY;
        }
        if (waveform == WAVEFORM_HARMONIC) {
            VG_FOR_EACH_OSC(waveformMod_a.arbitraryWaveform(HARMONIC_WAVE, AWFREQ));
            temp = WAVEFORM_ARBITRARY;
        }

        VG_FOR_EACH_OSC(waveformMod_a.begin(temp))
    }

    void setWaveformB(uint32_t waveform) {
        if (waveformB == waveform) return;
        waveformB = waveform;

        int temp = waveform;
        if (waveform == WAVEFORM_PARABOLIC) {
            VG_FOR_EACH_OSC(waveformMod_b.arbitraryWaveform(PARABOLIC_WAVE, AWFREQ));
            temp = WAVEFORM_ARBITRARY;
        }
        if (waveform == WAVEFORM_HARMONIC) {
            VG_FOR_EACH_OSC(waveformMod_b.arbitraryWaveform(PPG_WAVE, AWFREQ));
            temp = WAVEFORM_ARBITRARY;
        }

        VG_FOR_EACH_OSC(waveformMod_b.begin(temp))
    }

    void setPwmRate(float value) {
        pwmRate = value;

        // TODO: 12 copies of this.
        pwmLfoA.frequency(pwmRate);
        pwmLfoB.frequency(pwmRate);

        if (pwmRate == -10) {
            //Set to fixed PW mode
            this->setPwmMixerALFO(0); //LFO Source off
            this->setPwmMixerBLFO(0);
            this->setPwmMixerAFEnv(0); //Filter Env Source off
            this->setPwmMixerBFEnv(0);
            this->setPwmMixerAPW(1); //Manually adjustable pulse width on
            this->setPwmMixerBPW(1);
        } else if (pwmRate == -5) {
            //Set to Filter Env Mod source
            this->setPWMSource(PWMSOURCEFENV);
            this->setPwmMixerAFEnv(this->getPwmAmtA());
            this->setPwmMixerBFEnv(this->getPwmAmtB());
            this->setPwmMixerAPW(0);
            this->setPwmMixerBPW(0);
        } else {
            this->setPWMSource(PWMSOURCELFO);
            this->setPwmMixerAPW(0);
            this->setPwmMixerBPW(0);
        }
    }

    void setPitchEnvelope(float value) {
        pitchEnvelope = value;
        VG_FOR_EACH_OSC(oscModMixer_a.gain(1, value))
        VG_FOR_EACH_OSC(oscModMixer_b.gain(1, value))
    }

    void setPwmMixerALFO(float value) {
        VG_FOR_EACH_OSC(pwMixer_a.gain(0, value));
    }

    void setPwmMixerBLFO(float value) {
        VG_FOR_EACH_OSC(pwMixer_b.gain(0, value));
    }

    void setPwmMixerAPW(float value) {
        VG_FOR_EACH_OSC(pwMixer_a.gain(1, value));
    }

    void setPwmMixerBPW(float value) {
        VG_FOR_EACH_OSC(pwMixer_b.gain(1, value));
    }

    void setPwmMixerAFEnv(float value) {
        VG_FOR_EACH_OSC(pwMixer_a.gain(2, value));
    }

    void setPwmMixerBFEnv(float value) {
        VG_FOR_EACH_OSC(pwMixer_b.gain(2, value));
    }

    // MIDI-CC Only
    void overridePwmAmount(float value) {
        pwmAmtA = value;
        pwmAmtB = value;
        pwA = 0;
        pwB = 0;
        this->setPwmMixerALFO(value);
        this->setPwmMixerBLFO(value);
    }

    void setPWA(float valuePwA, float valuePwmAmtA) {
        pwA = valuePwA;
        pwmAmtA = valuePwmAmtA;
        if (pwmRate == -10) {
            //fixed PW is enabled
            this->setPwmMixerALFO(0);
            this->setPwmMixerBLFO(0);
            this->setPwmMixerAFEnv(0);
            this->setPwmMixerBFEnv(0);
            this->setPwmMixerAPW(1);
            this->setPwmMixerBPW(1);
        } else {
            this->setPwmMixerAPW(0);
            this->setPwmMixerBPW(0);
            if (pwmSource == PWMSOURCELFO) {
                //PW alters PWM LFO amount for waveform A
                this->setPwmMixerALFO(pwmAmtA);
            } else {
                //PW alters PWM Filter Env amount for waveform A
                this->setPwmMixerAFEnv(pwmAmtA);
            }
        }

        //Prevent silence when pw = +/-1.0 on pulse
        float pwA_Adj = pwA;
        if (pwA > 0.98) pwA_Adj = 0.98f;
        if (pwA < -0.98) pwA_Adj = -0.98f;
        pwa.amplitude(pwA_Adj);
    }

    void setPWB(float valuePwA, float valuePwmAmtA) {
        pwB = valuePwA;
        pwmAmtB = valuePwmAmtA;
        if (pwmRate == -10) {
            //fixed PW is enabled
            this->setPwmMixerALFO(0);
            this->setPwmMixerBLFO(0);
            this->setPwmMixerAFEnv(0);
            this->setPwmMixerBFEnv(0);
            this->setPwmMixerAPW(1);
            this->setPwmMixerBPW(1);
        } else {
            this->setPwmMixerAPW(0);
            this->setPwmMixerBPW(0);
            if (pwmSource == PWMSOURCELFO) {
                //PW alters PWM LFO amount for waveform B
                this->setPwmMixerBLFO(pwmAmtB);
            } else {
                //PW alters PWM Filter Env amount for waveform B
                this->setPwmMixerBFEnv(pwmAmtB);
            }
        }

        //Prevent silence when pw = +/-1.0 on pulse
        float pwB_Adj = pwB;
        if (pwB > 0.98) pwB_Adj = 0.98f;
        if (pwB < -0.98) pwB_Adj = -0.98f;
        pwb.amplitude(pwB_Adj);
    }

    void setPWMSource(uint8_t value) {
        pwmSource = value;
        if (value == PWMSOURCELFO) {
            //Set filter mod to zero
            this->setPwmMixerAFEnv(0);
            this->setPwmMixerBFEnv(0);

            //Set LFO mod
            if (pwmRate > -5) {
                this->setPwmMixerALFO(pwmAmtA);//Set LFO mod
                this->setPwmMixerBLFO(pwmAmtB);//Set LFO mod
            }
        } else {
            this->setPwmMixerALFO(0);//Set LFO mod to zero
            this->setPwmMixerBLFO(0);//Set LFO mod to zero
            if (pwmRate > -5) {
                this->setPwmMixerAFEnv(pwmAmtA);//Set filter mod
                this->setPwmMixerBFEnv(pwmAmtB);//Set filter mod
            }
        }
    }

    void setWaveformMixerLevel(int channel, float level) {
        VG_FOR_EACH_OSC(waveformMixer_.gain(channel, level))
    }

    void setOscModMixerA(int channel, float level) {
        VG_FOR_EACH_OSC(oscModMixer_a.gain(channel, level))
    }

    void setOscModMixerB(int channel, float level) {
        VG_FOR_EACH_OSC(oscModMixer_b.gain(channel, level))
    }

    void setOscFXCombineMode(AudioEffectDigitalCombine::combineMode mode) {
        VG_FOR_EACH_OSC(oscFX_.setCombineMode(mode))
    }

    void setOscLevelA(float value) {
        oscLevelA = value;

        switch (oscFX) {
            case 1://XOR
                setWaveformMixerLevel(0, oscLevelA);//Osc 1 (A)
                setWaveformMixerLevel(3, (oscLevelA + oscLevelB) / 2.0f);//oscFX XOR level
                break;
            case 2://XMod
                //osc A sounds with increasing osc B mod
                if (oscLevelA == 1.0f && oscLevelB <= 1.0f) {
                    setOscModMixerA(3, 1 - oscLevelB);//Feed from Osc 2 (B)
                    setWaveformMixerLevel(0, ONE);//Osc 1 (A)
                    setWaveformMixerLevel(1, 0);//Osc 2 (B)
                }
                break;
            case 0://None
                setOscModMixerA(3, 0);//Feed from Osc 2 (B)
                setWaveformMixerLevel(0, oscLevelA);//Osc 1 (A)
                setWaveformMixerLevel(3, 0);//XOR
                break;
        }
    }

    void setOscLevelB(float value) {
        oscLevelB = value;

        switch (oscFX) {
            case 1://XOR
                setWaveformMixerLevel(1, oscLevelB);//Osc 2 (B)
                setWaveformMixerLevel(3, (oscLevelA + oscLevelB) / 2.0f);//oscFX XOR level
                break;
            case 2://XMod
                //osc B sounds with increasing osc A mod
                if (oscLevelB == 1.0f && oscLevelA < 1.0f) {
                    setOscModMixerB(3, 1 - oscLevelA);//Feed from Osc 1 (A)
                    setWaveformMixerLevel(0, 0);//Osc 1 (A)
                    setWaveformMixerLevel(1, ONE);//Osc 2 (B)
                }
                break;
            case 0://None
                setOscModMixerB(3, 0);//Feed from Osc 1 (A)
                setWaveformMixerLevel(1, oscLevelB);//Osc 2 (B)
                setWaveformMixerLevel(3, 0);//XOR
                break;
        }
    }

    void setOscFX(uint8_t value) {
        oscFX = value;

        if (oscFX == 2) {
            if (oscLevelA == 1.0f && oscLevelB <= 1.0f) {
                setOscModMixerA(3, 1 - oscLevelB);//Feed from Osc 2 (B)
                setWaveformMixerLevel(0, ONE);//Osc 1 (A)
                setWaveformMixerLevel(1, 0);//Osc 2 (B)
            } else {
                setOscModMixerB(3, 1 - oscLevelA);//Feed from Osc 1 (A)
                setWaveformMixerLevel(0, 0);//Osc 1 (A)
                setWaveformMixerLevel(1, ONE);//Osc 2 (B)
            }
            //Set XOR type off
            setOscFXCombineMode(AudioEffectDigitalCombine::OFF);
            setWaveformMixerLevel(3, 0);//XOR
        } else if (oscFX == 1) {
            setOscModMixerA(3, 0);//XMod off
            setOscModMixerB(3, 0);//XMod off
            //XOR 'Ring Mod' type effect
            setOscFXCombineMode(AudioEffectDigitalCombine::XOR);
            setWaveformMixerLevel(3, (oscLevelA + oscLevelB) / 2.0f);//XOR
        } else {
            setOscModMixerA(3, 0);//XMod off
            setOscModMixerB(3, 0);//XMod off
            setOscFXCombineMode(AudioEffectDigitalCombine::OFF);//Set XOR type off
            setWaveformMixerLevel(3, 0);//XOR
        }
    }

    void setCutoff(float value) {
        this->cutoff = value;

        VG_FOR_EACH_OSC(filter_.frequency(value))

        float filterOctave = 0.0;
        //Altering filterOctave to give more cutoff width for deeper bass, but sharper cuttoff at higher frequncies
        if (value <= 2000) {
            filterOctave = 4.0f + ((2000.0f - value) / 710.0f);//More bass
        } else if (value > 2000 && value <= 3500) {
            filterOctave = 3.0f + ((3500.0f - value) / 1500.0f);//Sharper cutoff
        } else if (value > 3500 && value <= 7000) {
            filterOctave = 2.0f + ((7000.0f - value) / 4000.0f);//Sharper cutoff
        } else {
            filterOctave = 1.0f + ((12000.0f - value) / 5100.0f);//Sharper cutoff
        }

        VG_FOR_EACH_OSC(filter_.octaveControl(filterOctave))
    }

    void setResonance(float value) {
        resonance = value;
        VG_FOR_EACH_OSC(filter_.resonance(value))
    }

    void setFilterMixer(float value) {
        filterMixer = value;

        float LP = 1.0f;
        float BP = 0;
        float HP = 0;

        if (value == BANDPASS) {
            //BP mode
            LP = 0;
            BP = 1.0f;
            HP = 0;
        } else {
            //LP-HP mix mode - a notch filter
            LP = 1.0f - value;
            BP = 0;
            HP = value;
        }

        VG_FOR_EACH_VOICE(
            voices[i]->patch().filterMixer_.gain(0, LP);
            voices[i]->patch().filterMixer_.gain(1, BP);
            voices[i]->patch().filterMixer_.gain(2, HP);
        )
    }

    void setFilterModMixer(int channel, float level) {
        VG_FOR_EACH_OSC(filterModMixer_.gain(channel, level))
    }

    void setFilterEnvelope(float value) {
        filterEnvelope = value;
        this->setFilterModMixer(0, filterEnvelope);
    }

    void setFilterAttack(float value) {
        filterAttack = value;
        VG_FOR_EACH_OSC(filterEnvelope_.attack(value))
    }

    void setFilterDecay(float value) {
        filterDecay = value;
        VG_FOR_EACH_OSC(filterEnvelope_.decay(value))
    }

    void setFilterSustain(float value) {
        filterSustain = value;
        VG_FOR_EACH_OSC(filterEnvelope_.sustain(value))
    }

    void setFilterRelease(float value) {
        filterRelease = value;
        VG_FOR_EACH_OSC(filterEnvelope_.release(value))
    }

    void setAmpAttack(float value) {
        ampAttack = value;
        VG_FOR_EACH_OSC(ampEnvelope_.attack(value))
    }

    void setAmpDecay(float value) {
        ampDecay = value;
        VG_FOR_EACH_OSC(ampEnvelope_.decay(value))
    }

    void setAmpSustain(float value) {
        ampSustain = value;
        VG_FOR_EACH_OSC(ampEnvelope_.sustain(value))
    }

    void setAmpRelease(float value) {
        ampRelease = value;
        VG_FOR_EACH_OSC(ampEnvelope_.release(value))
    }

    void setKeytracking(float value) {
        // TODO: Move this out of params to avoid setting it directly without updating the mixer.
        _params.keytrackingAmount = value;
        setFilterModMixer(2, value);
    }

    inline void setMonophonic(uint8_t mode) {
        this->monophonic = mode;
    }

    void setUnisonMode(uint8_t mode) {
        this->_params.unisonMode = mode;
        this->notesOn = 0;
    }

    inline uint8_t unisonNotes() {
        return this->notesOn;
    }

    //
    // Configure the group
    //
    VoiceParams& params() {
        return _params;
    }

    //
    // Modify the group
    //

    // Splits out num voices from the current group into a new group.
    Voice* pop() {
        if (voices.size() == 0) {
            return nullptr;
        }
        Voice* result = voices.back();
        voices.pop_back();
        return result;
    }

    void add(Voice* v) {
        voices.push_back(v);
    }

    // Merges the other VoiceGroup into this one, making additional voices
    // available to this group.
    void merge(VoiceGroup other) {
    }

    //
    // Use the group
    //

    void noteOn(uint8_t note, uint8_t velocity) {
        if (this->monophonic) {
            handleMonophonicNoteOn(note, velocity);
            return;
        }
        noteOn(note, velocity, false);
    }

    void allNotesOn(uint8_t note, int velocity) {
        for (uint8_t i = 0; i < voices.size(); i++) {
            voices[i]->noteOn(note, velocity, this->_params, notesOn);
        }
    }

    void allNotesOff() {
        this->top = 0;
        this->notesOn = 0;
        for (uint8_t i = 0; i < voices.size(); i++) {
            voices[i]->noteOff();
        }
    }

    void updateVoices() {
        for (uint8_t i = 0; i < voices.size(); i++) {
            voices[i]->updateVoice(this->_params, notesOn);
        }
    }

    void noteOff(uint8_t note) {
        if (this->monophonic) {
            handleMonophonicNoteOff(note);
            return;
        }

        if (this->notesOn > 0) this->notesOn --;
        
        switch (this->_params.unisonMode) {
            case 0:
                noteOff(note, false);
                break;
            default:
                noteOff(note, true);
                break;
        }
    }

    // TODO: This helps during refactoring, maybe it will be removed later.
    Voice* operator [](int i) const { return voices[i]; }


    private:

    void handleMonophonicNoteOn(uint8_t note, uint8_t velocity) {
        this->noteStack[this->top].note = note;
        this->noteStack[this->top].velocity = velocity;
        this->top++;

        if (monophonic) {
            int newNoteIdx = -1;
            switch(monophonic) {
                case MONOPHONIC_LEGATO:
                case MONOPHONIC_LAST:
                    newNoteIdx = this->top - 1;
                    break;
                case MONOPHONIC_FIRST:
                    break;
                case MONOPHONIC_HIGHEST: {
                    if (note > monoNote) {
                        newNoteIdx = this->top - 1;
                    }
                    break;
                }
                case MONOPHONIC_LOWEST: {
                    if (note < monoNote) {
                        newNoteIdx = this->top - 1;
                    }
                    break;
                }
            }

            if (newNoteIdx == -1 && this->top == 1) newNoteIdx = 0;

            // New note does not take priority.
            if (newNoteIdx == -1) return;

            uint8_t t = this->top;
            allNotesOff();
            this->top = t;

            this->monoNote = this->noteStack[newNoteIdx].note;
            noteOn(this->noteStack[newNoteIdx].note, this->noteStack[newNoteIdx].velocity, false);

        } else {
            noteOn(note, velocity, false);
        }
    }

    void removeFromStack(uint8_t note) {
        bool shifting = false;
        for (uint8_t i = 0; i < top && i < NO_OF_VOICES; i++) {
            if (!shifting && this->noteStack[i].note == note) {
                shifting = true;
            }
            if (shifting) {
                this->noteStack[i] = this->noteStack[i+1];
            }
        }
    }

    void handleMonophonicNoteOff(uint8_t note) {

        if (this->monophonic) {
            // Remove turned-off note
            removeFromStack(note);
            bool activeNoteTurnedOff = 0 != noteOff(note, true);

            this->top--;
            // If last note is turned off, nothing to retrigger.
            if (this->top < 1) {
                this->top = 0;
                return;
            }

            // No retriggering if this wasn't the active note.
            if (!activeNoteTurnedOff) return;

            int noteIndex = -1;
            switch (this->monophonic)
            {
            case MONOPHONIC_LEGATO:
                // No retriggering.
                break;
            case MONOPHONIC_LAST:
                noteIndex = top - 1;
                break;
            case MONOPHONIC_FIRST:
                noteIndex = 0;
                break;
            case MONOPHONIC_LOWEST:
                noteIndex = 0;
                for(uint8_t i = 0; i < top; i++) {
                    if (this->noteStack[i].note < this->noteStack[noteIndex].note) {
                        noteIndex = i;
                    }
                }
                break;
            case MONOPHONIC_HIGHEST:
                noteIndex = 0;
                for(uint8_t i = 0; i < top; i++) {
                    if (this->noteStack[i].note > this->noteStack[noteIndex].note) {
                        noteIndex = i;
                    }
                }
                break;
            default:
                break;
            }
            
            if (noteIndex >= 0) {
                this->monoNote = this->noteStack[noteIndex].note;
                noteOn(this->noteStack[noteIndex].note, this->noteStack[noteIndex].velocity, true);
            }
            return;
        }
    }

    // Get the oldest free voice, of none free get the oldest active voice.
    Voice* getVoice() {
        Voice* result = nullptr;

        //NoteOn() - Get the oldest free voice, or oldest on voice if all are on. (recent voices may be still on release stage)
        for (uint8_t i = 0; i < voices.size(); i++) {
            if (result == nullptr || !voices[i]->on() || result->on()) {
                if (result == nullptr || voices[i]->timeOn() < result->timeOn()) {
                    result = voices[i];
                }
            }
        }

        return result;
    }

    // Turn off one or more notes, return the number of notes turned off.
    int noteOff(uint8_t note, bool all) {
        int num = 0;    
        //NoteOff() - Get voice number from note
        for (uint8_t i = 0; i < voices.size(); i++) {
            if (voices[i]->note() == note && voices[i]->on() == true) {
                num++;
                voices[i]->noteOff();
                if (! all) {
                    return 1;
                }
            }
        }
        return num;
    }

    void noteOn(uint8_t note, uint8_t velocity, bool monoRetrigger) {
        switch (this->_params.unisonMode) {
            case 0:
                this->_params.mixerLevel = VOICEMIXERLEVEL;
                this->getVoice()->noteOn(note, velocity, this->_params, notesOn);
                break;
            case 1:
                //UNISON MODE
                //1 Note : 0-11
                //2 Notes: 0-5, 6-11
                //3 Notes: 0-3, 4-7, 8-11
                //4 Notes: 0-2, 3/7/8, 4-6, 9-11
                //5 or more: extra notes are ignored and new voices used for 4 notes

                //Retrigger voices
                //      1 2 3 4 5 6 7 8 9 10 11 12
                //    1 x x x x x x x x x x  x  x
                //    2             x x x x  x  x
                //    3         x x x x
                //    4       x       x x
                this->notesOn ++;
                this->_params.mixerLevel = UNISONVOICEMIXERLEVEL;
                switch(this->notesOn) {
                    case 1:
                        allNotesOn(note, velocity);
                        break;
                    case 2:
                        voices[6]->noteOn(note, velocity, this->_params, notesOn);
                        voices[7]->noteOn(note, velocity, this->_params, notesOn);
                        voices[8]->noteOn(note, velocity, this->_params, notesOn);
                        voices[9]->noteOn(note, velocity, this->_params, notesOn);
                        voices[10]->noteOn(note, velocity, this->_params, notesOn);
                        voices[11]->noteOn(note, velocity, this->_params, notesOn);
                        break;
                    case 3:
                        voices[4]->noteOn(note, velocity, this->_params, notesOn);
                        voices[5]->noteOn(note, velocity, this->_params, notesOn);
                        voices[6]->noteOn(note, velocity, this->_params, notesOn);
                        voices[7]->noteOn(note, velocity, this->_params, notesOn);
                        break;
                    case 4:
                        voices[3]->noteOn(note, velocity, this->_params, notesOn);
                        voices[7]->noteOn(note, velocity, this->_params, notesOn);
                        voices[8]->noteOn(note, velocity, this->_params, notesOn);
                        break;
                }
                break;
            case 2:
                this->_params.mixerLevel = UNISONVOICEMIXERLEVEL;
                allNotesOn(note, velocity);
                break;
        }
        this->_params.prevNote = note;
    }
};

#endif
