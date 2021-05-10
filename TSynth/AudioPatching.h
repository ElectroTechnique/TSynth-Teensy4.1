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

PatchShared SharedAudio[12];
Patch Oscillators[12];

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

// These are now dynamic.
// pwmLfoA_ -> pwMixer_a, 0
// pwmLfoB_ -> pwMixer_b, 0
// pwa_ -> pwMixer_a
// pwb_ -> pwMixer_b
// oscPitchMixer -> oscModMixer_a, 0
// oscPitchMixer -> oscModMixer_b, 0

AudioConnection          patchCord21(SharedAudio[0].pitchLfo, 0, SharedAudio[0].pitchMixer, 1);
AudioConnection          patchCord23(SharedAudio[1].pitchLfo, 0, SharedAudio[1].pitchMixer, 1);
AudioConnection          patchCord485(SharedAudio[2].pitchLfo, 0, SharedAudio[2].pitchMixer, 1);
AudioConnection          patchCord486(SharedAudio[3].pitchLfo, 0, SharedAudio[3].pitchMixer, 1);
AudioConnection          patchCord464(SharedAudio[4].pitchLfo, 0, SharedAudio[4].pitchMixer, 1);
AudioConnection          patchCord465(SharedAudio[5].pitchLfo, 0, SharedAudio[5].pitchMixer, 1);
AudioConnection          patchCord466(SharedAudio[6].pitchLfo, 0, SharedAudio[6].pitchMixer, 1);
AudioConnection          patchCord467(SharedAudio[7].pitchLfo, 0, SharedAudio[7].pitchMixer, 1);
AudioConnection          patchCord469(SharedAudio[8].pitchLfo, 0, SharedAudio[8].pitchMixer, 1);
AudioConnection          patchCord470(SharedAudio[9].pitchLfo, 0, SharedAudio[9].pitchMixer, 1);
AudioConnection          patchCord471(SharedAudio[10].pitchLfo, 0, SharedAudio[10].pitchMixer, 1);
AudioConnection          patchCord472(SharedAudio[11].pitchLfo, 0, SharedAudio[11].pitchMixer, 1);

AudioConnection          patchCord473(SharedAudio[0].pitchBend, 0, SharedAudio[0].pitchMixer, 0);
AudioConnection          patchCord474(SharedAudio[1].pitchBend, 0, SharedAudio[1].pitchMixer, 0);
AudioConnection          patchCord475(SharedAudio[2].pitchBend, 0, SharedAudio[2].pitchMixer, 0);
AudioConnection          patchCord476(SharedAudio[3].pitchBend, 0, SharedAudio[3].pitchMixer, 0);
AudioConnection          patchCord477(SharedAudio[4].pitchBend, 0, SharedAudio[4].pitchMixer, 0);
AudioConnection          patchCord478(SharedAudio[5].pitchBend, 0, SharedAudio[5].pitchMixer, 0);
AudioConnection          patchCord479(SharedAudio[6].pitchBend, 0, SharedAudio[6].pitchMixer, 0);
AudioConnection          patchCord480(SharedAudio[7].pitchBend, 0, SharedAudio[7].pitchMixer, 0);
AudioConnection          patchCord481(SharedAudio[8].pitchBend, 0, SharedAudio[8].pitchMixer, 0);
AudioConnection          patchCord482(SharedAudio[9].pitchBend, 0, SharedAudio[9].pitchMixer, 0);
AudioConnection          patchCord483(SharedAudio[10].pitchBend, 0, SharedAudio[10].pitchMixer, 0);
AudioConnection          patchCord484(SharedAudio[11].pitchBend, 0, SharedAudio[11].pitchMixer, 0);

AudioConnection          patchCord28(Oscillators[0].keytracking_, 0, Oscillators[0].filterModMixer_, 2);
AudioConnection          patchCord29(Oscillators[1].keytracking_, 0, Oscillators[1].filterModMixer_, 2);
AudioConnection          patchCord30(Oscillators[2].keytracking_, 0, Oscillators[2].filterModMixer_, 2);
AudioConnection          patchCord31(Oscillators[3].keytracking_, 0, Oscillators[3].filterModMixer_, 2);
AudioConnection          patchCord149(Oscillators[4].keytracking_, 0, Oscillators[4].filterModMixer_, 2);
AudioConnection          patchCord150(Oscillators[5].keytracking_, 0, Oscillators[5].filterModMixer_, 2);
AudioConnection          patchCord264(Oscillators[6].keytracking_, 0, Oscillators[6].filterModMixer_, 2);
AudioConnection          patchCord265(Oscillators[7].keytracking_, 0, Oscillators[7].filterModMixer_, 2);
AudioConnection          patchCord266(Oscillators[8].keytracking_, 0, Oscillators[8].filterModMixer_, 2);
AudioConnection          patchCord267(Oscillators[9].keytracking_, 0, Oscillators[9].filterModMixer_, 2);
AudioConnection          patchCord268(Oscillators[10].keytracking_, 0, Oscillators[9].filterModMixer_, 2);
AudioConnection          patchCord269(Oscillators[11].keytracking_, 0, Oscillators[11].filterModMixer_, 2);


