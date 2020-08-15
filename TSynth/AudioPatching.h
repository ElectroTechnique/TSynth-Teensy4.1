//waveformX      -->   waveformMixerX   -->   voiceMixer1-3   -->   voiceMixerM
//WAVEFORMLEVEL        oscA/BLevel             VELOCITY             VOICEMIXERLEVEL/UNISONVOICEMIXERLEVEL
// GUItool: begin automatically generated code
AudioOutputUSB           usbAudio;       //xy=2356,593
AudioSynthWaveformDc     constant1Dc;    //xy=69.5,437
AudioSynthNoisePink      pink;           //xy=268,349
AudioSynthNoiseWhite     white;           //xy=268,359
AudioMixer4              noiseMixer;       //xy=288,369
AudioSynthWaveformTS       pwmLfo;         //xy=92,208
AudioSynthWaveformDc     pwa;            //xy=107,256
AudioSynthWaveformDc     pwb;            //xy=110,301

AudioEffectEnvelope      filterEnvelope1;   //xy=628,300
AudioEffectEnvelope      filterEnvelope2;   //xy=640,604
AudioEffectEnvelope      filterEnvelope3;   //xy=636,857
AudioEffectEnvelope      filterEnvelope4;   //xy=617,1107
AudioEffectEnvelope      filterEnvelope5;   //xy=638,300
AudioEffectEnvelope      filterEnvelope6;   //xy=629,1107
AudioEffectEnvelope      filterEnvelope7;   //xy=629,1107
AudioEffectEnvelope      filterEnvelope8;   //xy=638,300
AudioEffectEnvelope      filterEnvelope9;   //xy=617,1107
AudioEffectEnvelope      filterEnvelope10;   //xy=628,300
AudioEffectEnvelope      filterEnvelope11;   //xy=636,857
AudioEffectEnvelope      filterEnvelope12;   //xy=640,604

AudioMixer4              pwMixer1a;       //xy=281,169
AudioMixer4              pwMixer1b;       //xy=287,272
AudioMixer4              pwMixer2a;         //xy=323.75,472.75
AudioMixer4              pwMixer2b;         //xy=323.75,553.75
AudioMixer4              pwMixer3a;         //xy=353.75,688.75
AudioMixer4              pwMixer3b;         //xy=358.75,763.75
AudioMixer4              pwMixer4b;         //xy=313.75,954.75
AudioMixer4              pwMixer4a;         //xy=314.75,887.75
AudioMixer4              pwMixer5a;         //xy=323.75,472.75
AudioMixer4              pwMixer5b;         //xy=323.75,553.75
AudioMixer4              pwMixer6a;         //xy=353.75,688.75
AudioMixer4              pwMixer6b;         //xy=358.75,763.75
AudioMixer4              pwMixer7a;       //xy=281,169
AudioMixer4              pwMixer7b;       //xy=287,272
AudioMixer4              pwMixer8b;         //xy=313.75,954.75
AudioMixer4              pwMixer8a;         //xy=314.75,887.75
AudioMixer4              pwMixer9a;         //xy=323.75,472.75
AudioMixer4              pwMixer9b;         //xy=323.75,553.75
AudioMixer4              pwMixer10a;         //xy=353.75,688.75
AudioMixer4              pwMixer10b;         //xy=358.75,763.75
AudioMixer4              pwMixer11a;         //xy=323.75,472.75
AudioMixer4              pwMixer11b;         //xy=323.75,553.75
AudioMixer4              pwMixer12a;         //xy=353.75,688.75
AudioMixer4              pwMixer12b;         //xy=358.75,763.75


AudioSynthWaveformTS       pitchLfo;         //xy=122,111
AudioSynthWaveformDc     glide1;          //xy=124,152
AudioSynthWaveformDc     glide2;          //xy=124,152
AudioSynthWaveformDc     glide3;          //xy=124,152
AudioSynthWaveformDc     glide4;          //xy=124,152
AudioSynthWaveformDc     glide5;          //xy=124,152
AudioSynthWaveformDc     glide6;          //xy=124,152
AudioSynthWaveformDc     glide7;          //xy=124,152
AudioSynthWaveformDc     glide8;          //xy=124,152
AudioSynthWaveformDc     glide9;          //xy=124,152
AudioSynthWaveformDc     glide10;          //xy=124,152
AudioSynthWaveformDc     glide11;          //xy=124,152
AudioSynthWaveformDc     glide12;          //xy=124,152


