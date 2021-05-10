#ifndef TSYNTH_AUDIO_PATCHING_H
#define TSYNTH_AUDIO_PATCHING_H

//waveformX      -->   waveformMixerX   -->   voiceMixer1-3   -->   voiceMixerM  --> volumeMixer
//WAVEFORMLEVEL        oscA/BLevel             VELOCITY    VOICEMIXERLEVEL/UNISONVOICEMIXERLEVEL    volume

// Make the visual audio design tool work by substituting the *TS locally changed classes by those it knows: 
// but also ensure all redefined class names are undefined within this scope, if lingering defines exist, raise an error:
#if defined(AudioSynthWaveform) || defined(AudioSynthWaveformModulated)
# error "Either AudioSynthWaveform or AudioSynthWaveformModulated are already redefined!"
#endif
#define AudioSynthWaveform          AudioSynthWaveformTS
#define AudioSynthWaveformModulated AudioSynthWaveformModulatedTS


// Oscillator configurations.
struct Patch {
    AudioEffectEnvelope filterEnvelope_;

    AudioMixer4 pwMixer_a;
    AudioMixer4 pwMixer_b;

    AudioSynthWaveformDc glide_;

    AudioSynthWaveformDc keytracking_;

    AudioMixer4 oscModMixer_a;
    AudioMixer4 oscModMixer_b;

    AudioSynthWaveform filterLfo_;
    AudioSynthWaveformModulated waveformMod_a;
    AudioSynthWaveformModulated waveformMod_b;

    AudioEffectDigitalCombine oscFX_;

    AudioMixer4 waveformMixer_;

    AudioMixer4 filterModMixer_;

    AudioFilterStateVariable filter_;

    AudioMixer4 filterMixer_;

    AudioEffectEnvelope ampEnvelope_;

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
    AudioSynthWaveform pitchLfo;
    AudioMixer4 pitchMixer;
    AudioSynthWaveform pwmLfoA;
    AudioSynthWaveform pwmLfoB;
    AudioSynthWaveform filterLfo;
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


// GUItool: begin automatically generated code
AudioOutputUSB           usbAudio;       //xy=3197,1821
AudioSynthWaveformDc     constant1Dc;    //xy=69,1781
AudioSynthNoisePink      pink;           //xy=1462
AudioSynthNoiseWhite     white;          //xy=1460
AudioAnalyzePeak         peak;           //xy=2756,1817
AudioMixer4              voiceMixer1;    //xy=2233,581
AudioMixer4              voiceMixer2;    //xy=2240,1791
AudioMixer4              voiceMixer3;    //xy=2237,2976
AudioMixer4              voiceMixerM;    //xy=2431,1798
AudioFilterStateVariable dcOffsetFilter; //xy=2591,1804
AudioMixer4              volumeMixer;    //xy=2774,1756
AudioEffectEnsemble      ensemble;      //xy=1800,600
Oscilloscope             scope;
AudioMixer4              effectMixerR;   //xy=2984,1823
AudioMixer4              effectMixerL;   //xy=2985,1728
AudioOutputI2S           i2s;            //xy=3190,1737

PatchShared SharedAudio[2];
Patch Oscillators[12];

// 
AudioConnection          patchCord4(constant1Dc, Oscillators[0].filterEnvelope_);
AudioConnection          patchCord1(constant1Dc, Oscillators[1].filterEnvelope_);
AudioConnection          patchCord2(constant1Dc, Oscillators[2].filterEnvelope_);
AudioConnection          patchCord3(constant1Dc, Oscillators[3].filterEnvelope_);
AudioConnection          patchCord199(constant1Dc, Oscillators[4].filterEnvelope_);
AudioConnection          patchCord200(constant1Dc, Oscillators[5].filterEnvelope_);
AudioConnection          patchCord228(constant1Dc, Oscillators[6].filterEnvelope_);
AudioConnection          patchCord229(constant1Dc, Oscillators[7].filterEnvelope_);
AudioConnection          patchCord230(constant1Dc, Oscillators[8].filterEnvelope_);
AudioConnection          patchCord231(constant1Dc, Oscillators[9].filterEnvelope_);
AudioConnection          patchCord232(constant1Dc, Oscillators[10].filterEnvelope_);
AudioConnection          patchCord233(constant1Dc, Oscillators[11].filterEnvelope_);

// Global Shared Audio Connections
AudioConnection          patchCord452(pink, 0, SharedAudio[0].noiseMixer, 0);
AudioConnection          patchCord453(pink, 0, SharedAudio[1].noiseMixer, 0);

AudioConnection          patchCord137(white, 0, SharedAudio[0].noiseMixer, 1);
AudioConnection          patchCord138(white, 0, SharedAudio[1].noiseMixer, 1);

AudioConnection          voiceMixing001(SharedAudio[0].voiceMixerM, 0, voiceMixer1, 0);
AudioConnection          voiceMixing002(SharedAudio[1].voiceMixerM, 0, voiceMixer1, 1);

AudioConnection          patchCord197(voiceMixer1, 0, voiceMixerM, 0);
AudioConnection          patchCord198(voiceMixer2, 0, voiceMixerM, 1);
AudioConnection          patchCord215(voiceMixer3, 0, voiceMixerM, 2);
AudioConnection          patchCord203(voiceMixerM, 0, dcOffsetFilter, 0);
AudioConnection          patchCord441(dcOffsetFilter, 2, volumeMixer, 0);
AudioConnection          patchCord112(volumeMixer, 0, ensemble, 0);
AudioConnection          patchCord415(dcOffsetFilter, 2, scope, 0);
AudioConnection          patchCord416(dcOffsetFilter, 2, peak, 0);
AudioConnection          patchCord113(ensemble, 0, effectMixerL, 1);
AudioConnection          patchCord114(ensemble, 1, effectMixerR, 1);
AudioConnection          patchCord115(volumeMixer, 0, effectMixerL, 0);
AudioConnection          patchCord116(volumeMixer, 0, effectMixerR, 0);
AudioConnection          patchCord117(effectMixerR, 0, usbAudio, 1);
AudioConnection          patchCord118(effectMixerR, 0, i2s, 1);
AudioConnection          patchCord119(effectMixerL, 0, i2s, 0);
AudioConnection          patchCord120(effectMixerL, 0, usbAudio, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=2353,505
// GUItool: end automatically generated code


// Undefine custom classes renaming:
#undef AudioSynthWaveform
#undef AudioSynthWaveformModulated

#endif
