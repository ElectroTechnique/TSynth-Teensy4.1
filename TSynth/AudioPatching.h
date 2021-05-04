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

// VoiceShared are all of the audio objects shared across a single patch.
struct VoiceShared {
    public:

    AudioSynthWaveformDc pitchBend;
    AudioSynthWaveform pitchLfo;
    AudioMixer4 pitchMixer;
    AudioSynthWaveform pwmLfoA;
    AudioSynthWaveform pwmLfoB;
    AudioSynthWaveform filterLfo;
    AudioSynthWaveformDc pwa;
    AudioSynthWaveformDc pwb;
    AudioMixer4 noiseMixer;

    // Defining this after the voice mixers has a dramatic negative effect... weird!
    AudioEffectEnsemble ensemble;

    AudioMixer4 voiceMixer1;
    AudioMixer4 voiceMixer2;
    AudioMixer4 voiceMixer3;
    AudioMixer4 voiceMixerM;

    AudioConnection          patchCord[6] = {
        {pitchBend, 0, pitchMixer, 0},
        {pitchLfo, 0, pitchMixer, 1},
        {voiceMixer1, 0, voiceMixerM, 0},
        {voiceMixer2, 0, voiceMixerM, 1},
        {voiceMixer3, 0, voiceMixerM, 2},
        {voiceMixerM, 0, ensemble, 0}
    };

    // These go to the global mixer for final output.
    AudioConnection *pinkNoiseConnection;
    AudioConnection *whiteNoiseConnection;
    AudioConnection *ensembleToMixerLConnection;
    AudioConnection *ensembleToMixerRConnection;
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
    AudioSynthWaveform filterLfo_;
    AudioSynthWaveformModulated waveformMod_a;
    AudioSynthWaveformModulated waveformMod_b;
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

        
};

// GUItool: begin automatically generated code
AudioOutputUSB           usbAudio;       //xy=3197,1821
AudioSynthWaveformDc     constant1Dc;    //xy=69,1781
AudioSynthNoisePink      pink;           //xy=1462
AudioSynthNoiseWhite     white;          //xy=1460
AudioAnalyzePeak         peak;           //xy=2756,1817
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

Oscilloscope             scope;
AudioMixer4              effectMixerR;   //xy=2984,1823
AudioMixer4              effectMixerL;   //xy=2985,1728
AudioOutputI2S           i2s;            //xy=3190,1737

