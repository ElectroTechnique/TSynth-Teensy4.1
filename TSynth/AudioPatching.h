#ifndef TSYNTH_AUDIO_PATCHING_H
#define TSYNTH_AUDIO_PATCHING_H

//waveformX      -->   waveformMixerX   -->   voiceMixer1-3   -->   voiceMixerM  --> volumeMixer
//WAVEFORMLEVEL        oscA/BLevel             VELOCITY    VOICEMIXERLEVEL/UNISONVOICEMIXERLEVEL    volume

// Make the visual audio design tool work by substituting the *TS locally changed classes by those it knows: 
// but also ensure all redefined class names are undefined within this scope, if lingering defines exist, raise an error:
#if defined(AudioSynthWaveform) || defined(AudioSynthWaveformModulated)  || defined(AudioEffectEnvelope) || defined(AudioSynthWaveformDc)
# error "Either AudioSynthWaveform or AudioSynthWaveformModulated or AudioEffectEnvelope or AudioSynthWaveformDc are already redefined!"
#endif
#define AudioSynthWaveform          AudioSynthWaveformTS
#define AudioSynthWaveformModulated AudioSynthWaveformModulatedTS
#define AudioEffectEnvelope AudioEffectEnvelopeTS												 
#define AudioSynthWaveformDc AudioSynthWaveformDcTS			

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

AudioSynthWaveformDc     pitchBend1; //xy=472,342
AudioSynthWaveformDc     pitchBend2; //xy=441.5,661
AudioSynthWaveformDc     pitchBend3; //xy=439.5,946
AudioSynthWaveformDc     pitchBend4; //xy=442.5,1254
AudioSynthWaveformDc     pitchBend5; //xy=440.5,1559
AudioSynthWaveformDc     pitchBend6; //xy=441.5,1855
AudioSynthWaveformDc     pitchBend7; //xy=442.5,2163
AudioSynthWaveformDc     pitchBend8; //xy=439.5,2457
AudioSynthWaveformDc     pitchBend9; //xy=437.5,2744
AudioSynthWaveformDc     pitchBend10; //xy=440,3051
AudioSynthWaveformDc     pitchBend11; //xy=436.5,3361
AudioSynthWaveformDc     pitchBend12; //xy=441.5,3656

AudioSynthWaveform       pitchLfo1; //xy=466.5,382
AudioSynthWaveform       pitchLfo2; //xy=436,701
AudioSynthWaveform       pitchLfo3; //xy=434,986
AudioSynthWaveform       pitchLfo4; //xy=437,1294
AudioSynthWaveform       pitchLfo5; //xy=435,1599
AudioSynthWaveform       pitchLfo6; //xy=436,1895
AudioSynthWaveform       pitchLfo7; //xy=437,2203
AudioSynthWaveform       pitchLfo8; //xy=434,2497
AudioSynthWaveform       pitchLfo9; //xy=432,2784
AudioSynthWaveform       pitchLfo10; //xy=434.5,3091
AudioSynthWaveform       pitchLfo11; //xy=431,3401
AudioSynthWaveform       pitchLfo12; //xy=436,3696

AudioMixer4              oscPitchMixer1; //xy=647,373
AudioMixer4              oscPitchMixer2; //xy=616.5,692
AudioMixer4              oscPitchMixer3; //xy=614.5,977
AudioMixer4              oscPitchMixer4; //xy=617.5,1285
AudioMixer4              oscPitchMixer5; //xy=615.5,1590
AudioMixer4              oscPitchMixer6; //xy=616.5,1886
AudioMixer4              oscPitchMixer7; //xy=617.5,2194
AudioMixer4              oscPitchMixer8; //xy=614.5,2488
AudioMixer4              oscPitchMixer9; //xy=612.5,2775
AudioMixer4              oscPitchMixer10; //xy=615,3082
AudioMixer4              oscPitchMixer11; //xy=611.5,3392
AudioMixer4              oscPitchMixer12; //xy=616.5,3687

AudioSynthWaveformDc     pwa1;  //xy=783,715
AudioSynthWaveformDc     pwa2;  //xy=783,1028
AudioSynthWaveformDc     pwa3;  //xy=783,1319
AudioSynthWaveformDc     pwa4;  //xy=783,1617
AudioSynthWaveformDc     pwa5;  //xy=783,1911
AudioSynthWaveformDc     pwa6;  //xy=783,2205
AudioSynthWaveformDc     pwa7;  //xy=783,2508
AudioSynthWaveformDc     pwa8;  //xy=783,2806
AudioSynthWaveformDc     pwa9;  //xy=783,3118
AudioSynthWaveformDc     pwa10; //xy=783,3418
AudioSynthWaveformDc     pwa11; //xy=783,3716
AudioSynthWaveformDc     pwa12; //xy=783,4013

AudioSynthWaveformDc     pwb1;  //xy=783,786
AudioSynthWaveformDc     pwb2;  //xy=783,1094
AudioSynthWaveformDc     pwb3;  //xy=783,1394
AudioSynthWaveformDc     pwb4;  //xy=783,1695
AudioSynthWaveformDc     pwb5;  //xy=783,1986
AudioSynthWaveformDc     pwb6;  //xy=783,2291
AudioSynthWaveformDc     pwb7;  //xy=783,2575
AudioSynthWaveformDc     pwb8;  //xy=783,2875
AudioSynthWaveformDc     pwb9;  //xy=783,3192
AudioSynthWaveformDc     pwb10; //xy=783,3492
AudioSynthWaveformDc     pwb11; //xy=783,3790
AudioSynthWaveformDc     pwb12; //xy=783,4089

AudioSynthWaveform       pwmLfoA1;  //xy=773,678
AudioSynthWaveform       pwmLfoA2;  //xy=782,987
AudioSynthWaveform       pwmLfoA3;  //xy=777,1283
AudioSynthWaveform       pwmLfoA4;  //xy=786,1582
AudioSynthWaveform       pwmLfoA5;  //xy=776,1876
AudioSynthWaveform       pwmLfoA6;  //xy=768,2170
AudioSynthWaveform       pwmLfoA7;  //xy=773,2470
AudioSynthWaveform       pwmLfoA8;  //xy=778,2770
AudioSynthWaveform       pwmLfoA9;  //xy=787,3081
AudioSynthWaveform       pwmLfoA10; //xy=781,3381
AudioSynthWaveform       pwmLfoA11; //xy=785,3682
AudioSynthWaveform       pwmLfoA12; //xy=781,3976

AudioSynthWaveform       pwmLfoB1;  //xy=776,751
AudioSynthWaveform       pwmLfoB2;  //xy=777,1058
AudioSynthWaveform       pwmLfoB3;  //xy=773,1360
AudioSynthWaveform       pwmLfoB4;  //xy=783,1655
AudioSynthWaveform       pwmLfoB5;  //xy=782,1950
AudioSynthWaveform       pwmLfoB6;  //xy=784,2257
AudioSynthWaveform       pwmLfoB7;  //xy=780,2542
AudioSynthWaveform       pwmLfoB8;  //xy=784,2841
AudioSynthWaveform       pwmLfoB9;  //xy=786,3152
AudioSynthWaveform       pwmLfoB10; //xy=781,3452
AudioSynthWaveform       pwmLfoB11; //xy=782,3753
AudioSynthWaveform       pwmLfoB12; //xy=785,4051