AudioSynthWaveformDc     pitchBend;      //xy=128,74
AudioSynthWaveformTS       filterLfo;         //xy=166,632
AudioSynthWaveformDc     keytracking1;    //xy=176,745
AudioSynthWaveformDc     keytracking2;    //xy=196,745
AudioSynthWaveformDc     keytracking3;    //xy=216,745
AudioSynthWaveformDc     keytracking4;    //xy=236,745
AudioSynthWaveformDc     keytracking5;    //xy=216,745
AudioSynthWaveformDc     keytracking6;    //xy=236,745
AudioSynthWaveformDc     keytracking7;    //xy=176,745
AudioSynthWaveformDc     keytracking8;    //xy=196,745
AudioSynthWaveformDc     keytracking9;    //xy=216,745
AudioSynthWaveformDc     keytracking10;    //xy=236,745
AudioSynthWaveformDc     keytracking11;    //xy=216,745
AudioSynthWaveformDc     keytracking12;    //xy=236,745


AudioMixer4              oscGlobalModMixer;    //xy=296,95
AudioMixer4              oscModMixer1a;         //xy=550,165
AudioMixer4              oscModMixer1b;         //xy=550,195
AudioMixer4              oscModMixer2a;         //xy=550,205
AudioMixer4              oscModMixer2b;         //xy=550,225
AudioMixer4              oscModMixer3a;         //xy=550,245
AudioMixer4              oscModMixer3b;         //xy=550,265
AudioMixer4              oscModMixer4a;         //xy=550,285
AudioMixer4              oscModMixer4b;         //xy=550,305
AudioMixer4              oscModMixer5a;         //xy=550,245
AudioMixer4              oscModMixer5b;         //xy=550,265
AudioMixer4              oscModMixer6a;         //xy=550,285
AudioMixer4              oscModMixer6b;         //xy=550,305
AudioMixer4              oscModMixer7a;         //xy=550,165
AudioMixer4              oscModMixer7b;         //xy=550,195
AudioMixer4              oscModMixer8a;         //xy=550,205
AudioMixer4              oscModMixer8b;         //xy=550,225
AudioMixer4              oscModMixer9a;         //xy=550,245
AudioMixer4              oscModMixer9b;         //xy=550,265
AudioMixer4              oscModMixer10a;         //xy=550,285
AudioMixer4              oscModMixer10b;         //xy=550,305
AudioMixer4              oscModMixer11a;         //xy=550,245
AudioMixer4              oscModMixer11b;         //xy=550,265
AudioMixer4              oscModMixer12a;         //xy=550,285
AudioMixer4              oscModMixer12b;         //xy=550,305

AudioSynthWaveformModulatedTS   waveformMod1b;  //xy=503,170
AudioSynthWaveformModulatedTS  waveformMod1a;  //xy=507,114
AudioSynthWaveformModulatedTS  waveformMod2b;  //xy=513,550
AudioSynthWaveformModulatedTS  waveformMod4a;  //xy=519,899
AudioSynthWaveformModulatedTS  waveformMod2a;  //xy=521,458
AudioSynthWaveformModulatedTS waveformMod4b;  //xy=525,940
AudioSynthWaveformModulatedTS waveformMod3a;  //xy=532,678
AudioSynthWaveformModulatedTS waveformMod3b;  //xy=535,744
AudioSynthWaveformModulatedTS waveformMod5a;  //xy=521,458
AudioSynthWaveformModulatedTS waveformMod5b;  //xy=525,940
AudioSynthWaveformModulatedTS waveformMod6a;  //xy=532,678
AudioSynthWaveformModulatedTS waveformMod6b;  //xy=535,744
AudioSynthWaveformModulatedTS waveformMod7b;  //xy=503,170
AudioSynthWaveformModulatedTS waveformMod7a;  //xy=507,114
AudioSynthWaveformModulatedTS waveformMod8b;  //xy=513,550
AudioSynthWaveformModulatedTS waveformMod8a;  //xy=519,899
AudioSynthWaveformModulatedTS waveformMod9a;  //xy=521,458
AudioSynthWaveformModulatedTS waveformMod9b;  //xy=525,940
AudioSynthWaveformModulatedTS waveformMod10a;  //xy=532,678
AudioSynthWaveformModulatedTS waveformMod10b;  //xy=535,744
AudioSynthWaveformModulatedTS waveformMod11a;  //xy=521,458
AudioSynthWaveformModulatedTS waveformMod11b;  //xy=525,940
AudioSynthWaveformModulatedTS waveformMod12a;  //xy=532,678
AudioSynthWaveformModulatedTS waveformMod12b;  //xy=535,744

