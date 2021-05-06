#ifndef TSYNTH_AUDIO_PATCHING_H
#define TSYNTH_AUDIO_PATCHING_H

//waveformX      -->   waveformMixerX   -->   voiceMixer1-3   -->   voiceMixerM  --> volumeMixer
//WAVEFORMLEVEL        oscA/BLevel             VELOCITY    VOICEMIXERLEVEL/UNISONVOICEMIXERLEVEL    volume

// VoiceShared are all of the audio objects shared across a single patch.
struct VoiceShared {
    friend class VoicePath;

    AudioSynthWaveformDc pitchBend;
    AudioSynthWaveformTS pitchLfo;
    AudioMixer4 pitchMixer;
    AudioSynthWaveformTS pwmLfoA;
    AudioSynthWaveformTS pwmLfoB;
    AudioSynthWaveformTS filterLfo;
    AudioSynthWaveformDc pwa;
    AudioSynthWaveformDc pwb;
    AudioMixer4 noiseMixer;

    // Defining this after the voice mixers has a dramatic negative effect... weird!
    AudioEffectEnsemble ensemble;

    AudioMixer4 voiceMixer[3];
    AudioMixer4 voiceMixerM;
    AudioMixer4 effectMixerL;
    AudioMixer4 effectMixerR;

    AudioConnection          patchCord[8] = {
        {pitchBend, 0, pitchMixer, 0},
        {pitchLfo, 0, pitchMixer, 1},
        {voiceMixer[0], 0, voiceMixerM, 0},
        {voiceMixer[1], 0, voiceMixerM, 1},
        {voiceMixer[2], 0, voiceMixerM, 2},
        //{voiceMixerM, 0, effectMixerL, 0},
        //{voiceMixerM, 0, effectMixerR, 0},
        {voiceMixerM, 0, ensemble, 0},
        {ensemble, 0, effectMixerL, 1},
        {ensemble, 0, effectMixerR, 1}
    };

    // Connect global pink/white noise generators to the group noise mixer.
    AudioConnection *pinkNoiseConnection;
    AudioConnection *whiteNoiseConnection;

    // Connect the ensemble L/R output to the global mixers.
    AudioConnection *mixerLConnection;
    AudioConnection *mixerRConnection;

    public:
    VoiceShared() {
    }

    void connectNoise(AudioSynthNoisePink& pink, AudioSynthNoiseWhite& white) {
        delete pinkNoiseConnection;
        delete whiteNoiseConnection;
        pinkNoiseConnection = new AudioConnection(pink, 0, noiseMixer, 0);
        whiteNoiseConnection = new AudioConnection(white, 0, noiseMixer, 0);
    }

    void connectOutput(AudioMixer4& left, AudioMixer4& right, uint8_t index) {
        delete mixerLConnection;
        delete mixerRConnection;

        mixerLConnection = new AudioConnection(voiceMixerM, 0, left, index);
        mixerRConnection = new AudioConnection(voiceMixerM, 0, right, index);
        //mixerLConnection = new AudioConnection(effectMixerL, 0, left, index);
        //mixerRConnection = new AudioConnection(effectMixerR, 0, right, index);
    }

    void connectEffect(AudioMixer4& left, AudioMixer4& right, uint8_t index) {
        //new AudioConnection(effect, 0, left, index);
        //new AudioConnection(voiceMixerM, 0, right, index);
        new AudioConnection(effectMixerL, 0, left, index);
        new AudioConnection(effectMixerR, 0, right, index);
    }
};

struct Mixer {
    private:
    AudioMixer4& mixer;
    uint8_t index;

    public:
    Mixer(AudioMixer4& mixer_, uint8_t index_): mixer(mixer_), index(index_) {}

    void gain(float value) {
        mixer.gain(index, value);
    }
};

// VoicePath are all the audio objects for a single voice.
class VoicePath {
    public:
    AudioEffectEnvelope filterEnvelope_;
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
    AudioEffectEnvelope ampEnvelope_;

    // Dynamic runtime connections when associated with a VoiceShared.
    // pwmLfoA_ -> pwMixer_a, 0
    // pwmLfoB_ -> pwMixer_b, 0
    // pwa_ -> pwMixer_a
    // pwb_ -> pwMixer_b
    // oscPitchMixer -> oscModMixer_a, 0
    // oscPitchMixer -> oscModMixer_b, 0
    AudioConnection *pitchMixerAConnection;
    AudioConnection *pitchMixerBConnection;
    AudioConnection *pwmLfoAConnection;
    AudioConnection *pwmLfoBConnection;
    AudioConnection *filterLfoConnection;
    AudioConnection *pwaConnection;
    AudioConnection *pwbConnection;
    AudioConnection *noiseMixerConnection;
    AudioConnection *ampConnection;

    // Static connections
    AudioConnection patchCord[24] = {
        {keytracking_, 0, filterModMixer_, 2},
        {pwMixer_a, 0, waveformMod_a, 1},
        {pwMixer_b, 0, waveformMod_b, 1},
        {waveformMod_b, 0, waveformMixer_, 1},
        {waveformMod_b, 0, oscFX_, 1},
        {waveformMod_a, 0, waveformMixer_, 0},
        {waveformMod_a, 0, oscFX_, 0},
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
        {oscModMixer_a, 0, waveformMod_a, 0},
        {oscModMixer_b, 0, waveformMod_b, 0},
        {filterEnvelope_, 0, oscModMixer_a, 1},
        {filterEnvelope_, 0, oscModMixer_b, 1},
        {glide_, 0, oscModMixer_a, 2},
        {waveformMod_b, 0, oscModMixer_a, 3},
        {waveformMod_a, 0, oscModMixer_b, 3}
    };

