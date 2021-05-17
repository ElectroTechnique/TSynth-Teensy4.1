//Values below are just for initialising and will be changed when synth is initialised to current panel controls & EEPROM settings

// Globals used for OLED Settings
byte midiChannel = MIDI_CHANNEL_OMNI;//(EEPROM)
byte midiOutCh = 0;//(EEPROM)
midi::Thru::Mode MIDIThru = midi::Thru::Off;//(EEPROM)
String patchName = INITPATCHNAME;
boolean encCW = true;//This is to set the encoder to increment when turned CW - Settings Option
			   	 			 
float lfoSyncFreq = 1.0f;
float midiClkTimeInterval = 0.0f;
float lfoTempoValue = 1.0f;
int pitchBendRange = 12;
float modWheelDepth = 0.2f;
String oscLFOTimeDivStr = "";//For display

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
float filterfreqPrevValue = 0;//Need to set these when patch loaded
float filterMixPrevValue = 0.0f;//Need to set these when patch loaded
float resonancePrevValue = 0.0f;//Need to set these when patch loaded
float oscLfoAmtPrevValue = 0.0f;//Need to set these when patch loaded
float oscLfoRatePrevValue = 0.0f;//Need to set these when patch loaded
float filterLfoRatePrevValue = 0.0f;//Need to set these when patch loaded
float filterLfoAmtPrevValue = 0.0f;//Need to set these when patch loaded
float fxAmtPrevValue = 0.0f;//Need to set these when patch loaded
float fxMixPrevValue = 0.0f;//Need to set these when patch loaded

// Exponential envelopes (attack shape) & glide
int8_t envTypeAmp=-128; // Linear
int8_t envTypeFilt=-128; // Linear
int8_t glideShape=1; // Exp