// Dynamically create the desired number of voices + timbers
VoiceShared voicesShared[2];
VoicePath voices[12];

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
AudioConnection          patchCord115(volumeMixerL, 0, effectMixerL, 0);
AudioConnection          patchCord116(volumeMixerR, 0, effectMixerR, 0);
AudioConnection          patchCord117(effectMixerR, 0, usbAudio, 1);
AudioConnection          patchCord118(effectMixerR, 0, i2s, 1);
AudioConnection          patchCord119(effectMixerL, 0, i2s, 0);
AudioConnection          patchCord120(effectMixerL, 0, usbAudio, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=2353,505
// GUItool: end automatically generated code

#ifdef OLD_STUFF
// Oscillator configurations.
struct Patch {
    AudioEffectEnvelope &filterEnvelope_; // 1-12

    AudioMixer4 &pwMixer_a;
    AudioMixer4 &pwMixer_b;

    AudioSynthWaveformDc &glide_;

    AudioSynthWaveformDc &keytracking_;

    AudioMixer4 &oscModMixer_a;
    AudioMixer4 &oscModMixer_b;

    AudioSynthWaveform &filterLfo_;
    AudioSynthWaveformModulated &waveformMod_a;
    AudioSynthWaveformModulated &waveformMod_b;

    AudioEffectDigitalCombine &oscFX_;

    AudioMixer4 &waveformMixer_;

    AudioMixer4 &filterModMixer_;

    AudioFilterStateVariable &filter_;

    AudioMixer4 &filterMixer_;

    AudioEffectEnvelope &ampEnvelope_;

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
    AudioSynthWaveformDc &pitchBend;
    AudioSynthWaveform &pitchLfo;
    AudioMixer4 &pitchMixer;
    AudioSynthWaveform &pwmLfoA;
    AudioSynthWaveform &pwmLfoB;
    AudioSynthWaveform& filterLfo;
    AudioSynthWaveformDc& pwa;
    AudioSynthWaveformDc& pwb;
    AudioMixer4& noiseMixer;

    AudioMixer4& voiceMixer1;
    AudioMixer4& voiceMixer2;
    AudioMixer4& voiceMixer3;
    AudioMixer4& voiceMixerM;

    AudioEffectEnsemble& ensemble;
};

PatchShared SharedAudio[12] = {
    {
        pitchBend1,
        pitchLfo1,
        oscPitchMixer1,
        pwmLfoA1,
        pwmLfoB1,
        filterLfo1,
        pwa1,
        pwb1,
        noiseMixer1,
        voiceMixer1_1,
        voiceMixer2_1,
        voiceMixer3_1,
        voiceMixerM_1,
        ensemble1
    },
    {
        pitchBend2,
        pitchLfo2,
        oscPitchMixer2,
        pwmLfoA2,
        pwmLfoB2,
        filterLfo2,
        pwa2,
        pwb2,
        noiseMixer2,
        voiceMixer1_2,
        voiceMixer2_2,
        voiceMixer3_2,
        voiceMixerM_2,
        ensemble2
    },
    {
        pitchBend3,
        pitchLfo3,
        oscPitchMixer3,
        pwmLfoA3,
        pwmLfoB3,
        filterLfo3,
        pwa3,
        pwb3,
        noiseMixer3,
        voiceMixer1_3,
        voiceMixer2_3,
        voiceMixer3_3,
        voiceMixerM_3,
        ensemble3
    },
    {
        pitchBend4,
        pitchLfo4,
        oscPitchMixer4,
        pwmLfoA4,
        pwmLfoB4,
        filterLfo4,
        pwa4,
        pwb4,
        noiseMixer4,
        voiceMixer1_4,
        voiceMixer2_4,
        voiceMixer3_4,
        voiceMixerM_4,
        ensemble4
    },
    {
        pitchBend5,
        pitchLfo5,
        oscPitchMixer5,
        pwmLfoA5,
        pwmLfoB5,
        filterLfo5,
        pwa5,
        pwb5,
        noiseMixer5,
        voiceMixer1_5,
        voiceMixer2_5,
        voiceMixer3_5,
        voiceMixerM_5,
        ensemble5
    },
    {
        pitchBend6,
        pitchLfo6,
        oscPitchMixer6,
        pwmLfoA6,
        pwmLfoB6,
        filterLfo6,
        pwa6,
        pwb6,
        noiseMixer6,
        voiceMixer1_6,
        voiceMixer2_6,
        voiceMixer3_6,
        voiceMixerM_6,
        ensemble6
    },
    {
        pitchBend7,
        pitchLfo7,
        oscPitchMixer7,
        pwmLfoA7,
        pwmLfoB7,
        filterLfo7,
        pwa7,
        pwb7,
        noiseMixer7,
        voiceMixer1_7,
        voiceMixer2_7,
        voiceMixer3_7,
        voiceMixerM_7,
        ensemble7
    },
    {
        pitchBend8,
        pitchLfo8,
        oscPitchMixer8,
        pwmLfoA8,
        pwmLfoB8,
        filterLfo8,
        pwa8,
        pwb8,
        noiseMixer8,
        voiceMixer1_8,
        voiceMixer2_8,
        voiceMixer3_8,
        voiceMixerM_8,
        ensemble8
    },
    {
        pitchBend9,
        pitchLfo9,
        oscPitchMixer9,
        pwmLfoA9,
        pwmLfoB9,
        filterLfo9,
        pwa9,
        pwb9,
        noiseMixer9,
        voiceMixer1_9,
        voiceMixer2_9,
        voiceMixer3_9,
        voiceMixerM_9,
        ensemble9
    },
    {
        pitchBend10,
        pitchLfo10,
        oscPitchMixer10,
        pwmLfoA10,
        pwmLfoB10,
        filterLfo10,
        pwa10,
        pwb9,
        noiseMixer10,
        voiceMixer1_10,
        voiceMixer2_10,
        voiceMixer3_10,
        voiceMixerM_10,
        ensemble10
    },
    {
        pitchBend11,
        pitchLfo11,
        oscPitchMixer11,
        pwmLfoA11,
        pwmLfoB11,
        filterLfo11,
        pwa11,
        pwb9,
        noiseMixer11,
        voiceMixer1_11,
        voiceMixer2_11,
        voiceMixer3_11,
        voiceMixerM_11,
        ensemble11
    },
    {
        pitchBend12,
        pitchLfo12,
        oscPitchMixer12,
        pwmLfoA12,
        pwmLfoB12,
        filterLfo12,
        pwa12,
        pwb9,
        noiseMixer12,
        voiceMixer1_12,
        voiceMixer2_12,
        voiceMixer3_12,
        voiceMixerM_12,
        ensemble12
    }
};


//AudioSynthWaveform* filterLfo[12] = {&filterLfo1, &filterLfo2, &filterLfo}
//AudioSynthWaveformDc* pwa[12] = {&pwa1, &pwa2, &pwa3, &pwa4, &pwa5, &pwa6, &pwa7, &pwa8, &pwa9, &pwa10, &pwa11, &pwa12 };
//AudioSynthWaveformDc* pwb[12] = {&pwb1, &pwb2, &pwb3, &pwb4, &pwb5, &pwb6, &pwb7, &pwb8, &pwb9, &pwb10, &pwb11, &pwb12 };
//AudioMixer4* noiseMixer[12] = {&noiseMixer1, &noiseMixer2, &noiseMixer3, &noiseMixer4, &noiseMixer5, &noiseMixer6, &noiseMixer7, &noiseMixer8, &noiseMixer9, &noiseMixer10, &noiseMixer11, &noiseMixer12 };

// The 12 oscillator pairs in one data structure to allow for easier programming.
Patch Oscillators[12] = {
    {
        filterEnvelope1,
        pwMixer1a,
        pwMixer1b,
        glide1,
        keytracking1,
        oscModMixer1a,
        oscModMixer1b,
        filterLfo1,
        waveformMod1a,
        waveformMod1b,
        oscFX1,
        waveformMixer1,
        filterModMixer1,
        filter1,
        filterMixer1,
        ampEnvelope1,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    },
    {
        filterEnvelope2,
        pwMixer2a,
        pwMixer2b,
        glide2,
        keytracking2,
        oscModMixer2a,
        oscModMixer2b,
        filterLfo2,
        waveformMod2a,
        waveformMod2b,
        oscFX2,
        waveformMixer2,
        filterModMixer2,
        filter2,
        filterMixer2,
        ampEnvelope2,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    },
    {
        filterEnvelope3,
        pwMixer3a,
        pwMixer3b,
        glide3,
        keytracking3,
        oscModMixer3a,
        oscModMixer3b,
        filterLfo3,
        waveformMod3a,
        waveformMod3b,
        oscFX3,
        waveformMixer3,
        filterModMixer3,
        filter3,
        filterMixer3,
        ampEnvelope3,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    },
    {
        filterEnvelope4,
        pwMixer4a,
        pwMixer4b,
        glide4,
        keytracking4,
        oscModMixer4a,
        oscModMixer4b,
        filterLfo4,
        waveformMod4a,
        waveformMod4b,
        oscFX4,
        waveformMixer4,
        filterModMixer4,
        filter4,
        filterMixer4,
        ampEnvelope4,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    },
    {
        filterEnvelope5,
        pwMixer5a,
        pwMixer5b,
        glide5,
        keytracking5,
        oscModMixer5a,
        oscModMixer5b,
        filterLfo5,
        waveformMod5a,
        waveformMod5b,
        oscFX5,
        waveformMixer5,
        filterModMixer5,
        filter5,
        filterMixer5,
        ampEnvelope5,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    },
    {
        filterEnvelope6,
        pwMixer6a,
        pwMixer6b,
        glide6,
        keytracking6,
        oscModMixer6a,
        oscModMixer6b,
        filterLfo6,
        waveformMod6a,
        waveformMod6b,
        oscFX6,
        waveformMixer6,
        filterModMixer6,
        filter6,
        filterMixer6,
        ampEnvelope6,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    },
    {
        filterEnvelope7,
        pwMixer7a,
        pwMixer7b,
        glide7,
        keytracking7,
        oscModMixer7a,
        oscModMixer7b,
        filterLfo7,
        waveformMod7a,
        waveformMod7b,
        oscFX7,
        waveformMixer7,
        filterModMixer7,
        filter7,
        filterMixer7,
        ampEnvelope7,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    },
    {
        filterEnvelope8,
        pwMixer8a,
        pwMixer8b,
        glide8,
        keytracking8,
        oscModMixer8a,
        oscModMixer8b,
        filterLfo8,
        waveformMod8a,
        waveformMod8b,
        oscFX8,
        waveformMixer8,
        filterModMixer8,
        filter8,
        filterMixer8,
        ampEnvelope8,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    },
    {
        filterEnvelope9,
        pwMixer9a,
        pwMixer9b,
        glide9,
        keytracking9,
        oscModMixer9a,
        oscModMixer9b,
        filterLfo9,
        waveformMod9a,
        waveformMod9b,
        oscFX9,
        waveformMixer9,
        filterModMixer9,
        filter9,
        filterMixer9,
        ampEnvelope9,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    },
    {
        filterEnvelope10,
        pwMixer10a,
        pwMixer10b,
        glide10,
        keytracking10,
        oscModMixer10a,
        oscModMixer10b,
        filterLfo10,
        waveformMod10a,
        waveformMod10b,
        oscFX10,
        waveformMixer10,
        filterModMixer10,
        filter10,
        filterMixer10,
        ampEnvelope10,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    },
    {
        filterEnvelope11,
        pwMixer11a,
        pwMixer11b,
        glide11,
        keytracking11,
        oscModMixer11a,
        oscModMixer11b,
        filterLfo11,
        waveformMod11a,
        waveformMod11b,
        oscFX11,
        waveformMixer11,
        filterModMixer11,
        filter11,
        filterMixer11,
        ampEnvelope11,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    },
    {
        filterEnvelope12,
        pwMixer12a,
        pwMixer12b,
        glide12,
        keytracking12,
        oscModMixer12a,
        oscModMixer12b,
        filterLfo12,
        waveformMod12a,
        waveformMod12b,
        oscFX12,
        waveformMixer12,
        filterModMixer12,
        filter12,
        filterMixer12,
        ampEnvelope12,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    }
};
#endif

// Undefine custom classes renaming:
#undef AudioSynthWaveform
#undef AudioSynthWaveformModulated

#endif