AudioEffectDigitalCombine oscFX1;       //xy=663,223
AudioEffectDigitalCombine oscFX2;       //xy=684,542
AudioEffectDigitalCombine oscFX4;       //xy=686,1046
AudioEffectDigitalCombine oscFX3;       //xy=687,803
AudioEffectDigitalCombine oscFX5;       //xy=686,1046
AudioEffectDigitalCombine oscFX6;       //xy=687,803
AudioEffectDigitalCombine oscFX7;       //xy=663,223
AudioEffectDigitalCombine oscFX8;       //xy=684,542
AudioEffectDigitalCombine oscFX9;       //xy=686,1046
AudioEffectDigitalCombine oscFX10;       //xy=687,803
AudioEffectDigitalCombine oscFX11;       //xy=686,1046
AudioEffectDigitalCombine oscFX12;       //xy=687,803

AudioMixer4              waveformMixer1; //xy=824,170
AudioMixer4              waveformMixer2; //xy=829,476
AudioMixer4              waveformMixer3; //xy=846,774
AudioMixer4              waveformMixer4; //xy=854,1023
AudioMixer4              waveformMixer5; //xy=846,774
AudioMixer4              waveformMixer6; //xy=854,1023
AudioMixer4              waveformMixer7; //xy=824,170
AudioMixer4              waveformMixer8; //xy=829,476
AudioMixer4              waveformMixer9; //xy=846,774
AudioMixer4              waveformMixer10; //xy=854,1023
AudioMixer4              waveformMixer11; //xy=846,774
AudioMixer4              waveformMixer12; //xy=854,1023

AudioMixer4              filterModMixer1;   //xy=845,292
AudioMixer4              filterModMixer2;   //xy=848,637
AudioMixer4              filterModMixer3;   //xy=852,937
AudioMixer4              filterModMixer4;   //xy=855,1099
AudioMixer4              filterModMixer5;   //xy=852,937
AudioMixer4              filterModMixer6;   //xy=855,1099
AudioMixer4              filterModMixer7;   //xy=845,292
AudioMixer4              filterModMixer8;   //xy=848,637
AudioMixer4              filterModMixer9;   //xy=852,937
AudioMixer4              filterModMixer10;   //xy=855,1099
AudioMixer4              filterModMixer11;   //xy=852,937
AudioMixer4              filterModMixer12;   //xy=855,1099


AudioFilterStateVariable filter1;        //xy=1000,210
AudioFilterStateVariable filter2;        //xy=994,498
AudioFilterStateVariable filter3;        //xy=1002,822
AudioFilterStateVariable filter4;        //xy=1022,1047
AudioFilterStateVariable filter5;        //xy=1002,822
AudioFilterStateVariable filter6;        //xy=1022,1047
AudioFilterStateVariable filter7;        //xy=994,498
AudioFilterStateVariable filter8;        //xy=1000,210
AudioFilterStateVariable filter9;        //xy=1002,822
AudioFilterStateVariable filter10;        //xy=1022,1047
AudioFilterStateVariable filter11;        //xy=1002,822
AudioFilterStateVariable filter12;        //xy=1022,1047

AudioMixer4              filterMixer1;   //xy=1151,214
AudioMixer4              filterMixer2;   //xy=1144,504
AudioMixer4              filterMixer3;   //xy=1144,825
AudioMixer4              filterMixer4;   //xy=1155,1050
AudioMixer4              filterMixer5;   //xy=1151,214
AudioMixer4              filterMixer6;   //xy=1155,1050
AudioMixer4              filterMixer7;   //xy=1144,504
AudioMixer4              filterMixer8;   //xy=1144,825
AudioMixer4              filterMixer9;   //xy=1151,214
AudioMixer4              filterMixer10;   //xy=1155,1050
AudioMixer4              filterMixer11;   //xy=1151,214
AudioMixer4              filterMixer12;   //xy=1155,1050