AudioMixer4              noiseMixer1;  //xy=1207,621
AudioMixer4              noiseMixer2;  //xy=1205,930
AudioMixer4              noiseMixer3;  //xy=1189,1239
AudioMixer4              noiseMixer4;  //xy=1198,1521
AudioMixer4              noiseMixer5;  //xy=1198,1824
AudioMixer4              noiseMixer6;  //xy=1195,2116
AudioMixer4              noiseMixer7;  //xy=1191,2425
AudioMixer4              noiseMixer8;  //xy=1198,2724
AudioMixer4              noiseMixer9;  //xy=1199,3021
AudioMixer4              noiseMixer10; //xy=1184,3326
AudioMixer4              noiseMixer11; //xy=1190,3619
AudioMixer4              noiseMixer12; //xy=1178,3928

AudioSynthWaveform       filterLfo1;  //xy=1335,693
AudioSynthWaveform       filterLfo2;  //xy=1342,997
AudioSynthWaveform       filterLfo3;  //xy=1343,1288
AudioSynthWaveform       filterLfo4;  //xy=1331,1587
AudioSynthWaveform       filterLfo5;  //xy=1330,1892
AudioSynthWaveform       filterLfo6;  //xy=1336,2188
AudioSynthWaveform       filterLfo7;  //xy=1336,2482
AudioSynthWaveform       filterLfo8;  //xy=1333,2785
AudioSynthWaveform       filterLfo9;  //xy=1333,3084
AudioSynthWaveform       filterLfo10; //xy=1327,3388
AudioSynthWaveform       filterLfo11; //xy=1335,3686
AudioSynthWaveform       filterLfo12; //xy=1341,3988

AudioEffectEnvelope      filterEnvelope1; //xy=611,145
AudioEffectEnvelope      filterEnvelope2; //xy=611,445
AudioEffectEnvelope      filterEnvelope3; //xy=611,745
AudioEffectEnvelope      filterEnvelope4; //xy=611,1045
AudioEffectEnvelope      filterEnvelope5; //xy=611,1345
AudioEffectEnvelope      filterEnvelope6; //xy=611,1645
AudioEffectEnvelope      filterEnvelope7; //xy=611,1945
AudioEffectEnvelope      filterEnvelope8; //xy=611,2245
AudioEffectEnvelope      filterEnvelope9; //xy=611,2545
AudioEffectEnvelope      filterEnvelope10; //xy=611,2845
AudioEffectEnvelope      filterEnvelope11; //xy=611,3145
AudioEffectEnvelope      filterEnvelope12; //xy=611,3445

AudioMixer4              pwMixer1a;      //xy=941,185
AudioMixer4              pwMixer1b;      //xy=941,255
AudioMixer4              pwMixer2a;      //xy=941,485
AudioMixer4              pwMixer2b;      //xy=941,555
AudioMixer4              pwMixer3a;      //xy=941,785
AudioMixer4              pwMixer3b;      //xy=941,855
AudioMixer4              pwMixer4a;      //xy=941,1085
AudioMixer4              pwMixer4b;      //xy=941,1155
AudioMixer4              pwMixer5a;      //xy=941,1385
AudioMixer4              pwMixer5b;      //xy=941,1455
AudioMixer4              pwMixer6a;      //xy=941,1685
AudioMixer4              pwMixer6b;      //xy=941,1755
AudioMixer4              pwMixer7a;      //xy=941,1985
AudioMixer4              pwMixer7b;      //xy=941,2055
AudioMixer4              pwMixer8a;      //xy=941,2285
AudioMixer4              pwMixer8b;      //xy=941,2355
AudioMixer4              pwMixer9a;      //xy=941,2585
AudioMixer4              pwMixer9b;      //xy=941,2655
AudioMixer4              pwMixer10a;     //xy=941,2885
AudioMixer4              pwMixer10b;     //xy=941,2955
AudioMixer4              pwMixer11a;     //xy=941,3185
AudioMixer4              pwMixer11b;     //xy=941,3255
AudioMixer4              pwMixer12a;     //xy=941,3485
AudioMixer4              pwMixer12b;     //xy=941,3555

AudioSynthWaveformDc     glide1;         //xy=619,430
AudioSynthWaveformDc     glide2;         //xy=588,741
AudioSynthWaveformDc     glide3;         //xy=586,1036
AudioSynthWaveformDc     glide4;         //xy=591,1340
AudioSynthWaveformDc     glide5;         //xy=588,1640
AudioSynthWaveformDc     glide6;         //xy=589,1939
AudioSynthWaveformDc     glide7;         //xy=588,2243
AudioSynthWaveformDc     glide8;         //xy=584,2541
AudioSynthWaveformDc     glide9;         //xy=587,2834
AudioSynthWaveformDc     glide10;        //xy=582,3138
AudioSynthWaveformDc     glide11;        //xy=582,3442
AudioSynthWaveformDc     glide12;        //xy=584,3739

AudioSynthWaveformDc     keytracking1;   //xy=1326,172
AudioSynthWaveformDc     keytracking2;   //xy=1326,472
AudioSynthWaveformDc     keytracking3;   //xy=1326,772
AudioSynthWaveformDc     keytracking4;   //xy=1326,1072
AudioSynthWaveformDc     keytracking5;   //xy=1326,1372
AudioSynthWaveformDc     keytracking6;   //xy=1326,1672
AudioSynthWaveformDc     keytracking7;   //xy=1326,1972
AudioSynthWaveformDc     keytracking8;   //xy=1326,2272
AudioSynthWaveformDc     keytracking9;   //xy=1326,2572
AudioSynthWaveformDc     keytracking10;  //xy=1326,2872
AudioSynthWaveformDc     keytracking11;  //xy=1326,3172
AudioSynthWaveformDc     keytracking12;  //xy=1326,3472

AudioMixer4              oscModMixer1a;  //xy=956,35
AudioMixer4              oscModMixer1b;  //xy=956,105
AudioMixer4              oscModMixer2a;  //xy=956,335
AudioMixer4              oscModMixer2b;  //xy=956,405
AudioMixer4              oscModMixer3a;  //xy=956,635
AudioMixer4              oscModMixer3b;  //xy=956,705
AudioMixer4              oscModMixer4a;  //xy=956,935
AudioMixer4              oscModMixer4b;  //xy=956,1005
AudioMixer4              oscModMixer5a;  //xy=956,1235
AudioMixer4              oscModMixer5b;  //xy=956,1305
AudioMixer4              oscModMixer6a;  //xy=956,1535
AudioMixer4              oscModMixer6b;  //xy=956,1605
AudioMixer4              oscModMixer7a;  //xy=956,1835
AudioMixer4              oscModMixer7b;  //xy=956,1905
AudioMixer4              oscModMixer8a;  //xy=956,2135
AudioMixer4              oscModMixer8b;  //xy=956,2205
AudioMixer4              oscModMixer9a;  //xy=956,2435
AudioMixer4              oscModMixer9b;  //xy=956,2505
AudioMixer4              oscModMixer10a; //xy=956,2735
AudioMixer4              oscModMixer10b; //xy=956,2805
AudioMixer4              oscModMixer11a; //xy=956,3035
AudioMixer4              oscModMixer11b; //xy=956,3105
AudioMixer4              oscModMixer12a; //xy=956,3335
AudioMixer4              oscModMixer12b; //xy=956,3405

