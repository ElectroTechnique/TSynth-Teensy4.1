#ifndef TSYNTH_VOICE_GROUP_H
#define TSYNTH_VOICE_GROUP_H

#include <vector>
#include <stdint.h>
#include <stddef.h>
#include "Voice.h"

class VoiceGroup {
    private:
    std::vector<Voice*> voices;
    VoiceParams _params;
    uint8_t notesOn;
    uint8_t prevNote;

    public:
    VoiceGroup(): notesOn(0), prevNote(48) {
    }

    inline uint8_t size() {
        return this->voices.size();
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

    inline uint8_t unisonNotes() {
        return this->notesOn;
    }

    void noteOn(uint8_t note, int velocity) {
        switch (this->_params.unisonMode) {
            case 0:
                this->_params.mixerLevel = VOICEMIXERLEVEL;
                this->getVoice()->noteOn(note, velocity, this->_params);
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
                        voices[6]->noteOn(note, velocity, this->_params);
                        voices[7]->noteOn(note, velocity, this->_params);
                        voices[8]->noteOn(note, velocity, this->_params);
                        voices[9]->noteOn(note, velocity, this->_params);
                        voices[10]->noteOn(note, velocity, this->_params);
                        voices[11]->noteOn(note, velocity, this->_params);
                        break;
                    case 3:
                        voices[4]->noteOn(note, velocity, this->_params);
                        voices[5]->noteOn(note, velocity, this->_params);
                        voices[6]->noteOn(note, velocity, this->_params);
                        voices[7]->noteOn(note, velocity, this->_params);
                        break;
                    case 4:
                        voices[3]->noteOn(note, velocity, this->_params);
                        voices[7]->noteOn(note, velocity, this->_params);
                        voices[8]->noteOn(note, velocity, this->_params);
                        break;
                }
                break;
            case 2:
                this->_params.mixerLevel = UNISONVOICEMIXERLEVEL;
                allNotesOn(note, velocity);
                break;
        }
        this->prevNote = note;
    }

    void allNotesOn(uint8_t note, int velocity) {
        for (uint8_t i = 0; i < voices.size(); i++) {
            voices[i]->noteOn(note, velocity, this->_params);
        }
    }

    void allNotesOff() {
        this->notesOn = 0;
        for (uint8_t i = 0; i < voices.size(); i++) {
            voices[i]->noteOff();
        }
    }

    void updateVoices() {
        for (uint8_t i = 0; i < voices.size(); i++) {
            voices[i]->updateVoice(this->_params);
        }
    }

    void noteOff(uint8_t note) {
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

    // Get the oldest free voice, of none free get the oldest active voice.
    Voice* getVoice() {
        Voice* result = nullptr;

        //NoteOn() - Get the oldest free voice, or oldest on voice if all are on. (recent voices may be still on release stage)
        for (Voice* v : voices) {
            if (result == nullptr || !v->on() || result->on()) {
                if (result == nullptr || v->timeOn() < result->timeOn()) {
                    result = v;
                }
            }
        }

        return result;
    }

    // Turn off one or more notes.
    void noteOff(uint8_t note, bool all) {     
        //NoteOff() - Get voice number from note
        for (Voice* v : voices) {
            if (v->note() == note && v->on() == true) {
                v->noteOff();
                if (! all) {
                    return;
                }
            }
        }
    }
};

#endif