AudioEffectEnvelope      ampEnvelope1;   //xy=1327,211
AudioEffectEnvelope      ampEnvelope2;   //xy=1315,503
AudioEffectEnvelope      ampEnvelope3;   //xy=1315,823
AudioEffectEnvelope      ampEnvelope4;   //xy=1321,1045
AudioEffectEnvelope      ampEnvelope5;   //xy=1321,1045
AudioEffectEnvelope      ampEnvelope6;   //xy=1327,211
AudioEffectEnvelope      ampEnvelope7;   //xy=1315,503
AudioEffectEnvelope      ampEnvelope8;   //xy=1315,823
AudioEffectEnvelope      ampEnvelope9;   //xy=1321,1045
AudioEffectEnvelope      ampEnvelope10;   //xy=1327,211
AudioEffectEnvelope      ampEnvelope11;   //xy=1321,1045
AudioEffectEnvelope      ampEnvelope12;   //xy=1327,211


AudioMixer4              voiceMixer1;     //xy=1524,570
AudioMixer4              voiceMixer2;     //xy=1524,570
AudioMixer4              voiceMixer3;     //xy=1524,570
AudioMixer4              voiceMixerM;     //xy=1544,580
AudioFilterStateVariable dcOffsetFilter;     //xy=1564,580
AudioEffectEnsemble       ensemble;  //xy=1800,600
Oscilloscope              scope;     
AudioMixer4              effectMixerR;         //xy=1848,625
AudioMixer4              effectMixerL;         //xy=1857,539
AudioOutputI2S           i2s;            //xy=2364,547

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

AudioConnection          patchCord5(pwmLfo, 0, pwMixer1a, 0);
AudioConnection          patchCord6(pwmLfo, 0, pwMixer1b, 0);
AudioConnection          patchCord7(pwmLfo, 0, pwMixer2a, 0);
AudioConnection          patchCord8(pwmLfo, 0, pwMixer2b, 0);
AudioConnection          patchCord9(pwmLfo, 0, pwMixer3a, 0);
AudioConnection          patchCord10(pwmLfo, 0, pwMixer3b, 0);
AudioConnection          patchCord11(pwmLfo, 0, pwMixer4a, 0);
AudioConnection          patchCord12(pwmLfo, 0, pwMixer4b, 0);
AudioConnection          patchCord139(pwmLfo, 0, pwMixer5a, 0);
AudioConnection          patchCord140(pwmLfo, 0, pwMixer5b, 0);
AudioConnection          patchCord141(pwmLfo, 0, pwMixer6a, 0);
AudioConnection          patchCord142(pwmLfo, 0, pwMixer6b, 0);
AudioConnection          patchCord234(pwmLfo, 0, pwMixer7a, 0);
AudioConnection          patchCord235(pwmLfo, 0, pwMixer7b, 0);
AudioConnection          patchCord236(pwmLfo, 0, pwMixer8a, 0);
AudioConnection          patchCord237(pwmLfo, 0, pwMixer8b, 0);
AudioConnection          patchCord238(pwmLfo, 0, pwMixer9a, 0);
AudioConnection          patchCord239(pwmLfo, 0, pwMixer9b, 0);
AudioConnection          patchCord240(pwmLfo, 0, pwMixer10a, 0);
AudioConnection          patchCord241(pwmLfo, 0, pwMixer10b, 0);
AudioConnection          patchCord242(pwmLfo, 0, pwMixer11a, 0);
AudioConnection          patchCord243(pwmLfo, 0, pwMixer11b, 0);
AudioConnection          patchCord244(pwmLfo, 0, pwMixer12a, 0);
AudioConnection          patchCord245(pwmLfo, 0, pwMixer12b, 0);


