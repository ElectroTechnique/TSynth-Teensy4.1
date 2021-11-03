#pragma once

#include <stdint.h>
#include <stddef.h>
#include "AudioPatching.h"
#include "Constants.h"
#include "Detune.h"
#include "Velocity.h"

// TODO: Store / update these in the VoiceGroup instead of re-initializing for each noteOn.
struct VoiceParams {
    float keytrackingAmount;
    float mixerLevel;
    int prevNote;
    float glideSpeed;
    int unisonMode;
    uint32_t chordDetune;
    float detune;
    int oscPitchA;
    int oscPitchB;
};

class Voice {
    private:
        Patch &_oscillator;
        long _timeOn;
        uint8_t _note;
        float _velocity;
        bool _voiceOn;
        uint8_t _idx;
        uint8_t _noteId;
        Mixer* mixer = nullptr;

    public:
        Voice(Patch& p, uint8_t i);

        // getters and setters
        void setMixer(Mixer* mixer_);
        uint8_t index();
        bool on(); 
        uint8_t note();
        uint8_t noteId();
        uint8_t velocity();
        long timeOn();
        Patch& patch();

        // usage
        void updateVoice(VoiceParams &params, uint8_t notesOn);
        void noteOn(uint8_t note, int velocity, VoiceParams &params, uint8_t notesOn, uint8_t id);
        void noteOff();
};

inline void Voice::setMixer(Mixer* mixer_) {
    delete mixer;
    mixer = mixer_;
}

inline uint8_t Voice::index() {
    return this->_idx;
}

inline bool Voice::on() { 
    return this->_voiceOn;
}

inline uint8_t Voice::note() {
    return this->_note;
}

inline uint8_t Voice::noteId() {
    return this->_noteId;
}

inline uint8_t Voice::velocity() {
    return this->_velocity;
}

inline long Voice::timeOn() {
    return this->_timeOn;
}

inline Patch& Voice::patch() {
    return this->_oscillator;
}