AudioSynthWaveformModulated waveformMod1a;  //xy=1171,35
AudioSynthWaveformModulated waveformMod1b;  //xy=1171,255
AudioSynthWaveformModulated waveformMod2a;  //xy=1171,335
AudioSynthWaveformModulated waveformMod2b;  //xy=1171,555
AudioSynthWaveformModulated waveformMod3a;  //xy=1171,635
AudioSynthWaveformModulated waveformMod3b;  //xy=1171,855
AudioSynthWaveformModulated waveformMod4a;  //xy=1171,935
AudioSynthWaveformModulated waveformMod4b;  //xy=1171,1155
AudioSynthWaveformModulated waveformMod5a;  //xy=1171,1235
AudioSynthWaveformModulated waveformMod5b;  //xy=1171,1455
AudioSynthWaveformModulated waveformMod6a;  //xy=1171,1535
AudioSynthWaveformModulated waveformMod6b;  //xy=1171,1755
AudioSynthWaveformModulated waveformMod7a;  //xy=1171,1835
AudioSynthWaveformModulated waveformMod7b;  //xy=1171,2055
AudioSynthWaveformModulated waveformMod8a;  //xy=1171,2135
AudioSynthWaveformModulated waveformMod8b;  //xy=1171,2355
AudioSynthWaveformModulated waveformMod9a;  //xy=1171,2435
AudioSynthWaveformModulated waveformMod9b;  //xy=1171,2655
AudioSynthWaveformModulated waveformMod10a; //xy=1171,2735
AudioSynthWaveformModulated waveformMod10b; //xy=1171,2955
AudioSynthWaveformModulated waveformMod11a; //xy=1171,3035
AudioSynthWaveformModulated waveformMod11b; //xy=1171,3255
AudioSynthWaveformModulated waveformMod12a; //xy=1171,3335
AudioSynthWaveformModulated waveformMod12b; //xy=1171,3555

AudioEffectDigitalCombine oscFX1;         //xy=1336,115
AudioEffectDigitalCombine oscFX2;         //xy=1336,415
AudioEffectDigitalCombine oscFX3;         //xy=1336,715
AudioEffectDigitalCombine oscFX4;         //xy=1336,1015
AudioEffectDigitalCombine oscFX5;         //xy=1336,1315
AudioEffectDigitalCombine oscFX6;         //xy=1336,1615
AudioEffectDigitalCombine oscFX7;         //xy=1336,1915
AudioEffectDigitalCombine oscFX8;         //xy=1336,2215
AudioEffectDigitalCombine oscFX9;         //xy=1336,2515
AudioEffectDigitalCombine oscFX10;        //xy=1336,2815
AudioEffectDigitalCombine oscFX11;        //xy=1336,3115
AudioEffectDigitalCombine oscFX12;        //xy=1336,3415

AudioMixer4              waveformMixer1; //xy=1516,95
AudioMixer4              waveformMixer2; //xy=1516,395
AudioMixer4              waveformMixer3; //xy=1516,695
AudioMixer4              waveformMixer4; //xy=1516,995
AudioMixer4              waveformMixer5; //xy=1516,1295
AudioMixer4              waveformMixer6; //xy=1516,1595
AudioMixer4              waveformMixer7; //xy=1516,1895
AudioMixer4              waveformMixer8; //xy=1516,2195
AudioMixer4              waveformMixer9; //xy=1516,2495
AudioMixer4              waveformMixer10; //xy=1516,2795
AudioMixer4              waveformMixer11; //xy=1516,3095
AudioMixer4              waveformMixer12; //xy=1516,3395

AudioMixer4              filterModMixer1; //xy=1516,165
AudioMixer4              filterModMixer2; //xy=1516,465
AudioMixer4              filterModMixer3; //xy=1516,765
AudioMixer4              filterModMixer4; //xy=1516,1065
AudioMixer4              filterModMixer5; //xy=1516,1365
AudioMixer4              filterModMixer6; //xy=1516,1665
AudioMixer4              filterModMixer7; //xy=1516,1965
AudioMixer4              filterModMixer8; //xy=1516,2265
AudioMixer4              filterModMixer9; //xy=1516,2565
AudioMixer4              filterModMixer10; //xy=1516,2865
AudioMixer4              filterModMixer11; //xy=1516,3165
AudioMixer4              filterModMixer12; //xy=1516,3465

AudioFilterStateVariable filter1;        //xy=1681,145
AudioFilterStateVariable filter2;        //xy=1681,445
AudioFilterStateVariable filter3;        //xy=1681,745
AudioFilterStateVariable filter4;        //xy=1681,1045
AudioFilterStateVariable filter5;        //xy=1681,1345
AudioFilterStateVariable filter6;        //xy=1681,1645
AudioFilterStateVariable filter7;        //xy=1681,1945
AudioFilterStateVariable filter8;        //xy=1681,2245
AudioFilterStateVariable filter9;        //xy=1681,2545
AudioFilterStateVariable filter10;       //xy=1681,2845
AudioFilterStateVariable filter11;       //xy=1681,3145
AudioFilterStateVariable filter12;       //xy=1681,3445

AudioMixer4              filterMixer1;   //xy=1821,145
AudioMixer4              filterMixer2;   //xy=1821,445
AudioMixer4              filterMixer3;   //xy=1821,745
AudioMixer4              filterMixer4;   //xy=1821,1045
AudioMixer4              filterMixer5;   //xy=1821,1345
AudioMixer4              filterMixer6;   //xy=1821,1645
AudioMixer4              filterMixer7;   //xy=1821,1945
AudioMixer4              filterMixer8;   //xy=1821,2245
AudioMixer4              filterMixer9;   //xy=1821,2545
AudioMixer4              filterMixer10;  //xy=1821,2845
AudioMixer4              filterMixer11;  //xy=1821,3145
AudioMixer4              filterMixer12;  //xy=1821,3445