AudioConnection          patchCord36(Oscillators[0].pwMixer_a, 0, Oscillators[0].waveformMod_a, 1);
AudioConnection          patchCord37(Oscillators[0].pwMixer_b, 0, Oscillators[0].waveformMod_b, 1);
AudioConnection          patchCord49(Oscillators[1].pwMixer_a, 0, Oscillators[1].waveformMod_a, 1);
AudioConnection          patchCord50(Oscillators[1].pwMixer_b, 0, Oscillators[1].waveformMod_b, 1);
AudioConnection          patchCord51(Oscillators[2].pwMixer_a, 0, Oscillators[2].waveformMod_a, 1);
AudioConnection          patchCord38(Oscillators[2].pwMixer_b, 0, Oscillators[2].waveformMod_b, 1);
AudioConnection          patchCord47(Oscillators[3].pwMixer_b, 0, Oscillators[3].waveformMod_b, 1);
AudioConnection          patchCord48(Oscillators[3].pwMixer_a, 0, Oscillators[3].waveformMod_a, 1);
AudioConnection          patchCord165(Oscillators[4].pwMixer_a, 0, Oscillators[4].waveformMod_a, 1);
AudioConnection          patchCord166(Oscillators[4].pwMixer_b, 0, Oscillators[4].waveformMod_b, 1);
AudioConnection          patchCord167(Oscillators[5].pwMixer_a, 0, Oscillators[5].waveformMod_a, 1);
AudioConnection          patchCord168(Oscillators[5].pwMixer_b, 0, Oscillators[5].waveformMod_b, 1);
AudioConnection          patchCord276(Oscillators[6].pwMixer_a, 0, Oscillators[6].waveformMod_a, 1);
AudioConnection          patchCord277(Oscillators[6].pwMixer_b, 0, Oscillators[6].waveformMod_b, 1);
AudioConnection          patchCord278(Oscillators[7].pwMixer_a, 0, Oscillators[7].waveformMod_a, 1);
AudioConnection          patchCord279(Oscillators[7].pwMixer_b, 0, Oscillators[7].waveformMod_b, 1);
AudioConnection          patchCord280(Oscillators[8].pwMixer_a, 0, Oscillators[8].waveformMod_a, 1);
AudioConnection          patchCord281(Oscillators[8].pwMixer_b, 0, Oscillators[8].waveformMod_b, 1);
AudioConnection          patchCord282(Oscillators[9].pwMixer_b, 0, Oscillators[9].waveformMod_b, 1);
AudioConnection          patchCord283(Oscillators[9].pwMixer_a, 0, Oscillators[9].waveformMod_a, 1);
AudioConnection          patchCord284(Oscillators[10].pwMixer_a, 0, Oscillators[10].waveformMod_a, 1);
AudioConnection          patchCord285(Oscillators[10].pwMixer_b, 0, Oscillators[10].waveformMod_b, 1);
AudioConnection          patchCord286(Oscillators[11].pwMixer_a, 0, Oscillators[11].waveformMod_a, 1);
AudioConnection          patchCord287(Oscillators[11].pwMixer_b, 0, Oscillators[11].waveformMod_b, 1);


AudioConnection          patchCord52(Oscillators[0].waveformMod_b, 0,   Oscillators[0].waveformMixer_, 1);
AudioConnection          patchCord53(Oscillators[0].waveformMod_b, 0,   Oscillators[0].oscFX_, 1);
AudioConnection          patchCord54(Oscillators[0].waveformMod_a, 0,   Oscillators[0].waveformMixer_, 0);
AudioConnection          patchCord55(Oscillators[0].waveformMod_a, 0,   Oscillators[0].oscFX_, 0);
AudioConnection          patchCord56(Oscillators[1].waveformMod_b, 0,   Oscillators[1].waveformMixer_, 1);
AudioConnection          patchCord57(Oscillators[1].waveformMod_b, 0,   Oscillators[1].oscFX_, 1);
AudioConnection          patchCord60(Oscillators[1].waveformMod_a, 0,   Oscillators[1].waveformMixer_, 0);
AudioConnection          patchCord61(Oscillators[1].waveformMod_a, 0,   Oscillators[1].oscFX_, 0);
AudioConnection          patchCord64(Oscillators[2].waveformMod_a, 0,   Oscillators[2].oscFX_, 0);
AudioConnection          patchCord65(Oscillators[2].waveformMod_a, 0,   Oscillators[2].waveformMixer_, 0);
AudioConnection          patchCord66(Oscillators[2].waveformMod_b, 0,   Oscillators[2].oscFX_, 1);
AudioConnection          patchCord67(Oscillators[2].waveformMod_b, 0,   Oscillators[2].waveformMixer_, 1);
AudioConnection          patchCord58(Oscillators[3].waveformMod_a, 0,   Oscillators[3].waveformMixer_, 0);
AudioConnection          patchCord59(Oscillators[3].waveformMod_a, 0,   Oscillators[3].oscFX_, 0);
AudioConnection          patchCord62(Oscillators[3].waveformMod_b, 0,   Oscillators[3].waveformMixer_, 1);
AudioConnection          patchCord63(Oscillators[3].waveformMod_b, 0,   Oscillators[3].oscFX_, 1);
AudioConnection          patchCord169(Oscillators[4].waveformMod_a, 0,  Oscillators[4].waveformMixer_, 0);
AudioConnection          patchCord170(Oscillators[4].waveformMod_a, 0,  Oscillators[4].oscFX_, 0);
AudioConnection          patchCord171(Oscillators[4].waveformMod_b, 0,  Oscillators[4].waveformMixer_, 1);
AudioConnection          patchCord172(Oscillators[4].waveformMod_b, 0,  Oscillators[4].oscFX_, 1);
AudioConnection          patchCord173(Oscillators[5].waveformMod_a, 0,  Oscillators[5].waveformMixer_, 0);
AudioConnection          patchCord174(Oscillators[5].waveformMod_a, 0,  Oscillators[5].oscFX_, 0);
AudioConnection          patchCord175(Oscillators[5].waveformMod_b, 0,  Oscillators[5].waveformMixer_, 1);
AudioConnection          patchCord176(Oscillators[5].waveformMod_b, 0,  Oscillators[5].oscFX_, 1);
AudioConnection          patchCord300(Oscillators[6].waveformMod_b, 0,  Oscillators[6].waveformMixer_, 1);
AudioConnection          patchCord301(Oscillators[6].waveformMod_b, 0,  Oscillators[6].oscFX_, 1);
AudioConnection          patchCord302(Oscillators[6].waveformMod_a, 0,  Oscillators[6].waveformMixer_, 0);
AudioConnection          patchCord303(Oscillators[6].waveformMod_a, 0,  Oscillators[6].oscFX_, 0);
AudioConnection          patchCord304(Oscillators[7].waveformMod_b, 0,  Oscillators[7].waveformMixer_, 1);
AudioConnection          patchCord305(Oscillators[7].waveformMod_b, 0,  Oscillators[7].oscFX_, 1);
AudioConnection          patchCord308(Oscillators[7].waveformMod_a, 0,  Oscillators[7].waveformMixer_, 0);
AudioConnection          patchCord309(Oscillators[7].waveformMod_a, 0,  Oscillators[7].oscFX_, 0);
AudioConnection          patchCord312(Oscillators[8].waveformMod_a, 0,  Oscillators[8].oscFX_, 0);
AudioConnection          patchCord313(Oscillators[8].waveformMod_a, 0,  Oscillators[8].waveformMixer_, 0);
AudioConnection          patchCord314(Oscillators[8].waveformMod_b, 0,  Oscillators[8].oscFX_, 1);
AudioConnection          patchCord315(Oscillators[8].waveformMod_b, 0,  Oscillators[8].waveformMixer_, 1);
AudioConnection          patchCord306(Oscillators[9].waveformMod_a, 0, Oscillators[9].waveformMixer_, 0);
AudioConnection          patchCord307(Oscillators[9].waveformMod_a, 0, Oscillators[9].oscFX_, 0);
AudioConnection          patchCord310(Oscillators[9].waveformMod_b, 0, Oscillators[9].waveformMixer_, 1);
AudioConnection          patchCord311(Oscillators[8].waveformMod_b, 0, Oscillators[9].oscFX_, 1);
AudioConnection          patchCord316(Oscillators[10].waveformMod_a, 0, Oscillators[10].waveformMixer_, 0);
AudioConnection          patchCord317(Oscillators[10].waveformMod_a, 0, Oscillators[10].oscFX_, 0);
AudioConnection          patchCord318(Oscillators[10].waveformMod_b, 0, Oscillators[10].waveformMixer_, 1);
AudioConnection          patchCord319(Oscillators[10].waveformMod_b, 0, Oscillators[10].oscFX_, 1);
AudioConnection          patchCord320(Oscillators[11].waveformMod_a, 0, Oscillators[11].waveformMixer_, 0);
AudioConnection          patchCord321(Oscillators[11].waveformMod_a, 0, Oscillators[11].oscFX_, 0);
AudioConnection          patchCord322(Oscillators[11].waveformMod_b, 0, Oscillators[11].waveformMixer_, 1);
AudioConnection          patchCord323(Oscillators[11].waveformMod_b, 0, Oscillators[11].oscFX_, 1);


