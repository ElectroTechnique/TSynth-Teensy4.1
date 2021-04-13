#ifndef TSYNTH_VOICE_H
#define TSYNTH_VOICE_H

#include <stdint.h>
#include <stddef.h>
#include "AudioPatching.h"

class Voice {
    private:
        Patch &_oscillator;
        long _timeOn;
        uint8_t _note;
        bool _voiceOn;

    public:
        Voice(Patch& p): _oscillator(p), _timeOn(-1), _note(0), _voiceOn(false)
        {
        }

        inline bool on() { 
            return _voiceOn;
        }

        inline uint8_t note() {
            return _note;
        }

        inline long timeOn() {
            return _timeOn;
        }

        inline Patch& patch() {
            return _oscillator;
        }

        void noteOn(uint8_t note) {
            this->_voiceOn = true;
            this->_note = note;
            this->_timeOn = millis();
        }

        void noteOff() {
            this->_voiceOn = false;
        }
};

#endif