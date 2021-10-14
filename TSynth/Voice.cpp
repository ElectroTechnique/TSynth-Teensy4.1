#include "Voice.h"

#include "Parameters.h"
#include "synth_waveform.h"

Voice::Voice(Patch& p, uint8_t i): _oscillator(p), _timeOn(-1), _note(0), _velocity(0), _voiceOn(false), _idx(i), _noteId(0) {
    p.waveformMod_a.frequencyModulation(PITCHLFOOCTAVERANGE);
    p.waveformMod_a.begin(WAVEFORMLEVEL, 440.0f, WAVEFORM_SQUARE);
    p.waveformMod_b.frequencyModulation(PITCHLFOOCTAVERANGE);
    p.waveformMod_b.begin(WAVEFORMLEVEL, 440.0f, WAVEFORM_SQUARE);

    //Arbitary waveform needs initializing to something
    p.waveformMod_a.arbitraryWaveform(PARABOLIC_WAVE, AWFREQ);
    p.waveformMod_b.arbitraryWaveform(PARABOLIC_WAVE, AWFREQ);
}

void Voice::updateVoice(VoiceParams &params, uint8_t notesOn) {
    Patch& osc = this->patch();

    if (params.unisonMode == 1) {
        int offset = 2 * this->index();
        osc.waveformMod_a.frequency(NOTEFREQS[this->_note + params.oscPitchA] * (params.detune + ((1 - params.detune) * DETUNE[notesOn - 1][offset])));
        osc.waveformMod_b.frequency(NOTEFREQS[this->_note + params.oscPitchB] * (params.detune + ((1 - params.detune) * DETUNE[notesOn - 1][offset + 1])));
    } else if (params.unisonMode == 2) {
        // TODO: This approach doesn't make sense with voices spread across multiple timbres.
        osc.waveformMod_a.frequency(NOTEFREQS[this->_note + params.oscPitchA + CHORD_DETUNE[this->index()][params.chordDetune]]) ;
        osc.waveformMod_b.frequency(NOTEFREQS[this->_note + params.oscPitchB + CHORD_DETUNE[this->index()][params.chordDetune]] * CDT_DETUNE);
    } else {
        osc.waveformMod_a.frequency(NOTEFREQS[this->_note + params.oscPitchA]);
        osc.waveformMod_b.frequency(NOTEFREQS[this->_note + params.oscPitchB] * params.detune);
    }
}

void Voice::noteOn(uint8_t note, int velocity, VoiceParams &params, uint8_t notesOn, uint8_t id) {
    Patch& osc = this->patch();

    osc.keytracking_.amplitude(note * DIV127 * params.keytrackingAmount);
    mixer->gain(VELOCITY[velocitySens][velocity] * params.mixerLevel);
    osc.filterEnvelope_.noteOn();
    osc.ampEnvelope_.noteOn();
    if (params.glideSpeed > 0 && note != params.prevNote) {
        osc.glide_.amplitude((params.prevNote - note) * DIV24);   //Set glide to previous note frequency (limited to 1 octave max)
        osc.glide_.amplitude(0, params.glideSpeed * GLIDEFACTOR); //Glide to current note
    }

    this->_noteId = id;
    this->_voiceOn = true;
    this->_note = note;
    this->_velocity = velocity;
    this->_timeOn = millis();

    this->updateVoice(params, notesOn);
}

void Voice::noteOff() {
    if (!this->_voiceOn) return;
    this->_voiceOn = false;
    this->_noteId = 0;
    this->patch().filterEnvelope_.noteOff();
    this->patch().ampEnvelope_.noteOff();
}