AudioConnection          patchCord71(Oscillators[0].filterEnvelope_, 0,   Oscillators[0].filterModMixer_, 0);
AudioConnection          patchCord72(Oscillators[0].filterEnvelope_, 0,   Oscillators[0].pwMixer_a, 2);
AudioConnection          patchCord73(Oscillators[0].filterEnvelope_, 0,   Oscillators[0].pwMixer_b, 2);
AudioConnection          patchCord77(Oscillators[1].filterEnvelope_, 0,   Oscillators[1].filterModMixer_, 0);
AudioConnection          patchCord78(Oscillators[1].filterEnvelope_, 0,   Oscillators[1].pwMixer_a, 2);
AudioConnection          patchCord79(Oscillators[1].filterEnvelope_, 0,   Oscillators[1].pwMixer_b, 2);
AudioConnection          patchCord74(Oscillators[2].filterEnvelope_, 0,   Oscillators[2].filterModMixer_, 0);
AudioConnection          patchCord75(Oscillators[2].filterEnvelope_, 0,   Oscillators[2].pwMixer_a, 2);
AudioConnection          patchCord76(Oscillators[2].filterEnvelope_, 0,   Oscillators[2].pwMixer_b, 2);
AudioConnection          patchCord68(Oscillators[3].filterEnvelope_, 0,   Oscillators[3].filterModMixer_, 0);
AudioConnection          patchCord69(Oscillators[3].filterEnvelope_, 0,   Oscillators[3].pwMixer_a, 2);
AudioConnection          patchCord70(Oscillators[3].filterEnvelope_, 0,   Oscillators[3].pwMixer_b, 2);
AudioConnection          patchCord177(Oscillators[4].filterEnvelope_, 0,  Oscillators[4].filterModMixer_, 0);
AudioConnection          patchCord178(Oscillators[4].filterEnvelope_, 0,  Oscillators[4].pwMixer_a, 2);
AudioConnection          patchCord179(Oscillators[4].filterEnvelope_, 0,  Oscillators[4].pwMixer_b, 2);
AudioConnection          patchCord180(Oscillators[5].filterEnvelope_, 0,  Oscillators[5].filterModMixer_, 0);
AudioConnection          patchCord181(Oscillators[5].filterEnvelope_, 0,  Oscillators[5].pwMixer_a, 2);
AudioConnection          patchCord182(Oscillators[5].filterEnvelope_, 0,  Oscillators[5].pwMixer_b, 2);
AudioConnection          patchCord327(Oscillators[6].filterEnvelope_, 0,  Oscillators[6].filterModMixer_, 0);
AudioConnection          patchCord328(Oscillators[6].filterEnvelope_, 0,  Oscillators[6].pwMixer_a, 2);
AudioConnection          patchCord329(Oscillators[6].filterEnvelope_, 0,  Oscillators[6].pwMixer_b, 2);
AudioConnection          patchCord333(Oscillators[7].filterEnvelope_, 0,  Oscillators[7].filterModMixer_, 0);
AudioConnection          patchCord334(Oscillators[7].filterEnvelope_, 0,  Oscillators[7].pwMixer_a, 2);
AudioConnection          patchCord335(Oscillators[7].filterEnvelope_, 0,  Oscillators[7].pwMixer_b, 2);
AudioConnection          patchCord330(Oscillators[8].filterEnvelope_, 0,  Oscillators[8].filterModMixer_, 0);
AudioConnection          patchCord331(Oscillators[8].filterEnvelope_, 0,  Oscillators[8].pwMixer_a, 2);
AudioConnection          patchCord332(Oscillators[8].filterEnvelope_, 0,  Oscillators[8].pwMixer_b, 2);
AudioConnection          patchCord324(Oscillators[9].filterEnvelope_, 0, Oscillators[9].filterModMixer_, 0);
AudioConnection          patchCord325(Oscillators[9].filterEnvelope_, 0, Oscillators[9].pwMixer_a, 2);
AudioConnection          patchCord326(Oscillators[9].filterEnvelope_, 0, Oscillators[9].pwMixer_b, 2);
AudioConnection          patchCord336(Oscillators[10].filterEnvelope_, 0, Oscillators[10].filterModMixer_, 0);
AudioConnection          patchCord337(Oscillators[10].filterEnvelope_, 0, Oscillators[10].pwMixer_a, 2);
AudioConnection          patchCord338(Oscillators[10].filterEnvelope_, 0, Oscillators[10].pwMixer_b, 2);
AudioConnection          patchCord339(Oscillators[11].filterEnvelope_, 0, Oscillators[11].filterModMixer_, 0);
AudioConnection          patchCord340(Oscillators[11].filterEnvelope_, 0, Oscillators[11].pwMixer_a, 2);
AudioConnection          patchCord341(Oscillators[11].filterEnvelope_, 0, Oscillators[11].pwMixer_b, 2);