AudioConnection          patchCord13(pwa, 0, pwMixer1a, 1);
AudioConnection          patchCord14(pwa, 0, pwMixer2a, 1);
AudioConnection          patchCord15(pwa, 0, pwMixer3a, 1);
AudioConnection          patchCord16(pwa, 0, pwMixer4a, 1);
AudioConnection          patchCord17(pwb, 0, pwMixer1b, 1);
AudioConnection          patchCord18(pwb, 0, pwMixer2b, 1);
AudioConnection          patchCord19(pwb, 0, pwMixer3b, 1);
AudioConnection          patchCord20(pwb, 0, pwMixer4b, 1);
AudioConnection          patchCord143(pwa, 0, pwMixer5a, 1);
AudioConnection          patchCord144(pwb, 0, pwMixer5b, 1);
AudioConnection          patchCord145(pwa, 0, pwMixer6a, 1);
AudioConnection          patchCord146(pwb, 0, pwMixer6b, 1);
AudioConnection          patchCord246(pwa, 0, pwMixer7a, 1);
AudioConnection          patchCord247(pwb, 0, pwMixer7b, 1);
AudioConnection          patchCord248(pwa, 0, pwMixer8a, 1);
AudioConnection          patchCord249(pwb, 0, pwMixer8b, 1);
AudioConnection          patchCord250(pwa, 0, pwMixer9a, 1);
AudioConnection          patchCord251(pwb, 0, pwMixer9b, 1);
AudioConnection          patchCord252(pwa, 0, pwMixer10a, 1);
AudioConnection          patchCord253(pwb, 0, pwMixer10b, 1);
AudioConnection          patchCord254(pwa, 0, pwMixer11a, 1);
AudioConnection          patchCord255(pwb, 0, pwMixer11b, 1);
AudioConnection          patchCord256(pwa, 0, pwMixer12a, 1);
AudioConnection          patchCord257(pwb, 0, pwMixer12b, 1);

AudioConnection          patchCord21(pitchLfo, 0, oscGlobalModMixer, 1);
AudioConnection          patchCord23(pitchBend, 0, oscGlobalModMixer, 0);

AudioConnection          patchCord24(filterLfo, 0, filterModMixer1, 1);
AudioConnection          patchCord25(filterLfo, 0, filterModMixer2, 1);
AudioConnection          patchCord26(filterLfo, 0, filterModMixer3, 1);
AudioConnection          patchCord27(filterLfo, 0, filterModMixer4, 1);
AudioConnection          patchCord147(filterLfo, 0, filterModMixer5, 1);
AudioConnection          patchCord148(filterLfo, 0, filterModMixer6, 1);
AudioConnection          patchCord258(filterLfo, 0, filterModMixer7, 1);
AudioConnection          patchCord259(filterLfo, 0, filterModMixer8, 1);
AudioConnection          patchCord260(filterLfo, 0, filterModMixer9, 1);
AudioConnection          patchCord261(filterLfo, 0, filterModMixer10, 1);
AudioConnection          patchCord262(filterLfo, 0, filterModMixer11, 1);
AudioConnection          patchCord263(filterLfo, 0, filterModMixer12, 1);


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


AudioConnection          patchCord32(noiseMixer, 0, waveformMixer1, 2);
AudioConnection          patchCord33(noiseMixer, 0, waveformMixer2, 2);
AudioConnection          patchCord34(noiseMixer, 0, waveformMixer3, 2);
AudioConnection          patchCord35(noiseMixer, 0, waveformMixer4, 2);
AudioConnection          patchCord151(noiseMixer, 0, waveformMixer5, 2);
AudioConnection          patchCord152(noiseMixer, 0, waveformMixer6, 2);
AudioConnection          patchCord270(noiseMixer, 0, waveformMixer7, 2);
AudioConnection          patchCord271(noiseMixer, 0, waveformMixer8, 2);
AudioConnection          patchCord272(noiseMixer, 0, waveformMixer9, 2);
AudioConnection          patchCord273(noiseMixer, 0, waveformMixer10, 2);
AudioConnection          patchCord274(noiseMixer, 0, waveformMixer11, 2);
AudioConnection          patchCord275(noiseMixer, 0, waveformMixer12, 2);


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

