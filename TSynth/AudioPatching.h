#ifndef TSYNTH_AUDIO_PATCHING_H
#define TSYNTH_AUDIO_PATCHING_H

#include <vector>
#include "Constants.h"

//waveformX      -->   waveformMixerX   -->   voiceMixer1-3   -->   voiceMixerM  --> volumeMixer
//WAVEFORMLEVEL        oscA/BLevel             VELOCITY    VOICEMIXERLEVEL/UNISONVOICEMIXERLEVEL    volume

// Oscillator configurations.
struct Patch {
    AudioEffectEnvelopeTS filterEnvelope_;

    AudioMixer4 pwMixer_a;
    AudioMixer4 pwMixer_b;

    AudioSynthWaveformDc glide_;

    AudioSynthWaveformDc keytracking_;

    AudioMixer4 oscModMixer_a;
    AudioMixer4 oscModMixer_b;

    AudioSynthWaveformTS filterLfo_;
    AudioSynthWaveformModulatedTS waveformMod_a;
    AudioSynthWaveformModulatedTS waveformMod_b;

    AudioEffectDigitalCombine oscFX_;

    AudioMixer4 waveformMixer_;

    AudioMixer4 filterModMixer_;

    AudioFilterStateVariable filter_;

    AudioMixer4 filterMixer_;

    AudioEffectEnvelopeTS ampEnvelope_;

    AudioConnection connections[25] = {
        {keytracking_, 0, filterModMixer_, 2},
        {pwMixer_a, 0, waveformMod_a, 1},
        {pwMixer_b, 0, waveformMod_b, 1},
        {waveformMod_a, 0, waveformMixer_, 0},
        {waveformMod_a, 0, oscFX_, 0},
        {waveformMod_b, 0, waveformMixer_, 1},
        {waveformMod_b, 0, oscFX_, 1},
        {filterEnvelope_, 0, filterModMixer_, 0},
        {filterEnvelope_, 0, pwMixer_a, 2},
        {filterEnvelope_, 0, pwMixer_b, 2},
        {oscFX_, 0, waveformMixer_, 3},
        {waveformMixer_, 0, filter_, 0},
        {filterModMixer_, 0, filter_, 1},
        {filter_, 0, filterMixer_, 0},
        {filter_, 1, filterMixer_, 1},
        {filter_, 2, filterMixer_, 2},
        {filterMixer_, ampEnvelope_},
        // Mod sources
        {oscModMixer_a, 0, waveformMod_a, 0},
        {oscModMixer_b, 0, waveformMod_b, 0},
        // Pitch env
        {filterEnvelope_, 0, oscModMixer_a, 1},
        {filterEnvelope_, 0, oscModMixer_b, 1},
        // Glide
        {glide_, 0, oscModMixer_a, 2},
        {glide_, 0, oscModMixer_b, 2},
        // X Mod
        {waveformMod_a, 0, oscModMixer_a, 3},
        {waveformMod_b, 0, oscModMixer_b, 3}
    };

    // When added to a voice group, connect PWA/PWB.
    AudioConnection *pitchMixerAConnection;
    AudioConnection *pitchMixerBConnection;
    AudioConnection *pwmLfoAConnection;
    AudioConnection *pwmLfoBConnection;
    AudioConnection *filterLfoConnection;
    AudioConnection *pwaConnection;
    AudioConnection *pwbConnection;
    AudioConnection *noiseMixerConnection;
    AudioConnection *ampConnection;
};

struct PatchShared {
    AudioSynthWaveformDc pitchBend;
    AudioSynthWaveformTS pitchLfo;
    AudioMixer4 pitchMixer;
    AudioSynthWaveformTS pwmLfoA;
    AudioSynthWaveformTS pwmLfoB;
    AudioSynthWaveformTS filterLfo;
    AudioSynthWaveformDc pwa;
    AudioSynthWaveformDc pwb;
    AudioMixer4 noiseMixer;

    AudioMixer4 voiceMixer[3];
    AudioMixer4 voiceMixerM;

    AudioEffectEnsemble ensemble;

    AudioConnection connections[5] = {
        {pitchBend, 0, pitchMixer, 0},
        {pitchLfo, 0, pitchMixer, 1},
        {voiceMixer[0], 0, voiceMixerM, 0},
        {voiceMixer[1], 0, voiceMixerM, 1},
        {voiceMixer[2], 0, voiceMixerM, 2}
    };
};