AudioConnection          patchCord80(Oscillators[0].oscFX_, 0, Oscillators[0].waveformMixer_, 3);
AudioConnection          patchCord81(Oscillators[1].oscFX_, 0, Oscillators[1].waveformMixer_, 3);
AudioConnection          patchCord83(Oscillators[2].oscFX_, 0, Oscillators[2].waveformMixer_, 3);
AudioConnection          patchCord82(Oscillators[3].oscFX_, 0, Oscillators[3].waveformMixer_, 3);
AudioConnection          patchCord183(Oscillators[4].oscFX_, 0, Oscillators[4].waveformMixer_, 3);
AudioConnection          patchCord184(Oscillators[5].oscFX_, 0, Oscillators[5].waveformMixer_, 3);
AudioConnection          patchCord342(Oscillators[6].oscFX_, 0, Oscillators[6].waveformMixer_, 3);
AudioConnection          patchCord343(Oscillators[7].oscFX_, 0, Oscillators[7].waveformMixer_, 3);
AudioConnection          patchCord344(Oscillators[8].oscFX_, 0, Oscillators[8].waveformMixer_, 3);
AudioConnection          patchCord345(Oscillators[9].oscFX_, 0, Oscillators[9].waveformMixer_, 3);
AudioConnection          patchCord346(Oscillators[10].oscFX_, 0, Oscillators[10].waveformMixer_, 3);
AudioConnection          patchCord347(Oscillators[11].oscFX_, 0, Oscillators[11].waveformMixer_, 3);

AudioConnection          patchCord84(Oscillators[0].waveformMixer_, 0, Oscillators[0].filter_, 0);
AudioConnection          patchCord86(Oscillators[0].filterModMixer_, 0, Oscillators[0].filter_, 1);
AudioConnection          patchCord85(Oscillators[1].waveformMixer_, 0, Oscillators[1].filter_, 0);
AudioConnection          patchCord88(Oscillators[1].filterModMixer_, 0, Oscillators[1].filter_, 1);
AudioConnection          patchCord87(Oscillators[2].waveformMixer_, 0, Oscillators[2].filter_, 0);
AudioConnection          patchCord89(Oscillators[2].filterModMixer_, 0, Oscillators[2].filter_, 1);
AudioConnection          patchCord90(Oscillators[3].waveformMixer_, 0, Oscillators[3].filter_, 0);
AudioConnection          patchCord91(Oscillators[3].filterModMixer_, 0, Oscillators[3].filter_, 1);
AudioConnection          patchCord185(Oscillators[4].waveformMixer_, 0, Oscillators[4].filter_, 0);
AudioConnection          patchCord186(Oscillators[4].filterModMixer_, 0, Oscillators[4].filter_, 1);
AudioConnection          patchCord187(Oscillators[5].waveformMixer_, 0, Oscillators[5].filter_, 0);
AudioConnection          patchCord188(Oscillators[5].filterModMixer_, 0, Oscillators[5].filter_, 1);
AudioConnection          patchCord348(Oscillators[6].waveformMixer_, 0, Oscillators[6].filter_, 0);
AudioConnection          patchCord350(Oscillators[6].filterModMixer_, 0, Oscillators[6].filter_, 1);
AudioConnection          patchCord349(Oscillators[7].waveformMixer_, 0, Oscillators[7].filter_, 0);
AudioConnection          patchCord352(Oscillators[7].filterModMixer_, 0, Oscillators[7].filter_, 1);
AudioConnection          patchCord351(Oscillators[8].waveformMixer_, 0, Oscillators[8].filter_, 0);
AudioConnection          patchCord354(Oscillators[8].filterModMixer_, 0, Oscillators[8].filter_, 1);
AudioConnection          patchCord355(Oscillators[9].waveformMixer_, 0, Oscillators[9].filter_, 0);
AudioConnection          patchCord356(Oscillators[9].filterModMixer_, 0, Oscillators[9].filter_, 1);
AudioConnection          patchCord357(Oscillators[10].waveformMixer_, 0, Oscillators[10].filter_, 0);
AudioConnection          patchCord358(Oscillators[10].filterModMixer_, 0, Oscillators[10].filter_, 1);
AudioConnection          patchCord359(Oscillators[11].waveformMixer_, 0, Oscillators[11].filter_, 0);
AudioConnection          patchCord360(Oscillators[11].filterModMixer_, 0, Oscillators[11].filter_, 1);

AudioConnection          patchCord95(Oscillators[0].filter_, 0, Oscillators[0].filterMixer_, 0);
AudioConnection          patchCord96(Oscillators[0].filter_, 1, Oscillators[0].filterMixer_, 1);
AudioConnection          patchCord97(Oscillators[0].filter_, 2, Oscillators[0].filterMixer_, 2);
AudioConnection          patchCord92(Oscillators[1].filter_, 0, Oscillators[1].filterMixer_, 0);
AudioConnection          patchCord93(Oscillators[1].filter_, 1, Oscillators[1].filterMixer_, 1);
AudioConnection          patchCord94(Oscillators[1].filter_, 2, Oscillators[1].filterMixer_, 2);
AudioConnection          patchCord98(Oscillators[2].filter_, 0, Oscillators[2].filterMixer_, 0);
AudioConnection          patchCord99(Oscillators[2].filter_, 1, Oscillators[2].filterMixer_, 1);
AudioConnection          patchCord100(Oscillators[2].filter_, 2, Oscillators[2].filterMixer_, 2);
AudioConnection          patchCord101(Oscillators[3].filter_, 0, Oscillators[3].filterMixer_, 0);
AudioConnection          patchCord102(Oscillators[3].filter_, 1, Oscillators[3].filterMixer_, 1);
AudioConnection          patchCord103(Oscillators[3].filter_, 2, Oscillators[3].filterMixer_, 2);
AudioConnection          patchCord201(Oscillators[4].filter_, 0, Oscillators[4].filterMixer_, 0);
AudioConnection          patchCord189(Oscillators[4].filter_, 1, Oscillators[4].filterMixer_, 1);
AudioConnection          patchCord190(Oscillators[4].filter_, 2, Oscillators[4].filterMixer_, 2);
AudioConnection          patchCord202(Oscillators[5].filter_, 0, Oscillators[5].filterMixer_, 0);
AudioConnection          patchCord191(Oscillators[5].filter_, 1, Oscillators[5].filterMixer_, 1);
AudioConnection          patchCord192(Oscillators[5].filter_, 2, Oscillators[5].filterMixer_, 2);
AudioConnection          patchCord364(Oscillators[6].filter_, 0, Oscillators[6].filterMixer_, 0);
AudioConnection          patchCord365(Oscillators[6].filter_, 1, Oscillators[6].filterMixer_, 1);
AudioConnection          patchCord366(Oscillators[6].filter_, 2, Oscillators[6].filterMixer_, 2);
AudioConnection          patchCord361(Oscillators[7].filter_, 0, Oscillators[7].filterMixer_, 0);
AudioConnection          patchCord362(Oscillators[7].filter_, 1, Oscillators[7].filterMixer_, 1);
AudioConnection          patchCord363(Oscillators[7].filter_, 2, Oscillators[7].filterMixer_, 2);
AudioConnection          patchCord367(Oscillators[8].filter_, 0, Oscillators[8].filterMixer_, 0);
AudioConnection          patchCord368(Oscillators[8].filter_, 1, Oscillators[8].filterMixer_, 1);
AudioConnection          patchCord369(Oscillators[8].filter_, 2, Oscillators[8].filterMixer_, 2);
AudioConnection          patchCord370(Oscillators[9].filter_, 0, Oscillators[9].filterMixer_, 0);
AudioConnection          patchCord371(Oscillators[9].filter_, 1, Oscillators[9].filterMixer_, 1);
AudioConnection          patchCord372(Oscillators[9].filter_, 2, Oscillators[9].filterMixer_, 2);
AudioConnection          patchCord373(Oscillators[10].filter_, 0, Oscillators[10].filterMixer_, 0);
AudioConnection          patchCord374(Oscillators[10].filter_, 1, Oscillators[10].filterMixer_, 1);
AudioConnection          patchCord375(Oscillators[10].filter_, 2, Oscillators[10].filterMixer_, 2);
AudioConnection          patchCord376(Oscillators[11].filter_, 0, Oscillators[11].filterMixer_, 0);
AudioConnection          patchCord377(Oscillators[11].filter_, 1, Oscillators[11].filterMixer_, 1);
AudioConnection          patchCord378(Oscillators[11].filter_, 2, Oscillators[11].filterMixer_, 2);