    public:
    VoicePath(){
    }

    Mixer connectTo(VoiceShared& shared, uint8_t index) {
        delete pitchMixerAConnection;
        delete pitchMixerBConnection;
        delete pwmLfoAConnection;
        delete pwmLfoBConnection;
        delete pwaConnection;
        delete pwbConnection;
        delete noiseMixerConnection;
        delete filterLfoConnection;
        delete ampConnection;

        pitchMixerAConnection = new AudioConnection(shared.pitchMixer, 0, oscModMixer_a, 0);
        pitchMixerBConnection = new AudioConnection(shared.pitchMixer, 0, oscModMixer_b, 0);
        pwmLfoAConnection = new AudioConnection(shared.pwmLfoA, 0, pwMixer_a, 0);
        pwmLfoBConnection = new AudioConnection(shared.pwmLfoB, 0, pwMixer_b, 0);
        pwaConnection = new AudioConnection(shared.pwa, 0, pwMixer_a, 1);
        pwbConnection = new AudioConnection(shared.pwb, 0, pwMixer_b, 1);
        noiseMixerConnection = new AudioConnection(shared.noiseMixer, 0, waveformMixer_, 2);
        filterLfoConnection = new AudioConnection(shared.filterLfo, 0, filterModMixer_, 1);

        uint8_t voiceMixerIndex = 0;
        uint8_t indexMod4 = index % 4;
        if (index != 0) voiceMixerIndex = index / 4;
        ampConnection = new AudioConnection(ampEnvelope_, 0, shared.voiceMixer[voiceMixerIndex], indexMod4);
        return Mixer{shared.voiceMixer[voiceMixerIndex], indexMod4};
    }
};

AudioOutputUSB           usbAudio;       //xy=3197,1821
AudioSynthWaveformDc     constant1Dc;    //xy=69,1781
AudioSynthNoisePink      pink;           //xy=1462
AudioSynthNoiseWhite     white;          //xy=1460
AudioAnalyzePeak         peak;           //xy=2756,1817

// Dynamically create the desired number of voices + timbers
VoiceShared voicesShared[2];
VoicePath voices[12];

AudioMixer4              voiceMixer1L;    //xy=2233,581
AudioMixer4              voiceMixer2L;    //xy=2240,1791
AudioMixer4              voiceMixer3L;    //xy=2237,2976
AudioMixer4              voiceMixerML;    //xy=2431,1798
AudioMixer4              voiceMixer1R;    //xy=2233,581
AudioMixer4              voiceMixer2R;    //xy=2240,1791
AudioMixer4              voiceMixer3R;    //xy=2237,2976
AudioMixer4              voiceMixerMR;    //xy=2431,1798
AudioFilterStateVariable dcOffsetFilterL; //xy=2591,1804
AudioFilterStateVariable dcOffsetFilterR; //xy=2591,1804
AudioMixer4              volumeMixerL;    //xy=2774,1756
AudioMixer4              volumeMixerR;    //xy=2774,1756
AudioMixer4 effectMixerL;
AudioMixer4 effectMixerR;

Oscilloscope             scope;
AudioOutputI2S           i2s;            //xy=3190,1737

// Final mixing and output
AudioConnection          patchCord197(voiceMixer1L, 0, voiceMixerML, 0);
AudioConnection          patchCord198(voiceMixer2L, 0, voiceMixerML, 1);
AudioConnection          patchCord215(voiceMixer3L, 0, voiceMixerML, 2);
AudioConnection          patchCord3197(voiceMixer1R, 0, voiceMixerMR, 0);
AudioConnection          patchCord3198(voiceMixer2R, 0, voiceMixerMR, 1);
AudioConnection          patchCord3215(voiceMixer3R, 0, voiceMixerMR, 2);

AudioConnection          patchCord2203(voiceMixerML, 0, dcOffsetFilterL, 0);
AudioConnection          patchCord2204(voiceMixerMR, 1, dcOffsetFilterR, 0);

AudioConnection          patchCord441(dcOffsetFilterL, 2, volumeMixerL, 0);
AudioConnection          patchCord4441(dcOffsetFilterR, 2, volumeMixerR, 0);

AudioConnection          patchCord415(dcOffsetFilterL, 2, scope, 0);
AudioConnection          patchCord416(dcOffsetFilterL, 2, peak, 0);

// TODO: Move effect mixer into VoiceShared.
AudioConnection          patchCord117(volumeMixerR, 0, effectMixerL, 0);
AudioConnection          patchCord118(volumeMixerL, 0, effectMixerR, 0);

AudioConnection          patchCord1117(effectMixerR, 0, usbAudio, 1);
AudioConnection          patchCord1118(effectMixerR, 0, i2s, 1);
AudioConnection          patchCord119(effectMixerL, 0, i2s, 0);
AudioConnection          patchCord120(effectMixerL, 0, usbAudio, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=2353,505

#endif
