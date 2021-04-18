#ifndef TSYNTH_VOICE_GROUP_H
#define TSYNTH_VOICE_GROUP_H

// These are here because of a Settings.h circular dependency.
#define MONOPHONIC_OFF 0
#define MONOPHONIC_LAST 1
#define MONOPHONIC_FIRST 2
#define MONOPHONIC_HIGHEST 3
#define MONOPHONIC_LOWEST 4
// Legato isn't supported, the envelope state from the previous note needs to transfer to the new note
#define MONOPHONIC_LEGATO 5

#include <vector>
#include <stdint.h>
#include <stddef.h>
#include "Voice.h"

class VoiceGroup {
    private:
    std::vector<Voice*> voices;
    VoiceParams _params{0.0, 0.0, 48, 0.0, 0, 0, 0.98f, 0, 0};
    uint8_t notesOn;
    uint8_t monoNote;
    uint8_t monophonic;

    struct noteStackData {
        uint8_t note;
        uint8_t velocity;
    };

    noteStackData noteStack[NO_OF_VOICES];
    uint8_t top = 0;

    public:
    VoiceGroup(): notesOn(0), monophonic(0) {
    }

    inline uint8_t size() {
        return this->voices.size();
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
            voices[i]->noteOn(note, velocity, this->_params);
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
            voices[i]->updateVoice(this->_params);
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
        this->_params.prevNote = note;
    }
};

#endif