AudioConnection          patchCord106(Oscillators[0].filterMixer_, Oscillators[0].ampEnvelope_);
AudioConnection          patchCord104(Oscillators[1].filterMixer_, Oscillators[1].ampEnvelope_);
AudioConnection          patchCord105(Oscillators[2].filterMixer_, Oscillators[2].ampEnvelope_);
AudioConnection          patchCord107(Oscillators[3].filterMixer_, Oscillators[3].ampEnvelope_);
AudioConnection          patchCord193(Oscillators[4].filterMixer_, Oscillators[4].ampEnvelope_);
AudioConnection          patchCord194(Oscillators[5].filterMixer_, Oscillators[5].ampEnvelope_);
AudioConnection          patchCord222(Oscillators[6].filterMixer_, Oscillators[6].ampEnvelope_);
AudioConnection          patchCord223(Oscillators[7].filterMixer_, Oscillators[7].ampEnvelope_);
AudioConnection          patchCord224(Oscillators[8].filterMixer_, Oscillators[8].ampEnvelope_);
AudioConnection          patchCord225(Oscillators[9].filterMixer_, Oscillators[9].ampEnvelope_);
AudioConnection          patchCord226(Oscillators[10].filterMixer_, Oscillators[10].ampEnvelope_);
AudioConnection          patchCord227(Oscillators[11].filterMixer_, Oscillators[11].ampEnvelope_);

AudioConnection          patchCord1000(SharedAudio[0].voiceMixer[0], 0, SharedAudio[0].voiceMixerM, 0);
AudioConnection          patchCord1001(SharedAudio[0].voiceMixer[1], 0, SharedAudio[0].voiceMixerM, 1);
AudioConnection          patchCord1002(SharedAudio[0].voiceMixer[2], 0, SharedAudio[0].voiceMixerM, 2);
AudioConnection          patchCord1003(SharedAudio[1].voiceMixer[0], 0, SharedAudio[1].voiceMixerM, 0);
AudioConnection          patchCord1004(SharedAudio[1].voiceMixer[1], 0, SharedAudio[1].voiceMixerM, 1);
AudioConnection          patchCord1005(SharedAudio[1].voiceMixer[2], 0, SharedAudio[1].voiceMixerM, 2);
/*
AudioConnection          patchCord1006(voiceMixer1_3, 0, voiceMixerM_3, 0);
AudioConnection          patchCord1007(voiceMixer2_3, 0, voiceMixerM_3, 1);
AudioConnection          patchCord1008(voiceMixer3_3, 0, voiceMixerM_3, 2);
AudioConnection          patchCord1009(voiceMixer1_4, 0, voiceMixerM_4, 0);
AudioConnection          patchCord1010(voiceMixer2_4, 0, voiceMixerM_4, 1);
AudioConnection          patchCord1011(voiceMixer3_4, 0, voiceMixerM_4, 2);
AudioConnection          patchCord1012(voiceMixer1_5, 0, voiceMixerM_5, 0);
AudioConnection          patchCord1013(voiceMixer2_5, 0, voiceMixerM_5, 1);
AudioConnection          patchCord1014(voiceMixer3_5, 0, voiceMixerM_5, 2);
AudioConnection          patchCord1015(voiceMixer1_6, 0, voiceMixerM_6, 0);
AudioConnection          patchCord1016(voiceMixer2_6, 0, voiceMixerM_6, 1);
AudioConnection          patchCord1017(voiceMixer3_6, 0, voiceMixerM_6, 2);
AudioConnection          patchCord1018(voiceMixer1_7, 0, voiceMixerM_7, 0);
AudioConnection          patchCord1019(voiceMixer2_7, 0, voiceMixerM_7, 1);
AudioConnection          patchCord1020(voiceMixer3_7, 0, voiceMixerM_7, 2);
AudioConnection          patchCord1021(voiceMixer1_8, 0, voiceMixerM_8, 0);
AudioConnection          patchCord1022(voiceMixer2_8, 0, voiceMixerM_8, 1);
AudioConnection          patchCord1023(voiceMixer3_8, 0, voiceMixerM_8, 2);
AudioConnection          patchCord1024(voiceMixer1_9, 0, voiceMixerM_9, 0);
AudioConnection          patchCord1025(voiceMixer2_9, 0, voiceMixerM_9, 1);
AudioConnection          patchCord1026(voiceMixer3_9, 0, voiceMixerM_9, 2);
AudioConnection          patchCord1027(voiceMixer1_10, 0, voiceMixerM_10, 0);
AudioConnection          patchCord1028(voiceMixer2_10, 1, voiceMixerM_10, 2);
AudioConnection          patchCord1029(voiceMixer3_10, 1, voiceMixerM_10, 3);
AudioConnection          patchCord1030(voiceMixer1_11, 0, voiceMixerM_11, 0);
AudioConnection          patchCord1031(voiceMixer2_11, 1, voiceMixerM_11, 2);
AudioConnection          patchCord1032(voiceMixer3_11, 1, voiceMixerM_11, 3);
AudioConnection          patchCord1033(voiceMixer1_12, 0, voiceMixerM_12, 0);
AudioConnection          patchCord1034(voiceMixer2_12, 1, voiceMixerM_12, 2);
AudioConnection          patchCord1035(voiceMixer3_12, 1, voiceMixerM_12, 3);
*/