AudioEffectEnvelope      ampEnvelope1;   //xy=1996,145
AudioEffectEnvelope      ampEnvelope2;   //xy=1996,445
AudioEffectEnvelope      ampEnvelope3;   //xy=1996,745
AudioEffectEnvelope      ampEnvelope4;   //xy=1996,1045
AudioEffectEnvelope      ampEnvelope5;   //xy=1996,1345
AudioEffectEnvelope      ampEnvelope6;   //xy=1996,1645
AudioEffectEnvelope      ampEnvelope7;   //xy=1996,1945
AudioEffectEnvelope      ampEnvelope8;   //xy=1996,2245
AudioEffectEnvelope      ampEnvelope9;   //xy=1996,2545
AudioEffectEnvelope      ampEnvelope10;  //xy=1996,2845
AudioEffectEnvelope      ampEnvelope11;  //xy=1996,3145
AudioEffectEnvelope      ampEnvelope12;  //xy=1996,3445

AudioConnection          patchCord1(constant1Dc, filterEnvelope2);
AudioConnection          patchCord2(constant1Dc, filterEnvelope3);
AudioConnection          patchCord3(constant1Dc, filterEnvelope4);
AudioConnection          patchCord4(constant1Dc, filterEnvelope1);
AudioConnection          patchCord199(constant1Dc, filterEnvelope5);
AudioConnection          patchCord200(constant1Dc, filterEnvelope6);
AudioConnection          patchCord228(constant1Dc, filterEnvelope7);
AudioConnection          patchCord229(constant1Dc, filterEnvelope8);
AudioConnection          patchCord230(constant1Dc, filterEnvelope9);
AudioConnection          patchCord231(constant1Dc, filterEnvelope10);
AudioConnection          patchCord232(constant1Dc, filterEnvelope11);
AudioConnection          patchCord233(constant1Dc, filterEnvelope12);

AudioConnection          patchCord21(pitchLfo1, 0, oscPitchMixer1, 1);
AudioConnection          patchCord23(pitchLfo2, 0, oscPitchMixer2, 1);
AudioConnection          patchCord485(pitchLfo3, 0, oscPitchMixer3, 1);
AudioConnection          patchCord486(pitchLfo4, 0, oscPitchMixer4, 1);
AudioConnection          patchCord464(pitchLfo5, 0, oscPitchMixer5, 1);
AudioConnection          patchCord465(pitchLfo6, 0, oscPitchMixer6, 1);
AudioConnection          patchCord466(pitchLfo7, 0, oscPitchMixer7, 1);
AudioConnection          patchCord467(pitchLfo8, 0, oscPitchMixer8, 1);
AudioConnection          patchCord469(pitchLfo9, 0, oscPitchMixer9, 1);
AudioConnection          patchCord470(pitchLfo10, 0, oscPitchMixer10, 1);
AudioConnection          patchCord471(pitchLfo11, 0, oscPitchMixer11, 1);
AudioConnection          patchCord472(pitchLfo12, 0, oscPitchMixer12, 1);

AudioConnection          patchCord473(pitchBend1, 0, oscPitchMixer1, 0);
AudioConnection          patchCord474(pitchBend2, 0, oscPitchMixer2, 0);
AudioConnection          patchCord475(pitchBend3, 0, oscPitchMixer3, 0);
AudioConnection          patchCord476(pitchBend4, 0, oscPitchMixer4, 0);
AudioConnection          patchCord477(pitchBend5, 0, oscPitchMixer5, 0);
AudioConnection          patchCord478(pitchBend6, 0, oscPitchMixer6, 0);
AudioConnection          patchCord479(pitchBend7, 0, oscPitchMixer7, 0);
AudioConnection          patchCord480(pitchBend8, 0, oscPitchMixer8, 0);
AudioConnection          patchCord481(pitchBend9, 0, oscPitchMixer9, 0);
AudioConnection          patchCord482(pitchBend10, 0, oscPitchMixer10, 0);
AudioConnection          patchCord483(pitchBend11, 0, oscPitchMixer11, 0);
AudioConnection          patchCord484(pitchBend12, 0, oscPitchMixer12, 0);

AudioConnection          patchCord28(keytracking1, 0, filterModMixer1, 2);
AudioConnection          patchCord29(keytracking2, 0, filterModMixer2, 2);
AudioConnection          patchCord30(keytracking3, 0, filterModMixer3, 2);
AudioConnection          patchCord31(keytracking4, 0, filterModMixer4, 2);
AudioConnection          patchCord149(keytracking5, 0, filterModMixer5, 2);
AudioConnection          patchCord150(keytracking6, 0, filterModMixer6, 2);
AudioConnection          patchCord264(keytracking7, 0, filterModMixer7, 2);
AudioConnection          patchCord265(keytracking8, 0, filterModMixer8, 2);
AudioConnection          patchCord266(keytracking9, 0, filterModMixer9, 2);
AudioConnection          patchCord267(keytracking10, 0, filterModMixer10, 2);
AudioConnection          patchCord268(keytracking11, 0, filterModMixer11, 2);
AudioConnection          patchCord269(keytracking12, 0, filterModMixer12, 2);																 

AudioConnection          patchCord36(pwMixer1a, 0, waveformMod1a, 1);
AudioConnection          patchCord37(pwMixer1b, 0, waveformMod1b, 1);
AudioConnection          patchCord38(pwMixer3b, 0, waveformMod3b, 1);
AudioConnection          patchCord47(pwMixer4b, 0, waveformMod4b, 1);
AudioConnection          patchCord48(pwMixer4a, 0, waveformMod4a, 1);
AudioConnection          patchCord49(pwMixer2a, 0, waveformMod2a, 1);
AudioConnection          patchCord50(pwMixer2b, 0, waveformMod2b, 1);
AudioConnection          patchCord51(pwMixer3a, 0, waveformMod3a, 1);
AudioConnection          patchCord165(pwMixer5a, 0, waveformMod5a, 1);
AudioConnection          patchCord166(pwMixer5b, 0, waveformMod5b, 1);
AudioConnection          patchCord167(pwMixer6a, 0, waveformMod6a, 1);
AudioConnection          patchCord168(pwMixer6b, 0, waveformMod6b, 1);
AudioConnection          patchCord276(pwMixer7a, 0, waveformMod7a, 1);
AudioConnection          patchCord277(pwMixer7b, 0, waveformMod7b, 1);
AudioConnection          patchCord278(pwMixer8a, 0, waveformMod8a, 1);
AudioConnection          patchCord279(pwMixer8b, 0, waveformMod8b, 1);
AudioConnection          patchCord280(pwMixer9a, 0, waveformMod9a, 1);
AudioConnection          patchCord281(pwMixer9b, 0, waveformMod9b, 1);
AudioConnection          patchCord282(pwMixer10b, 0, waveformMod10b, 1);
AudioConnection          patchCord283(pwMixer10a, 0, waveformMod10a, 1);
AudioConnection          patchCord284(pwMixer11a, 0, waveformMod11a, 1);
AudioConnection          patchCord285(pwMixer11b, 0, waveformMod11b, 1);
AudioConnection          patchCord286(pwMixer12a, 0, waveformMod12a, 1);
AudioConnection          patchCord287(pwMixer12b, 0, waveformMod12b, 1);

