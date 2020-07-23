//Values below are just for initialising and will be changed when synth is initialised to current panel controls & EEPROM settings
byte midiChannel = MIDI_CHANNEL_OMNI;//(EEPROM)
String patchName = INITPATCHNAME;
boolean encCW = true;//This is to set the encoder to increment when turned CW - Settings Option
float oscALevel = 1;
float oscBLevel = 1;
float noiseLevel = 0;
int unison = 0;
int oscFX = 0;
float detune = 0.98;
float lfoSyncFreq = 1.0;
float midiClkTimeInterval = 0.0;
float lfoTempoValue = 1.0;
int pitchBendRange = 12;
float modWheelDepth = 0.2f;
float keytrackingAmount = 0.5;//MIDI CC & settings option (EEPROM)
float glideSpeed = 0;
int oscPitchA = 0;
int oscPitchB = 12;
float pitchEnv = 0;
int oscWaveformA = WAVEFORM_SQUARE;
int oscWaveformB = WAVEFORM_SQUARE;
float pwmAmtA = 1;
float pwmAmtB = 1;
float pwmRate = 0.5;
float pwA = 0;
float pwB = 0;
int pwmSource = PWMSOURCELFO;

float filterRes = 1.1;
float filterFreq = 12000;
float filterOctave = 7.0;
float filterMix = 0;
int filterMixStr = 0;//For display
float filterEnv = 0;
float oscLfoAmt = 0;
float oscLfoRate = 4;
int oscLFOWaveform = WAVEFORM_SINE;
int oscLfoRetrig = 0;
int oscLFOMidiClkSync = 0;//MIDI Only
String oscLFOTimeDivStr = "";//For display
float filterLfoRate = 2;
int filterLfoRetrig = 0;
int filterLFOMidiClkSync = 0;
String filterLFOTimeDivStr = "";//For display
float filterLfoAmt = 0;
int filterLfoWaveform = WAVEFORM_SINE;

float filterAttack = 100;
float filterDecay = 350;
float filterSustain = 0.7;
float filterRelease = 300;

float ampAttack = 10;
float ampDecay = 35;
float ampSustain = 1;
float ampRelease = 300;

float fxAmt = 1;
float fxMix = 0;

//For T4.1, T3.6 isn't fast enough to read MUX
boolean pickUp = true;
int filterfreqPrevValue = 63;