//AudioConnection          patchCord2001(SharedAudio[0].voiceMixerM, 0, SharedAudio[0].ensemble, 0);
//AudioConnection          patchCord2002(SharedAudio[1].voiceMixerM, 0, SharedAudio[1].ensemble, 0);
/*
AudioConnection          patchCord2003(voiceMixerM_3, 0, ensemble3, 0);
AudioConnection          patchCord2004(voiceMixerM_4, 0, ensemble4, 0);
AudioConnection          patchCord2005(voiceMixerM_5, 0, ensemble5, 0);
AudioConnection          patchCord2006(voiceMixerM_6, 0, ensemble6, 0);
AudioConnection          patchCord2007(voiceMixerM_7, 0, ensemble7, 0);
AudioConnection          patchCord2008(voiceMixerM_8, 0, ensemble8, 0);
AudioConnection          patchCord2009(voiceMixerM_9, 0, ensemble9, 0);
AudioConnection          patchCord2010(voiceMixerM_10, 0, ensemble10, 0);
AudioConnection          patchCord2011(voiceMixerM_11, 0, ensemble11, 0);
AudioConnection          patchCord2012(voiceMixerM_12, 0, ensemble12, 0);
*/

/*
AudioConnection          patchCord111(SharedAudio[0].ensemble, 0, voiceMixer1L, 0);
AudioConnection          patchCord3111(SharedAudio[0].ensemble, 1, voiceMixer1R, 0);
AudioConnection          patchCord108(SharedAudio[1].ensemble, 0, voiceMixer1L, 1);
AudioConnection          patchCord3108(SharedAudio[1].ensemble, 1, voiceMixer1R, 1);
AudioConnection          patchCord109(ensemble3, 0, voiceMixer1L, 2);
AudioConnection          patchCord3109(ensemble3, 0, voiceMixer1R, 2);
AudioConnection          patchCord110(ensemble4, 0, voiceMixer1L, 3);
AudioConnection          patchCord3110(ensemble4, 0, voiceMixer1R, 3);
AudioConnection          patchCord195(ensemble5, 0, voiceMixer2L, 0);
AudioConnection          patchCord3195(ensemble5, 0, voiceMixer2R, 0);
AudioConnection          patchCord196(ensemble6, 0, voiceMixer2L, 1);
AudioConnection          patchCord3196(ensemble6, 0, voiceMixer2R, 1);
AudioConnection          patchCord216(ensemble7, 0, voiceMixer2L, 2);
AudioConnection          patchCord3216(ensemble7, 0, voiceMixer2R, 2);
AudioConnection          patchCord217(ensemble8, 0, voiceMixer2L, 3);
AudioConnection          patchCord3217(ensemble8, 0, voiceMixer2R, 3);
AudioConnection          patchCord218(ensemble9, 0, voiceMixer3L, 0);
AudioConnection          patchCord3218(ensemble9, 0, voiceMixer3R, 0);
AudioConnection          patchCord219(ensemble10, 0, voiceMixer3L, 1);
AudioConnection          patchCord3219(ensemble10, 0, voiceMixer3R, 1);
AudioConnection          patchCord220(ensemble11, 0, voiceMixer3L, 2);
AudioConnection          patchCord3220(ensemble11, 0, voiceMixer3R, 2);
AudioConnection          patchCord221(ensemble12, 0, voiceMixer3L, 3);
AudioConnection          patchCord3221(ensemble12, 0, voiceMixer3R, 3);
*/

//Mod sources
AudioConnection          patchCord121(Oscillators[0].oscModMixer_a, 0, Oscillators[0].waveformMod_a, 0);
AudioConnection          patchCord122(Oscillators[0].oscModMixer_b, 0, Oscillators[0].waveformMod_b, 0);
AudioConnection          patchCord123(Oscillators[1].oscModMixer_a, 0, Oscillators[1].waveformMod_a, 0);
AudioConnection          patchCord124(Oscillators[1].oscModMixer_b, 0, Oscillators[1].waveformMod_b, 0);
AudioConnection          patchCord125(Oscillators[2].oscModMixer_a, 0, Oscillators[2].waveformMod_a, 0);
AudioConnection          patchCord126(Oscillators[2].oscModMixer_b, 0, Oscillators[2].waveformMod_b, 0);
AudioConnection          patchCord127(Oscillators[3].oscModMixer_a, 0, Oscillators[3].waveformMod_a, 0);
AudioConnection          patchCord128(Oscillators[3].oscModMixer_b, 0, Oscillators[3].waveformMod_b, 0);
AudioConnection          patchCord157(Oscillators[4].oscModMixer_a, 0, Oscillators[4].waveformMod_a, 0);
AudioConnection          patchCord158(Oscillators[4].oscModMixer_b, 0, Oscillators[4].waveformMod_b, 0);
AudioConnection          patchCord159(Oscillators[5].oscModMixer_a, 0, Oscillators[5].waveformMod_a, 0);
AudioConnection          patchCord160(Oscillators[5].oscModMixer_b, 0, Oscillators[5].waveformMod_b, 0);
AudioConnection          patchCord379(Oscillators[6].oscModMixer_a, 0, Oscillators[6].waveformMod_a, 0);
AudioConnection          patchCord380(Oscillators[6].oscModMixer_b, 0, Oscillators[6].waveformMod_b, 0);
AudioConnection          patchCord381(Oscillators[7].oscModMixer_a, 0, Oscillators[7].waveformMod_a, 0);
AudioConnection          patchCord382(Oscillators[7].oscModMixer_b, 0, Oscillators[7].waveformMod_b, 0);
AudioConnection          patchCord383(Oscillators[8].oscModMixer_a, 0, Oscillators[8].waveformMod_a, 0);
AudioConnection          patchCord384(Oscillators[8].oscModMixer_b, 0, Oscillators[8].waveformMod_b, 0);
AudioConnection          patchCord385(Oscillators[9].oscModMixer_a, 0, Oscillators[9].waveformMod_a, 0);
AudioConnection          patchCord386(Oscillators[9].oscModMixer_b, 0, Oscillators[9].waveformMod_b, 0);
AudioConnection          patchCord387(Oscillators[10].oscModMixer_a, 0, Oscillators[10].waveformMod_a, 0);
AudioConnection          patchCord388(Oscillators[10].oscModMixer_b, 0, Oscillators[10].waveformMod_b, 0);
AudioConnection          patchCord389(Oscillators[11].oscModMixer_a, 0, Oscillators[11].waveformMod_a, 0);
AudioConnection          patchCord390(Oscillators[11].oscModMixer_b, 0, Oscillators[11].waveformMod_b, 0);