AudioConnection          patchCord52(waveformMod1b, 0, waveformMixer1, 1);
AudioConnection          patchCord53(waveformMod1b, 0, oscFX1, 1);
AudioConnection          patchCord54(waveformMod1a, 0, waveformMixer1, 0);
AudioConnection          patchCord55(waveformMod1a, 0, oscFX1, 0);
AudioConnection          patchCord56(waveformMod2b, 0, waveformMixer2, 1);
AudioConnection          patchCord57(waveformMod2b, 0, oscFX2, 1);
AudioConnection          patchCord58(waveformMod4a, 0, waveformMixer4, 0);
AudioConnection          patchCord59(waveformMod4a, 0, oscFX4, 0);
AudioConnection          patchCord60(waveformMod2a, 0, waveformMixer2, 0);
AudioConnection          patchCord61(waveformMod2a, 0, oscFX2, 0);
AudioConnection          patchCord62(waveformMod4b, 0, waveformMixer4, 1);
AudioConnection          patchCord63(waveformMod4b, 0, oscFX4, 1);
AudioConnection          patchCord64(waveformMod3a, 0, oscFX3, 0);
AudioConnection          patchCord65(waveformMod3a, 0, waveformMixer3, 0);
AudioConnection          patchCord66(waveformMod3b, 0, oscFX3, 1);
AudioConnection          patchCord67(waveformMod3b, 0, waveformMixer3, 1);
AudioConnection          patchCord169(waveformMod5a, 0, waveformMixer5, 0);
AudioConnection          patchCord170(waveformMod5a, 0, oscFX5, 0);
AudioConnection          patchCord171(waveformMod5b, 0, waveformMixer5, 1);
AudioConnection          patchCord172(waveformMod5b, 0, oscFX5, 1);
AudioConnection          patchCord173(waveformMod6a, 0, waveformMixer6, 0);
AudioConnection          patchCord174(waveformMod6a, 0, oscFX6, 0);
AudioConnection          patchCord175(waveformMod6b, 0, waveformMixer6, 1);
AudioConnection          patchCord176(waveformMod6b, 0, oscFX6, 1);
AudioConnection          patchCord300(waveformMod7b, 0, waveformMixer7, 1);
AudioConnection          patchCord301(waveformMod7b, 0, oscFX7, 1);
AudioConnection          patchCord302(waveformMod7a, 0, waveformMixer7, 0);
AudioConnection          patchCord303(waveformMod7a, 0, oscFX7, 0);
AudioConnection          patchCord304(waveformMod8b, 0, waveformMixer8, 1);
AudioConnection          patchCord305(waveformMod8b, 0, oscFX8, 1);
AudioConnection          patchCord306(waveformMod10a, 0, waveformMixer10, 0);
AudioConnection          patchCord307(waveformMod10a, 0, oscFX10, 0);
AudioConnection          patchCord308(waveformMod8a, 0, waveformMixer8, 0);
AudioConnection          patchCord309(waveformMod8a, 0, oscFX8, 0);
AudioConnection          patchCord310(waveformMod10b, 0, waveformMixer10, 1);
AudioConnection          patchCord311(waveformMod10b, 0, oscFX10, 1);
AudioConnection          patchCord312(waveformMod9a, 0, oscFX9, 0);
AudioConnection          patchCord313(waveformMod9a, 0, waveformMixer9, 0);
AudioConnection          patchCord314(waveformMod9b, 0, oscFX9, 1);
AudioConnection          patchCord315(waveformMod9b, 0, waveformMixer9, 1);
AudioConnection          patchCord316(waveformMod11a, 0, waveformMixer11, 0);
AudioConnection          patchCord317(waveformMod11a, 0, oscFX11, 0);
AudioConnection          patchCord318(waveformMod11b, 0, waveformMixer11, 1);
AudioConnection          patchCord319(waveformMod11b, 0, oscFX11, 1);
AudioConnection          patchCord320(waveformMod12a, 0, waveformMixer12, 0);
AudioConnection          patchCord321(waveformMod12a, 0, oscFX12, 0);
AudioConnection          patchCord322(waveformMod12b, 0, waveformMixer12, 1);
AudioConnection          patchCord323(waveformMod12b, 0, oscFX12, 1);

AudioConnection          patchCord68(filterEnvelope4, 0, filterModMixer4, 0);
AudioConnection          patchCord69(filterEnvelope4, 0, pwMixer4a, 2);
AudioConnection          patchCord70(filterEnvelope4, 0, pwMixer4b, 2);
AudioConnection          patchCord71(filterEnvelope1, 0, filterModMixer1, 0);
AudioConnection          patchCord72(filterEnvelope1, 0, pwMixer1a, 2);
AudioConnection          patchCord73(filterEnvelope1, 0, pwMixer1b, 2);
AudioConnection          patchCord74(filterEnvelope3, 0, filterModMixer3, 0);
AudioConnection          patchCord75(filterEnvelope3, 0, pwMixer3a, 2);
AudioConnection          patchCord76(filterEnvelope3, 0, pwMixer3b, 2);
AudioConnection          patchCord77(filterEnvelope2, 0, filterModMixer2, 0);
AudioConnection          patchCord78(filterEnvelope2, 0, pwMixer2a, 2);
AudioConnection          patchCord79(filterEnvelope2, 0, pwMixer2b, 2);
AudioConnection          patchCord177(filterEnvelope5, 0, filterModMixer5, 0);
AudioConnection          patchCord178(filterEnvelope5, 0, pwMixer5a, 2);
AudioConnection          patchCord179(filterEnvelope5, 0, pwMixer5b, 2);
AudioConnection          patchCord180(filterEnvelope6, 0, filterModMixer6, 0);
AudioConnection          patchCord181(filterEnvelope6, 0, pwMixer6a, 2);
AudioConnection          patchCord182(filterEnvelope6, 0, pwMixer6b, 2);

AudioConnection          patchCord324(filterEnvelope10, 0, filterModMixer10, 0);
AudioConnection          patchCord325(filterEnvelope10, 0, pwMixer10a, 2);
AudioConnection          patchCord326(filterEnvelope10, 0, pwMixer10b, 2);
AudioConnection          patchCord327(filterEnvelope7, 0, filterModMixer7, 0);
AudioConnection          patchCord328(filterEnvelope7, 0, pwMixer7a, 2);
AudioConnection          patchCord329(filterEnvelope7, 0, pwMixer7b, 2);
AudioConnection          patchCord330(filterEnvelope9, 0, filterModMixer9, 0);
AudioConnection          patchCord331(filterEnvelope9, 0, pwMixer9a, 2);
AudioConnection          patchCord332(filterEnvelope9, 0, pwMixer9b, 2);
AudioConnection          patchCord333(filterEnvelope8, 0, filterModMixer8, 0);
AudioConnection          patchCord334(filterEnvelope8, 0, pwMixer8a, 2);
AudioConnection          patchCord335(filterEnvelope8, 0, pwMixer8b, 2);
AudioConnection          patchCord336(filterEnvelope11, 0, filterModMixer11, 0);
AudioConnection          patchCord337(filterEnvelope11, 0, pwMixer11a, 2);
AudioConnection          patchCord338(filterEnvelope11, 0, pwMixer11b, 2);
AudioConnection          patchCord339(filterEnvelope12, 0, filterModMixer12, 0);
AudioConnection          patchCord340(filterEnvelope12, 0, pwMixer12a, 2);
AudioConnection          patchCord341(filterEnvelope12, 0, pwMixer12b, 2);