const uint8_t MAX_NO_TIMBER = 2;
const uint8_t MAX_NO_VOICE = 12;

struct Global {
    AudioOutputUSB           usbAudio;
    AudioSynthWaveformDc     constant1Dc;
    AudioSynthNoisePink      pink;
    AudioSynthNoiseWhite     white;
    AudioAnalyzePeak         peak;
    AudioMixer4              voiceMixer[3];
    AudioMixer4              voiceMixerM;
    AudioFilterStateVariable dcOffsetFilter;
    AudioMixer4              volumeMixer;
    AudioEffectEnsemble      ensemble;
    Oscilloscope             scope;
    AudioMixer4              effectMixerR;
    AudioMixer4              effectMixerL;
    AudioOutputI2S           i2s;

    PatchShared SharedAudio[MAX_NO_TIMBER];
    Patch Oscillators[MAX_NO_VOICE];

    AudioControlSGTL5000     sgtl5000_1;     //xy=2353,505

    std::vector<AudioConnection*> connections;
    Global(float mixerLevel) {
        for (int i = 0; i < MAX_NO_VOICE; i++) {
            connections.push_back(new AudioConnection{constant1Dc, Oscillators[i].filterEnvelope_});
        }
        for (int i = 0; i < MAX_NO_TIMBER; i++) {
            connections.push_back(new AudioConnection{pink, 0, SharedAudio[i].noiseMixer, 0});
            connections.push_back(new AudioConnection{white, 0, SharedAudio[i].noiseMixer, 1});

            uint8_t mixerIdx = 0;
            if (i > 0) mixerIdx = i / 12;
            uint8_t chanIdx = i % 4;
            connections.push_back(new AudioConnection{SharedAudio[i].voiceMixerM, 0, voiceMixer[mixerIdx], chanIdx});
        }

        connections.push_back(new AudioConnection{voiceMixer[0], 0, voiceMixerM, 0});
        connections.push_back(new AudioConnection{voiceMixer[1], 0, voiceMixerM, 1});
        connections.push_back(new AudioConnection{voiceMixer[2], 0, voiceMixerM, 2});
        connections.push_back(new AudioConnection{voiceMixerM, 0, dcOffsetFilter, 0});
        connections.push_back(new AudioConnection{dcOffsetFilter, 2, volumeMixer, 0});
        connections.push_back(new AudioConnection{volumeMixer, 0, ensemble, 0});
        connections.push_back(new AudioConnection{dcOffsetFilter, 2, scope, 0});
        connections.push_back(new AudioConnection{dcOffsetFilter, 2, peak, 0});
        connections.push_back(new AudioConnection{ensemble, 0, effectMixerL, 1});
        connections.push_back(new AudioConnection{ensemble, 1, effectMixerR, 1});
        connections.push_back(new AudioConnection{volumeMixer, 0, effectMixerL, 0});
        connections.push_back(new AudioConnection{volumeMixer, 0, effectMixerR, 0});
        connections.push_back(new AudioConnection{effectMixerR, 0, usbAudio, 1});
        connections.push_back(new AudioConnection{effectMixerR, 0, i2s, 1});
        connections.push_back(new AudioConnection{effectMixerL, 0, i2s, 0});
        connections.push_back(new AudioConnection{effectMixerL, 0, usbAudio, 0});

        constant1Dc.amplitude(1.0);

        voiceMixerM.gain(0, 0.25f);
        voiceMixerM.gain(1, 0.25f);
        voiceMixerM.gain(2, 0.25f);
        voiceMixerM.gain(3, 0.25f);

        pink.amplitude(1.0);
        white.amplitude(1.0);

        voiceMixerM.gain(0, mixerLevel);
        voiceMixerM.gain(1, mixerLevel);
        voiceMixerM.gain(2, mixerLevel);
        voiceMixerM.gain(3, mixerLevel);

        //This removes dc offset (mostly from unison pulse waves) before the ensemble effect
        dcOffsetFilter.octaveControl(1.0f);
        dcOffsetFilter.frequency(12.0f);//Lower values will give clicks on note on/off

        volumeMixer.gain(0, 1.0f);
        volumeMixer.gain(1, 0);
        volumeMixer.gain(2, 0);
        volumeMixer.gain(3, 0);
    }
};

#endif