//Pitch env
AudioConnection          patchCord129(Oscillators[0].filterEnvelope_, 0, Oscillators[0].oscModMixer_a, 1);
AudioConnection          patchCord130(Oscillators[0].filterEnvelope_, 0, Oscillators[0].oscModMixer_b, 1);
AudioConnection          patchCord131(Oscillators[1].filterEnvelope_, 0, Oscillators[1].oscModMixer_a, 1);
AudioConnection          patchCord132(Oscillators[1].filterEnvelope_, 0, Oscillators[1].oscModMixer_b, 1);
AudioConnection          patchCord133(Oscillators[2].filterEnvelope_, 0, Oscillators[2].oscModMixer_a, 1);
AudioConnection          patchCord134(Oscillators[2].filterEnvelope_, 0, Oscillators[2].oscModMixer_b, 1);
AudioConnection          patchCord135(Oscillators[3].filterEnvelope_, 0, Oscillators[3].oscModMixer_a, 1);
AudioConnection          patchCord136(Oscillators[3].filterEnvelope_, 0, Oscillators[3].oscModMixer_b, 1);
AudioConnection          patchCord161(Oscillators[4].filterEnvelope_, 0, Oscillators[4].oscModMixer_a, 1);
AudioConnection          patchCord162(Oscillators[4].filterEnvelope_, 0, Oscillators[4].oscModMixer_b, 1);
AudioConnection          patchCord163(Oscillators[5].filterEnvelope_, 0, Oscillators[5].oscModMixer_a, 1);
AudioConnection          patchCord164(Oscillators[5].filterEnvelope_, 0, Oscillators[5].oscModMixer_b, 1);
AudioConnection          patchCord391(Oscillators[6].filterEnvelope_, 0, Oscillators[6].oscModMixer_a, 1);
AudioConnection          patchCord392(Oscillators[6].filterEnvelope_, 0, Oscillators[6].oscModMixer_b, 1);
AudioConnection          patchCord393(Oscillators[7].filterEnvelope_, 0, Oscillators[7].oscModMixer_a, 1);
AudioConnection          patchCord394(Oscillators[7].filterEnvelope_, 0, Oscillators[7].oscModMixer_b, 1);
AudioConnection          patchCord395(Oscillators[8].filterEnvelope_, 0, Oscillators[8].oscModMixer_a, 1);
AudioConnection          patchCord396(Oscillators[8].filterEnvelope_, 0, Oscillators[8].oscModMixer_b, 1);
AudioConnection          patchCord397(Oscillators[9].filterEnvelope_, 0, Oscillators[9].oscModMixer_a, 1);
AudioConnection          patchCord398(Oscillators[9].filterEnvelope_, 0, Oscillators[9].oscModMixer_b, 1);
AudioConnection          patchCord399(Oscillators[10].filterEnvelope_, 0, Oscillators[10].oscModMixer_a, 1);
AudioConnection          patchCord400(Oscillators[10].filterEnvelope_, 0, Oscillators[10].oscModMixer_b, 1);
AudioConnection          patchCord401(Oscillators[11].filterEnvelope_, 0, Oscillators[11].oscModMixer_a, 1);
AudioConnection          patchCord402(Oscillators[11].filterEnvelope_, 0, Oscillators[11].oscModMixer_b, 1);

//Glide
AudioConnection          patchCord22(Oscillators[0].glide_, 0, Oscillators[0].oscModMixer_a, 2);
AudioConnection          patchCord204(Oscillators[0].glide_, 0, Oscillators[0].oscModMixer_b, 2);
AudioConnection          patchCord205(Oscillators[1].glide_, 0, Oscillators[1].oscModMixer_a, 2);
AudioConnection          patchCord206(Oscillators[1].glide_, 0, Oscillators[1].oscModMixer_b, 2);
AudioConnection          patchCord207(Oscillators[2].glide_, 0, Oscillators[2].oscModMixer_a, 2);
AudioConnection          patchCord208(Oscillators[2].glide_, 0, Oscillators[2].oscModMixer_b, 2);
AudioConnection          patchCord209(Oscillators[3].glide_, 0, Oscillators[3].oscModMixer_a, 2);
AudioConnection          patchCord210(Oscillators[3].glide_, 0, Oscillators[3].oscModMixer_b, 2);
AudioConnection          patchCord211(Oscillators[4].glide_, 0, Oscillators[4].oscModMixer_a, 2);
AudioConnection          patchCord212(Oscillators[4].glide_, 0, Oscillators[4].oscModMixer_b, 2);
AudioConnection          patchCord213(Oscillators[5].glide_, 0, Oscillators[5].oscModMixer_a, 2);
AudioConnection          patchCord214(Oscillators[5].glide_, 0, Oscillators[5].oscModMixer_b, 2);
AudioConnection          patchCord403(Oscillators[6].glide_, 0, Oscillators[6].oscModMixer_a, 2);
AudioConnection          patchCord404(Oscillators[6].glide_, 0, Oscillators[6].oscModMixer_b, 2);
AudioConnection          patchCord405(Oscillators[7].glide_, 0, Oscillators[7].oscModMixer_a, 2);
AudioConnection          patchCord406(Oscillators[7].glide_, 0, Oscillators[7].oscModMixer_b, 2);
AudioConnection          patchCord407(Oscillators[8].glide_, 0, Oscillators[8].oscModMixer_a, 2);
AudioConnection          patchCord408(Oscillators[8].glide_, 0, Oscillators[8].oscModMixer_b, 2);
AudioConnection          patchCord409(Oscillators[9].glide_, 0, Oscillators[9].oscModMixer_a, 2);
AudioConnection          patchCord410(Oscillators[9].glide_, 0, Oscillators[9].oscModMixer_b, 2);
AudioConnection          patchCord411(Oscillators[10].glide_, 0, Oscillators[10].oscModMixer_a, 2);
AudioConnection          patchCord412(Oscillators[10].glide_, 0, Oscillators[10].oscModMixer_b, 2);
AudioConnection          patchCord413(Oscillators[11].glide_, 0, Oscillators[11].oscModMixer_a, 2);
AudioConnection          patchCord414(Oscillators[11].glide_, 0, Oscillators[11].oscModMixer_b, 2);