AudioConnection          patchCord80(oscFX1, 0, waveformMixer1, 3);
AudioConnection          patchCord81(oscFX2, 0, waveformMixer2, 3);
AudioConnection          patchCord82(oscFX4, 0, waveformMixer4, 3);
AudioConnection          patchCord83(oscFX3, 0, waveformMixer3, 3);
AudioConnection          patchCord183(oscFX5, 0, waveformMixer5, 3);
AudioConnection          patchCord184(oscFX6, 0, waveformMixer6, 3);
AudioConnection          patchCord342(oscFX7, 0, waveformMixer7, 3);
AudioConnection          patchCord343(oscFX8, 0, waveformMixer8, 3);
AudioConnection          patchCord344(oscFX9, 0, waveformMixer9, 3);
AudioConnection          patchCord345(oscFX10, 0, waveformMixer10, 3);
AudioConnection          patchCord346(oscFX11, 0, waveformMixer11, 3);
AudioConnection          patchCord347(oscFX12, 0, waveformMixer12, 3);

AudioConnection          patchCord84(waveformMixer1, 0, filter1, 0);
AudioConnection          patchCord85(waveformMixer2, 0, filter2, 0);
AudioConnection          patchCord86(filterModMixer1, 0, filter1, 1);
AudioConnection          patchCord87(waveformMixer3, 0, filter3, 0);
AudioConnection          patchCord88(filterModMixer2, 0, filter2, 1);
AudioConnection          patchCord89(filterModMixer3, 0, filter3, 1);
AudioConnection          patchCord90(waveformMixer4, 0, filter4, 0);
AudioConnection          patchCord91(filterModMixer4, 0, filter4, 1);
AudioConnection          patchCord185(waveformMixer5, 0, filter5, 0);
AudioConnection          patchCord186(filterModMixer5, 0, filter5, 1);
AudioConnection          patchCord187(waveformMixer6, 0, filter6, 0);
AudioConnection          patchCord188(filterModMixer6, 0, filter6, 1);

AudioConnection          patchCord348(waveformMixer7, 0, filter7, 0);
AudioConnection          patchCord349(waveformMixer8, 0, filter8, 0);
AudioConnection          patchCord350(filterModMixer7, 0, filter7, 1);
AudioConnection          patchCord351(waveformMixer9, 0, filter9, 0);
AudioConnection          patchCord352(filterModMixer8, 0, filter8, 1);
AudioConnection          patchCord354(filterModMixer9, 0, filter9, 1);
AudioConnection          patchCord355(waveformMixer10, 0, filter10, 0);
AudioConnection          patchCord356(filterModMixer10, 0, filter10, 1);
AudioConnection          patchCord357(waveformMixer11, 0, filter11, 0);
AudioConnection          patchCord358(filterModMixer11, 0, filter11, 1);
AudioConnection          patchCord359(waveformMixer12, 0, filter12, 0);
AudioConnection          patchCord360(filterModMixer12, 0, filter12, 1);

AudioConnection          patchCord92(filter2, 0, filterMixer2, 0);
AudioConnection          patchCord93(filter2, 1, filterMixer2, 1);
AudioConnection          patchCord94(filter2, 2, filterMixer2, 2);
AudioConnection          patchCord95(filter1, 0, filterMixer1, 0);
AudioConnection          patchCord96(filter1, 1, filterMixer1, 1);
AudioConnection          patchCord97(filter1, 2, filterMixer1, 2);
AudioConnection          patchCord98(filter3, 0, filterMixer3, 0);
AudioConnection          patchCord99(filter3, 1, filterMixer3, 1);
AudioConnection          patchCord100(filter3, 2, filterMixer3, 2);
AudioConnection          patchCord101(filter4, 0, filterMixer4, 0);
AudioConnection          patchCord102(filter4, 1, filterMixer4, 1);
AudioConnection          patchCord103(filter4, 2, filterMixer4, 2);
AudioConnection          patchCord201(filter5, 0, filterMixer5, 0);
AudioConnection          patchCord189(filter5, 1, filterMixer5, 1);
AudioConnection          patchCord190(filter5, 2, filterMixer5, 2);
AudioConnection          patchCord202(filter6, 0, filterMixer6, 0);
AudioConnection          patchCord191(filter6, 1, filterMixer6, 1);
AudioConnection          patchCord192(filter6, 2, filterMixer6, 2);

AudioConnection          patchCord361(filter8, 0, filterMixer8, 0);
AudioConnection          patchCord362(filter8, 1, filterMixer8, 1);
AudioConnection          patchCord363(filter8, 2, filterMixer8, 2);
AudioConnection          patchCord364(filter7, 0, filterMixer7, 0);
AudioConnection          patchCord365(filter7, 1, filterMixer7, 1);
AudioConnection          patchCord366(filter7, 2, filterMixer7, 2);
AudioConnection          patchCord367(filter9, 0, filterMixer9, 0);
AudioConnection          patchCord368(filter9, 1, filterMixer9, 1);
AudioConnection          patchCord369(filter9, 2, filterMixer9, 2);
AudioConnection          patchCord370(filter10, 0, filterMixer10, 0);
AudioConnection          patchCord371(filter10, 1, filterMixer10, 1);
AudioConnection          patchCord372(filter10, 2, filterMixer10, 2);
AudioConnection          patchCord373(filter11, 0, filterMixer11, 0);
AudioConnection          patchCord374(filter11, 1, filterMixer11, 1);
AudioConnection          patchCord375(filter11, 2, filterMixer11, 2);
AudioConnection          patchCord376(filter12, 0, filterMixer12, 0);
AudioConnection          patchCord377(filter12, 1, filterMixer12, 1);
AudioConnection          patchCord378(filter12, 2, filterMixer12, 2);

AudioConnection          patchCord104(filterMixer2, ampEnvelope2);
AudioConnection          patchCord105(filterMixer3, ampEnvelope3);
AudioConnection          patchCord106(filterMixer1, ampEnvelope1);
AudioConnection          patchCord107(filterMixer4, ampEnvelope4);
AudioConnection          patchCord193(filterMixer5, ampEnvelope5);
AudioConnection          patchCord194(filterMixer6, ampEnvelope6);
AudioConnection          patchCord222(filterMixer7, ampEnvelope7);
AudioConnection          patchCord223(filterMixer8, ampEnvelope8);
AudioConnection          patchCord224(filterMixer9, ampEnvelope9);
AudioConnection          patchCord225(filterMixer10, ampEnvelope10);
AudioConnection          patchCord226(filterMixer11, ampEnvelope11);
AudioConnection          patchCord227(filterMixer12, ampEnvelope12);

