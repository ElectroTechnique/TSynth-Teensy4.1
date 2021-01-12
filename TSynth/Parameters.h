//Values below are just for initialising and will be changed when synth is initialised to current panel controls & EEPROM settings
byte midiChannel = MIDI_CHANNEL_OMNI;//(EEPROM)
byte midiOutCh = 0;//(EEPROM)
int MIDIThru = midi::Thru::Off;//(EEPROM)
String patchName = INITPATCHNAME;
boolean encCW = true;//This is to set the encoder to increment when turned CW - Settings Option
float oscALevel = 1.0f;
float oscBLevel = 1.0f;
float noiseLevel = 0.0f;
int unison = 0;
int oscFX = 0;
float detune = 0.98f;
uint32_t chordDetune = 0;
float lfoSyncFreq = 1.0f;
float midiClkTimeInterval = 0.0f;
float lfoTempoValue = 1.0f;
int pitchBendRange = 12;
float modWheelDepth = 0.2f;
float modWhAmt = 0.0f;
float keytrackingAmount = 0.5f;//Half - MIDI CC & settings option
float glideSpeed = 0.0f;
int oscPitchA = 0;
int oscPitchB = 12;
float pitchEnv = 0.0f;
int oscWaveformA = WAVEFORM_SQUARE;
int oscWaveformB = WAVEFORM_SQUARE;
float pwmAmtA = 1.0f;
float pwmAmtB = 1.0f;
float pwmRate = 0.5f;
float pwA = 0.0f;
float pwB = 0.0f;
int pwmSource = PWMSOURCELFO;

float filterRes = 1.1f;
float filterFreq = 12000.0f;
float filterOctave = 1.0f;
float filterMix = 0.0f;
int filterMixStr = 0;//For display
float filterEnv = 0.0f;
float oscLfoAmt = 0.0f;
float oscLfoRate = 4.0f;
int oscLFOWaveform = WAVEFORM_SINE;
int oscLfoRetrig = 0;
int oscLFOMidiClkSync = 0;//MIDI Only
String oscLFOTimeDivStr = "";//For display
float filterLfoRate = 2.0f;
int filterLfoRetrig = 0;
int filterLFOMidiClkSync = 0;
String filterLFOTimeDivStr = "";//For display
float filterLfoAmt = 0.0f;
int filterLfoWaveform = WAVEFORM_SINE;

float filterAttack = 100.0f;
float filterDecay = 350.0f;
float filterSustain = 0.7f;
float filterRelease = 300.0f;

float ampAttack = 10.0f;
float ampDecay = 35.0f;
float ampSustain = 1.0f;
float ampRelease = 300.0f;

float fxAmt = 1.0f;
float fxMix = 0.0f;

int velocitySens = 0;//Default off - settings option

boolean vuMeter = false;

//Pick-up - Experimental feature
//Control will only start changing when the Knob/MIDI control reaches the current parameter value
//Prevents jumps in value when the patch parameter and control are different values
boolean pickUp = false;//settings option (EEPROM)
boolean pickUpActive = false;
#define TOLERANCE 2 //Gives a window of when pick-up occurs, this is due to the speed of control changing and Mux reading
uint32_t filterfreqPrevValue = 0;//Need to set these when patch loaded
float filterMixPrevValue = 0.0f;//Need to set these when patch loaded
float resonancePrevValue = 0.0f;//Need to set these when patch loaded
float oscLfoAmtPrevValue = 0.0f;//Need to set these when patch loaded
float oscLfoRatePrevValue = 0.0f;//Need to set these when patch loaded
float filterLfoRatePrevValue = 0.0f;//Need to set these when patch loaded
float filterLfoAmtPrevValue = 0.0f;//Need to set these when patch loaded
float fxAmtPrevValue = 0.0f;//Need to set these when patch loaded
float fxMixPrevValue = 0.0f;//Need to set these when patch loaded