AudioConnection          patchCord39(oscGlobalModMixer, 0, oscModMixer1a, 0);
AudioConnection          patchCord40(oscGlobalModMixer, 0, oscModMixer1b, 0);
AudioConnection          patchCord41(oscGlobalModMixer, 0, oscModMixer2a, 0);
AudioConnection          patchCord42(oscGlobalModMixer, 0, oscModMixer2b, 0);
AudioConnection          patchCord43(oscGlobalModMixer, 0, oscModMixer3a, 0);
AudioConnection          patchCord44(oscGlobalModMixer, 0, oscModMixer3b, 0);
AudioConnection          patchCord45(oscGlobalModMixer, 0, oscModMixer4a, 0);
AudioConnection          patchCord46(oscGlobalModMixer, 0, oscModMixer4b, 0);
AudioConnection          patchCord153(oscGlobalModMixer, 0, oscModMixer5a, 0);
AudioConnection          patchCord154(oscGlobalModMixer, 0, oscModMixer5b, 0);
AudioConnection          patchCord155(oscGlobalModMixer, 0, oscModMixer6a, 0);
AudioConnection          patchCord156(oscGlobalModMixer, 0, oscModMixer6b, 0);
AudioConnection          patchCord288(oscGlobalModMixer, 0, oscModMixer7a, 0);
AudioConnection          patchCord289(oscGlobalModMixer, 0, oscModMixer7b, 0);
AudioConnection          patchCord290(oscGlobalModMixer, 0, oscModMixer8a, 0);
AudioConnection          patchCord291(oscGlobalModMixer, 0, oscModMixer8b, 0);
AudioConnection          patchCord292(oscGlobalModMixer, 0, oscModMixer9a, 0);
AudioConnection          patchCord293(oscGlobalModMixer, 0, oscModMixer9b, 0);
AudioConnection          patchCord294(oscGlobalModMixer, 0, oscModMixer10a, 0);
AudioConnection          patchCord295(oscGlobalModMixer, 0, oscModMixer10b, 0);
AudioConnection          patchCord296(oscGlobalModMixer, 0, oscModMixer11a, 0);
AudioConnection          patchCord297(oscGlobalModMixer, 0, oscModMixer11b, 0);
AudioConnection          patchCord298(oscGlobalModMixer, 0, oscModMixer12a, 0);
AudioConnection          patchCord299(oscGlobalModMixer, 0, oscModMixer12b, 0);


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

AudioConnection          patchCord22(glide1, 0, oscModMixer1a, 2);
AudioConnection          patchCord204(glide1, 0, oscModMixer1b, 2);
AudioConnection          patchCord205(glide2, 0, oscModMixer2a, 2);
AudioConnection          patchCord206(glide2, 0, oscModMixer2b, 2);
AudioConnection          patchCord207(glide3, 0, oscModMixer3a, 2);
AudioConnection         patchCord208(glide3, 0, oscModMixer3b, 2);
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


AudioConnection          patchCord137(pink, 0, noiseMixer, 0);
AudioConnection          patchCord138(white, 0, noiseMixer, 1);

AudioConnection          patchCord197(voiceMixer1, 0, voiceMixerM, 0);
AudioConnection          patchCord198(voiceMixer2, 0, voiceMixerM, 1);
AudioConnection          patchCord215(voiceMixer3, 0, voiceMixerM, 2);
AudioConnection          patchCord203(voiceMixerM, 0, dcOffsetFilter, 0);
AudioConnection          patchCord112(dcOffsetFilter, 2, ensemble, 0);
AudioConnection          patchCord415(dcOffsetFilter, 2, scope, 0);
AudioConnection          patchCord113(ensemble, 0, effectMixerL, 1);
AudioConnection          patchCord114(ensemble, 1, effectMixerR, 1);
AudioConnection          patchCord115(dcOffsetFilter, 2, effectMixerL, 0);
AudioConnection          patchCord116(dcOffsetFilter, 2, effectMixerR, 0);
AudioConnection          patchCord117(effectMixerR, 0, usbAudio, 1);
AudioConnection          patchCord118(effectMixerR, 0, i2s, 1);
AudioConnection          patchCord119(effectMixerL, 0, i2s, 0);
AudioConnection          patchCord120(effectMixerL, 0, usbAudio, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=2353,505
// GUItool: end automatically generated code