AudioConnection          patchCord111(ampEnvelope1, 0, voiceMixer1, 0);
AudioConnection          patchCord108(ampEnvelope2, 0, voiceMixer1, 1);
AudioConnection          patchCord109(ampEnvelope3, 0, voiceMixer1, 2);
AudioConnection          patchCord110(ampEnvelope4, 0, voiceMixer1, 3);
AudioConnection          patchCord195(ampEnvelope5, 0, voiceMixer2, 0);
AudioConnection          patchCord196(ampEnvelope6, 0, voiceMixer2, 1);
AudioConnection          patchCord216(ampEnvelope7, 0, voiceMixer2, 2);
AudioConnection          patchCord217(ampEnvelope8, 0, voiceMixer2, 3);
AudioConnection          patchCord218(ampEnvelope9, 0, voiceMixer3, 0);
AudioConnection          patchCord219(ampEnvelope10, 0, voiceMixer3, 1);
AudioConnection          patchCord220(ampEnvelope11, 0, voiceMixer3, 2);
AudioConnection          patchCord221(ampEnvelope12, 0, voiceMixer3, 3);

//Mod sources
AudioConnection          patchCord121(oscModMixer1a, 0, waveformMod1a, 0);
AudioConnection          patchCord122(oscModMixer1b, 0, waveformMod1b, 0);
AudioConnection          patchCord123(oscModMixer2a, 0, waveformMod2a, 0);
AudioConnection          patchCord124(oscModMixer2b, 0, waveformMod2b, 0);
AudioConnection          patchCord125(oscModMixer3a, 0, waveformMod3a, 0);
AudioConnection          patchCord126(oscModMixer3b, 0, waveformMod3b, 0);
AudioConnection          patchCord127(oscModMixer4a, 0, waveformMod4a, 0);
AudioConnection          patchCord128(oscModMixer4b, 0, waveformMod4b, 0);
AudioConnection          patchCord157(oscModMixer5a, 0, waveformMod5a, 0);
AudioConnection          patchCord158(oscModMixer5b, 0, waveformMod5b, 0);
AudioConnection          patchCord159(oscModMixer6a, 0, waveformMod6a, 0);
AudioConnection          patchCord160(oscModMixer6b, 0, waveformMod6b, 0);

AudioConnection          patchCord379(oscModMixer7a, 0, waveformMod7a, 0);
AudioConnection          patchCord380(oscModMixer7b, 0, waveformMod7b, 0);
AudioConnection          patchCord381(oscModMixer8a, 0, waveformMod8a, 0);
AudioConnection          patchCord382(oscModMixer8b, 0, waveformMod8b, 0);
AudioConnection          patchCord383(oscModMixer9a, 0, waveformMod9a, 0);
AudioConnection          patchCord384(oscModMixer9b, 0, waveformMod9b, 0);
AudioConnection          patchCord385(oscModMixer10a, 0, waveformMod10a, 0);
AudioConnection          patchCord386(oscModMixer10b, 0, waveformMod10b, 0);
AudioConnection          patchCord387(oscModMixer11a, 0, waveformMod11a, 0);
AudioConnection          patchCord388(oscModMixer11b, 0, waveformMod11b, 0);
AudioConnection          patchCord389(oscModMixer12a, 0, waveformMod12a, 0);
AudioConnection          patchCord390(oscModMixer12b, 0, waveformMod12b, 0);

//Pitch env
AudioConnection          patchCord129(filterEnvelope1, 0, oscModMixer1a, 1);
AudioConnection          patchCord130(filterEnvelope1, 0, oscModMixer1b, 1);
AudioConnection          patchCord131(filterEnvelope2, 0, oscModMixer2a, 1);
AudioConnection          patchCord132(filterEnvelope2, 0, oscModMixer2b, 1);
AudioConnection          patchCord133(filterEnvelope3, 0, oscModMixer3a, 1);
AudioConnection          patchCord134(filterEnvelope3, 0, oscModMixer3b, 1);
AudioConnection          patchCord135(filterEnvelope4, 0, oscModMixer4a, 1);
AudioConnection          patchCord136(filterEnvelope4, 0, oscModMixer4b, 1);
AudioConnection          patchCord161(filterEnvelope5, 0, oscModMixer5a, 1);
AudioConnection          patchCord162(filterEnvelope5, 0, oscModMixer5b, 1);
AudioConnection          patchCord163(filterEnvelope6, 0, oscModMixer6a, 1);
AudioConnection          patchCord164(filterEnvelope6, 0, oscModMixer6b, 1);

AudioConnection          patchCord391(filterEnvelope7, 0, oscModMixer7a, 1);
AudioConnection          patchCord392(filterEnvelope7, 0, oscModMixer7b, 1);
AudioConnection          patchCord393(filterEnvelope8, 0, oscModMixer8a, 1);
AudioConnection          patchCord394(filterEnvelope8, 0, oscModMixer8b, 1);
AudioConnection          patchCord395(filterEnvelope9, 0, oscModMixer9a, 1);
AudioConnection          patchCord396(filterEnvelope9, 0, oscModMixer9b, 1);
AudioConnection          patchCord397(filterEnvelope10, 0, oscModMixer10a, 1);
AudioConnection          patchCord398(filterEnvelope10, 0, oscModMixer10b, 1);
AudioConnection          patchCord399(filterEnvelope11, 0, oscModMixer11a, 1);
AudioConnection          patchCord400(filterEnvelope11, 0, oscModMixer11b, 1);
AudioConnection          patchCord401(filterEnvelope12, 0, oscModMixer12a, 1);
AudioConnection          patchCord402(filterEnvelope12, 0, oscModMixer12b, 1);

//Glide
AudioConnection          patchCord22(glide1, 0, oscModMixer1a, 2);
AudioConnection          patchCord204(glide1, 0, oscModMixer1b, 2);
AudioConnection          patchCord205(glide2, 0, oscModMixer2a, 2);
AudioConnection          patchCord206(glide2, 0, oscModMixer2b, 2);
AudioConnection          patchCord207(glide3, 0, oscModMixer3a, 2);
AudioConnection          patchCord208(glide3, 0, oscModMixer3b, 2);
AudioConnection          patchCord209(glide4, 0, oscModMixer4a, 2);
AudioConnection          patchCord210(glide4, 0, oscModMixer4b, 2);
AudioConnection          patchCord211(glide5, 0, oscModMixer5a, 2);
AudioConnection          patchCord212(glide5, 0, oscModMixer5b, 2);
AudioConnection          patchCord213(glide6, 0, oscModMixer6a, 2);
AudioConnection          patchCord214(glide6, 0, oscModMixer6b, 2);