//X Mod
AudioConnection          patchCord417(Oscillators[0].waveformMod_b, 0, Oscillators[0].oscModMixer_a, 3);
AudioConnection          patchCord429(Oscillators[0].waveformMod_a, 0, Oscillators[0].oscModMixer_b, 3);
AudioConnection          patchCord418(Oscillators[1].waveformMod_b, 0, Oscillators[1].oscModMixer_a, 3);
AudioConnection          patchCord430(Oscillators[1].waveformMod_a, 0, Oscillators[1].oscModMixer_b, 3);
AudioConnection          patchCord419(Oscillators[2].waveformMod_b, 0, Oscillators[2].oscModMixer_a, 3);
AudioConnection          patchCord431(Oscillators[2].waveformMod_a, 0, Oscillators[2].oscModMixer_b, 3);
AudioConnection          patchCord420(Oscillators[3].waveformMod_b, 0, Oscillators[3].oscModMixer_a, 3);
AudioConnection          patchCord432(Oscillators[3].waveformMod_a, 0, Oscillators[3].oscModMixer_b, 3);
AudioConnection          patchCord421(Oscillators[4].waveformMod_b, 0, Oscillators[4].oscModMixer_a, 3);
AudioConnection          patchCord433(Oscillators[4].waveformMod_a, 0, Oscillators[4].oscModMixer_b, 3);
AudioConnection          patchCord422(Oscillators[5].waveformMod_b, 0, Oscillators[5].oscModMixer_a, 3);
AudioConnection          patchCord434(Oscillators[5].waveformMod_a, 0, Oscillators[5].oscModMixer_b, 3);
AudioConnection          patchCord423(Oscillators[6].waveformMod_b, 0, Oscillators[6].oscModMixer_a, 3);
AudioConnection          patchCord435(Oscillators[6].waveformMod_a, 0, Oscillators[6].oscModMixer_b, 3);
AudioConnection          patchCord424(Oscillators[7].waveformMod_b, 0, Oscillators[7].oscModMixer_a, 3);
AudioConnection          patchCord436(Oscillators[7].waveformMod_a, 0, Oscillators[7].oscModMixer_b, 3);
AudioConnection          patchCord425(Oscillators[8].waveformMod_b, 0, Oscillators[8].oscModMixer_a, 3);
AudioConnection          patchCord437(Oscillators[8].waveformMod_a, 0, Oscillators[8].oscModMixer_b, 3);
AudioConnection          patchCord426(Oscillators[9].waveformMod_b, 0, Oscillators[9].oscModMixer_a, 3);
AudioConnection          patchCord438(Oscillators[9].waveformMod_a, 0, Oscillators[9].oscModMixer_b, 3);
AudioConnection          patchCord427(Oscillators[10].waveformMod_b, 0, Oscillators[10].oscModMixer_a, 3);
AudioConnection          patchCord439(Oscillators[10].waveformMod_a, 0, Oscillators[10].oscModMixer_b, 3);
AudioConnection          patchCord428(Oscillators[11].waveformMod_b, 0, Oscillators[11].oscModMixer_a, 3);
AudioConnection          patchCord440(Oscillators[11].waveformMod_a, 0, Oscillators[11].oscModMixer_b, 3);

//Noise
AudioConnection          patchCord452(pink, 0, SharedAudio[0].noiseMixer, 0);
AudioConnection          patchCord453(pink, 0, SharedAudio[1].noiseMixer, 0);
AudioConnection          patchCord454(pink, 0, SharedAudio[2].noiseMixer, 0);
AudioConnection          patchCord455(pink, 0, SharedAudio[3].noiseMixer, 0);
AudioConnection          patchCord456(pink, 0, SharedAudio[4].noiseMixer, 0);
AudioConnection          patchCord457(pink, 0, SharedAudio[5].noiseMixer, 0);
AudioConnection          patchCord458(pink, 0, SharedAudio[6].noiseMixer, 0);
AudioConnection          patchCord459(pink, 0, SharedAudio[7].noiseMixer, 0);
AudioConnection          patchCord460(pink, 0, SharedAudio[8].noiseMixer, 0);
AudioConnection          patchCord461(pink, 0, SharedAudio[9].noiseMixer, 0);
AudioConnection          patchCord462(pink, 0, SharedAudio[10].noiseMixer, 0);
AudioConnection          patchCord463(pink, 0, SharedAudio[11].noiseMixer, 0);

AudioConnection          patchCord137(white, 0, SharedAudio[0].noiseMixer, 1);
AudioConnection          patchCord138(white, 0, SharedAudio[1].noiseMixer, 1);
AudioConnection          patchCord442(white, 0, SharedAudio[2].noiseMixer, 1);
AudioConnection          patchCord443(white, 0, SharedAudio[3].noiseMixer, 1);
AudioConnection          patchCord444(white, 0, SharedAudio[4].noiseMixer, 1);
AudioConnection          patchCord445(white, 0, SharedAudio[5].noiseMixer, 1);
AudioConnection          patchCord446(white, 0, SharedAudio[6].noiseMixer, 1);
AudioConnection          patchCord447(white, 0, SharedAudio[7].noiseMixer, 1);
AudioConnection          patchCord448(white, 0, SharedAudio[8].noiseMixer, 1);
AudioConnection          patchCord449(white, 0, SharedAudio[9].noiseMixer, 1);
AudioConnection          patchCord450(white, 0, SharedAudio[10].noiseMixer, 1);
AudioConnection          patchCord451(white, 0, SharedAudio[11].noiseMixer, 1);

// Final mixing, funnel the groups into the global mixer.
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