AudioConnection          patchCord403(glide7, 0, oscModMixer7a, 2);
AudioConnection          patchCord404(glide7, 0, oscModMixer7b, 2);
AudioConnection          patchCord405(glide8, 0, oscModMixer8a, 2);
AudioConnection          patchCord406(glide8, 0, oscModMixer8b, 2);
AudioConnection          patchCord407(glide9, 0, oscModMixer9a, 2);
AudioConnection          patchCord408(glide9, 0, oscModMixer9b, 2);
AudioConnection          patchCord409(glide10, 0, oscModMixer10a, 2);
AudioConnection          patchCord410(glide10, 0, oscModMixer10b, 2);
AudioConnection          patchCord411(glide11, 0, oscModMixer11a, 2);
AudioConnection          patchCord412(glide11, 0, oscModMixer11b, 2);
AudioConnection          patchCord413(glide12, 0, oscModMixer12a, 2);
AudioConnection          patchCord414(glide12, 0, oscModMixer12b, 2);

//X Mod
AudioConnection          patchCord417(waveformMod1b, 0, oscModMixer1a, 3);
AudioConnection          patchCord429(waveformMod1a, 0, oscModMixer1b, 3);
AudioConnection          patchCord418(waveformMod2b, 0, oscModMixer2a, 3);
AudioConnection          patchCord430(waveformMod2a, 0, oscModMixer2b, 3);
AudioConnection          patchCord419(waveformMod3b, 0, oscModMixer3a, 3);
AudioConnection          patchCord431(waveformMod3a, 0, oscModMixer3b, 3);
AudioConnection          patchCord420(waveformMod4b, 0, oscModMixer4a, 3);
AudioConnection          patchCord432(waveformMod4a, 0, oscModMixer4b, 3);
AudioConnection          patchCord421(waveformMod5b, 0, oscModMixer5a, 3);
AudioConnection          patchCord433(waveformMod5a, 0, oscModMixer5b, 3);
AudioConnection          patchCord422(waveformMod6b, 0, oscModMixer6a, 3);
AudioConnection          patchCord434(waveformMod6a, 0, oscModMixer6b, 3);

AudioConnection          patchCord423(waveformMod7b, 0, oscModMixer7a, 3);
AudioConnection          patchCord435(waveformMod7a, 0, oscModMixer7b, 3);
AudioConnection          patchCord424(waveformMod8b, 0, oscModMixer8a, 3);
AudioConnection          patchCord436(waveformMod8a, 0, oscModMixer8b, 3);
AudioConnection          patchCord425(waveformMod9b, 0, oscModMixer9a, 3);
AudioConnection          patchCord437(waveformMod9a, 0, oscModMixer9b, 3);
AudioConnection          patchCord426(waveformMod10b, 0, oscModMixer10a, 3);
AudioConnection          patchCord438(waveformMod10a, 0, oscModMixer10b, 3);
AudioConnection          patchCord427(waveformMod11b, 0, oscModMixer11a, 3);
AudioConnection          patchCord439(waveformMod11a, 0, oscModMixer11b, 3);
AudioConnection          patchCord428(waveformMod12b, 0, oscModMixer12a, 3);
AudioConnection          patchCord440(waveformMod12a, 0, oscModMixer12b, 3);

//Noise
AudioConnection          patchCord452(pink, 0, noiseMixer1, 0);
AudioConnection          patchCord453(pink, 0, noiseMixer2, 0);
AudioConnection          patchCord454(pink, 0, noiseMixer3, 0);
AudioConnection          patchCord455(pink, 0, noiseMixer4, 0);
AudioConnection          patchCord456(pink, 0, noiseMixer5, 0);
AudioConnection          patchCord457(pink, 0, noiseMixer6, 0);
AudioConnection          patchCord458(pink, 0, noiseMixer7, 0);
AudioConnection          patchCord459(pink, 0, noiseMixer8, 0);
AudioConnection          patchCord460(pink, 0, noiseMixer9, 0);
AudioConnection          patchCord461(pink, 0, noiseMixer10, 0);
AudioConnection          patchCord462(pink, 0, noiseMixer11, 0);
AudioConnection          patchCord463(pink, 0, noiseMixer12, 0);

AudioConnection          patchCord137(white, 0, noiseMixer1, 1);
AudioConnection          patchCord138(white, 0, noiseMixer2, 1);
AudioConnection          patchCord442(white, 0, noiseMixer3, 1);
AudioConnection          patchCord443(white, 0, noiseMixer4, 1);
AudioConnection          patchCord444(white, 0, noiseMixer5, 1);
AudioConnection          patchCord445(white, 0, noiseMixer6, 1);
AudioConnection          patchCord446(white, 0, noiseMixer7, 1);
AudioConnection          patchCord447(white, 0, noiseMixer8, 1);
AudioConnection          patchCord448(white, 0, noiseMixer9, 1);
AudioConnection          patchCord449(white, 0, noiseMixer10, 1);
AudioConnection          patchCord450(white, 0, noiseMixer11, 1);
AudioConnection          patchCord451(white, 0, noiseMixer12, 1);

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


// Oscillator configurations.
struct Patch {
    // What is this?
    AudioMixer4 &voiceMixer_; // 1-3

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
        noiseMixer1
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
        noiseMixer2
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
        noiseMixer3
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
        noiseMixer4
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
        noiseMixer5
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
        noiseMixer6
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
        noiseMixer7
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
        noiseMixer8
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
        noiseMixer9
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
        noiseMixer10
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
        noiseMixer11
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
        noiseMixer12
    }
};


//AudioSynthWaveform* filterLfo[12] = {&filterLfo1, &filterLfo2, &filterLfo}
//AudioSynthWaveformDc* pwa[12] = {&pwa1, &pwa2, &pwa3, &pwa4, &pwa5, &pwa6, &pwa7, &pwa8, &pwa9, &pwa10, &pwa11, &pwa12 };
//AudioSynthWaveformDc* pwb[12] = {&pwb1, &pwb2, &pwb3, &pwb4, &pwb5, &pwb6, &pwb7, &pwb8, &pwb9, &pwb10, &pwb11, &pwb12 };
//AudioMixer4* noiseMixer[12] = {&noiseMixer1, &noiseMixer2, &noiseMixer3, &noiseMixer4, &noiseMixer5, &noiseMixer6, &noiseMixer7, &noiseMixer8, &noiseMixer9, &noiseMixer10, &noiseMixer11, &noiseMixer12 };

// The 12 oscillator pairs in one data structure to allow for easier programming.
Patch Oscillators[12] = {
    {
        voiceMixer1,
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
        nullptr
    },
    {
        voiceMixer1,
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
        nullptr
    },
    {
        voiceMixer1,
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
        nullptr
    },
    {
        voiceMixer1,
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
        nullptr
    },
    {
        voiceMixer2,
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
        nullptr
    },
    {
        voiceMixer2,
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
        nullptr
    },
    {
        voiceMixer2,
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
        nullptr
    },
    {
        voiceMixer2,
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
        nullptr
    },
    {
        voiceMixer3,
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
        nullptr
    },
    {
        voiceMixer3,
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
        nullptr
    },
    {
        voiceMixer3,
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
        nullptr
    },
    {
        voiceMixer3,
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
        nullptr
    }
};

// Undefine custom classes renaming:
#undef AudioSynthWaveform
#undef AudioSynthWaveformModulated
#undef AudioEffectEnvelope
#endif
