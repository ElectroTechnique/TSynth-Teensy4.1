#include <Arduino.h>
#include "VoiceGroup.h"
#include "Constants.h"

#define VG_FOR_EACH_OSC(CMD) VG_FOR_EACH_VOICE(voices[i]->patch().CMD)
#define VG_FOR_EACH_VOICE(CMD)                  \
    for (uint8_t i = 0; i < voices.size(); i++) \
    {                                           \
        CMD;                                    \
    }

VoiceGroup::VoiceGroup(PatchShared &shared_):
        patchName(""),
        patchIndex(0),
        shared(shared_),
        midiClockSignal(false),
        filterLfoMidiClockSync(false),
        pitchLFOMidiClockSync(false),
        unisonNotesOn(0),
        voicesOn(0),
        monophonic(0),
        waveformA(WAVEFORM_SQUARE),
        waveformB(WAVEFORM_SQUARE),
        pitchEnvelope(0.0),
        pwmSource(PWMSOURCELFO),
        pwmAmtA(1.0),
        pwmAmtB(1.0),
        pwA(0.0),
        pwB(0.0),
        pwmRate(0.5),
        oscFX(0),
        oscLevelA(1.0),
        oscLevelB(1.0),
        cutoff(12000.0),
        resonance(1.1),
        filterMixer(0.0),
        filterEnvelope(0.0),
        filterAttack(100.0),
        filterDecay(350.0),
        filterSustain(0.7),
        filterRelease(300.0),
        ampAttack(10.0),
        ampDecay(35.0),
        ampSustain(1.0),
        ampRelease(300.0),
        filterLfoRetrig(false),
        filterLfoRate(2.0),
        filterLfoAmt(0.0),
        filterLfoWaveform(WAVEFORM_SINE),
        pinkLevel(0),
        whiteLevel(0),
        pitchLfoWaveform(WAVEFORM_SINE),
        pitchLfoRetrig(false),
        pitchLfoAmount(0),
        pitchLfoRate(4.0),
        modWhAmount(0.0),
        effectAmount(1.0),
        effectMix(0.0)
{
    _params.keytrackingAmount = 0.5; //Half - MIDI CC & settings option
    _params.mixerLevel = 0.0;
    _params.prevNote = 48;
    _params.glideSpeed = 0.0;
    _params.unisonMode = 0;
    _params.chordDetune = 0;
    _params.detune = 0;
    _params.oscPitchA = 0;
    _params.oscPitchB = 12;

    shared.noiseMixer.gain(0, 0);
    shared.noiseMixer.gain(1, 0);

    shared.pitchLfo.begin(WAVEFORM_SINE);
    shared.pwmLfoA.amplitude(ONE);
    shared.pwmLfoA.begin(PWMWAVEFORM);
    shared.pwmLfoB.amplitude(ONE);
    shared.pwmLfoB.begin(PWMWAVEFORM);
    shared.pwmLfoB.phase(10.0f); //Off set phase of second osc

    setEffectAmount(effectAmount);
    setEffectMix(effectMix);
}


void VoiceGroup::setWaveformA(uint32_t waveform)
{
    if (waveformA == waveform)
        return;
    waveformA = waveform;

    int temp = waveform;
    if (waveform == WAVEFORM_PARABOLIC)
    {
        VG_FOR_EACH_OSC(waveformMod_a.arbitraryWaveform(PARABOLIC_WAVE, AWFREQ));
        temp = WAVEFORM_ARBITRARY;
    }
    if (waveform == WAVEFORM_HARMONIC)
    {
        VG_FOR_EACH_OSC(waveformMod_a.arbitraryWaveform(HARMONIC_WAVE, AWFREQ));
        temp = WAVEFORM_ARBITRARY;
    }

    VG_FOR_EACH_OSC(waveformMod_a.begin(temp))
}

void VoiceGroup::setWaveformB(uint32_t waveform)
{
    if (waveformB == waveform)
        return;
    waveformB = waveform;

    int temp = waveform;
    if (waveform == WAVEFORM_PARABOLIC)
    {
        VG_FOR_EACH_OSC(waveformMod_b.arbitraryWaveform(PARABOLIC_WAVE, AWFREQ));
        temp = WAVEFORM_ARBITRARY;
    }
    if (waveform == WAVEFORM_HARMONIC)
    {
        VG_FOR_EACH_OSC(waveformMod_b.arbitraryWaveform(PPG_WAVE, AWFREQ));
        temp = WAVEFORM_ARBITRARY;
    }

    VG_FOR_EACH_OSC(waveformMod_b.begin(temp))
}

void VoiceGroup::setPwmRate(float value)
{
    pwmRate = value;

    shared.pwmLfoA.frequency(pwmRate);
    shared.pwmLfoB.frequency(pwmRate);

    if (pwmRate == PWMRATE_PW_MODE)
    {
        //Set to fixed PW mode
        this->setPwmMixerALFO(0); //LFO Source off
        this->setPwmMixerBLFO(0);
        this->setPwmMixerAFEnv(0); //Filter Env Source off
        this->setPwmMixerBFEnv(0);
        this->setPwmMixerAPW(1); //Manually adjustable pulse width on
        this->setPwmMixerBPW(1);
    }
    else if (pwmRate == PWMRATE_SOURCE_FILTER_ENV)
    {
        //Set to Filter Env Mod source
        this->setPWMSource(PWMSOURCEFENV);
        this->setPwmMixerAFEnv(this->getPwmAmtA());
        this->setPwmMixerBFEnv(this->getPwmAmtB());
        this->setPwmMixerAPW(0);
        this->setPwmMixerBPW(0);
    }
    else
    {
        this->setPWMSource(PWMSOURCELFO);
        this->setPwmMixerAPW(0);
        this->setPwmMixerBPW(0);
    }
}

void VoiceGroup::setPitchEnvelope(float value)
{
    pitchEnvelope = value;
    VG_FOR_EACH_OSC(oscModMixer_a.gain(1, value))
    VG_FOR_EACH_OSC(oscModMixer_b.gain(1, value))
}

void VoiceGroup::setPwmMixerALFO(float value)
{
    VG_FOR_EACH_OSC(pwMixer_a.gain(0, value));
}

void VoiceGroup::setPwmMixerBLFO(float value)
{
    VG_FOR_EACH_OSC(pwMixer_b.gain(0, value));
}

void VoiceGroup::setPwmMixerAPW(float value)
{
    VG_FOR_EACH_OSC(pwMixer_a.gain(1, value));
}

void VoiceGroup::setPwmMixerBPW(float value)
{
    VG_FOR_EACH_OSC(pwMixer_b.gain(1, value));
}

void VoiceGroup::setPwmMixerAFEnv(float value)
{
    VG_FOR_EACH_OSC(pwMixer_a.gain(2, value));
}

void VoiceGroup::setPwmMixerBFEnv(float value)
{
    VG_FOR_EACH_OSC(pwMixer_b.gain(2, value));
}

void VoiceGroup::overridePwmAmount(float value)
{
    pwmAmtA = value;
    pwmAmtB = value;
    pwA = 0;
    pwB = 0;
    this->setPwmMixerALFO(value);
    this->setPwmMixerBLFO(value);
}

void VoiceGroup::setPWA(float valuePwA, float valuePwmAmtA)
{
    pwA = valuePwA;
    pwmAmtA = valuePwmAmtA;
    if (pwmRate == PWMRATE_PW_MODE)
    {
        //fixed PW is enabled
        this->setPwmMixerALFO(0);
        this->setPwmMixerBLFO(0);
        this->setPwmMixerAFEnv(0);
        this->setPwmMixerBFEnv(0);
        this->setPwmMixerAPW(1);
        this->setPwmMixerBPW(1);
    }
    else
    {
        this->setPwmMixerAPW(0);
        this->setPwmMixerBPW(0);
        if (pwmSource == PWMSOURCELFO)
        {
            //PW alters PWM LFO amount for waveform A
            this->setPwmMixerALFO(pwmAmtA);
        }
        else
        {
            //PW alters PWM Filter Env amount for waveform A
            this->setPwmMixerAFEnv(pwmAmtA);
        }
    }

    //Prevent silence when pw = +/-1.0 on pulse
    float pwA_Adj = pwA;
    if (pwA > 0.98)
        pwA_Adj = 0.98f;
    if (pwA < -0.98)
        pwA_Adj = -0.98f;
    shared.pwa.amplitude(pwA_Adj);
}

void VoiceGroup::setPWB(float valuePwA, float valuePwmAmtA)
{
    pwB = valuePwA;
    pwmAmtB = valuePwmAmtA;
    if (pwmRate == PWMRATE_PW_MODE)
    {
        //fixed PW is enabled
        this->setPwmMixerALFO(0);
        this->setPwmMixerBLFO(0);
        this->setPwmMixerAFEnv(0);
        this->setPwmMixerBFEnv(0);
        this->setPwmMixerAPW(1);
        this->setPwmMixerBPW(1);
    }
    else
    {
        this->setPwmMixerAPW(0);
        this->setPwmMixerBPW(0);
        if (pwmSource == PWMSOURCELFO)
        {
            //PW alters PWM LFO amount for waveform B
            this->setPwmMixerBLFO(pwmAmtB);
        }
        else
        {
            //PW alters PWM Filter Env amount for waveform B
            this->setPwmMixerBFEnv(pwmAmtB);
        }
    }

    //Prevent silence when pw = +/-1.0 on pulse
    float pwB_Adj = pwB;
    if (pwB > 0.98)
        pwB_Adj = 0.98f;
    if (pwB < -0.98)
        pwB_Adj = -0.98f;
    shared.pwb.amplitude(pwB_Adj);
}

void VoiceGroup::setPWMSource(uint8_t value)
{
    pwmSource = value;
    if (value == PWMSOURCELFO)
    {
        //Set filter mod to zero
        this->setPwmMixerAFEnv(0);
        this->setPwmMixerBFEnv(0);

        //Set LFO mod
        if (pwmRate > PWMRATE_SOURCE_FILTER_ENV)
        {
            this->setPwmMixerALFO(pwmAmtA); //Set LFO mod
            this->setPwmMixerBLFO(pwmAmtB); //Set LFO mod
        }
    }
    else
    {
        this->setPwmMixerALFO(0); //Set LFO mod to zero
        this->setPwmMixerBLFO(0); //Set LFO mod to zero
        if (pwmRate > PWMRATE_SOURCE_FILTER_ENV)
        {
            this->setPwmMixerAFEnv(pwmAmtA); //Set filter mod
            this->setPwmMixerBFEnv(pwmAmtB); //Set filter mod
        }
    }
}

void VoiceGroup::setWaveformMixerLevel(int channel, float level)
{
    VG_FOR_EACH_OSC(waveformMixer_.gain(channel, level))
}

void VoiceGroup::setOscModMixerA(int channel, float level)
{
    VG_FOR_EACH_OSC(oscModMixer_a.gain(channel, level))
}

void VoiceGroup::setOscModMixerB(int channel, float level)
{
    VG_FOR_EACH_OSC(oscModMixer_b.gain(channel, level))
}

void VoiceGroup::setOscFXCombineMode(AudioEffectDigitalCombine::combineMode mode)
{
    VG_FOR_EACH_OSC(oscFX_.setCombineMode(mode))
}

void VoiceGroup::setOscLevelA(float value)
{
    oscLevelA = value;

    switch (oscFX)
    {
    case 1:                                                       //XOR
        setWaveformMixerLevel(0, oscLevelA);                      //Osc 1 (A)
        setWaveformMixerLevel(3, (oscLevelA + oscLevelB) / 2.0f); //oscFX XOR level
        break;
    case 2: //XMod
        //osc A sounds with increasing osc B mod
        if (oscLevelA == 1.0f && oscLevelB <= 1.0f)
        {
            setOscModMixerA(3, 1 - oscLevelB); //Feed from Osc 2 (B)
            setWaveformMixerLevel(0, ONE);     //Osc 1 (A)
            setWaveformMixerLevel(1, 0);       //Osc 2 (B)
        }
        break;
    case 0:                                  //None
        setOscModMixerA(3, 0);               //Feed from Osc 2 (B)
        setWaveformMixerLevel(0, oscLevelA); //Osc 1 (A)
        setWaveformMixerLevel(3, 0);         //XOR
        break;
    }
}

void VoiceGroup::setOscLevelB(float value)
{
    oscLevelB = value;

    switch (oscFX)
    {
    case 1:                                                       //XOR
        setWaveformMixerLevel(1, oscLevelB);                      //Osc 2 (B)
        setWaveformMixerLevel(3, (oscLevelA + oscLevelB) / 2.0f); //oscFX XOR level
        break;
    case 2: //XMod
        //osc B sounds with increasing osc A mod
        if (oscLevelB == 1.0f && oscLevelA < 1.0f)
        {
            setOscModMixerB(3, 1 - oscLevelA); //Feed from Osc 1 (A)
            setWaveformMixerLevel(0, 0);       //Osc 1 (A)
            setWaveformMixerLevel(1, ONE);     //Osc 2 (B)
        }
        break;
    case 0:                                  //None
        setOscModMixerB(3, 0);               //Feed from Osc 1 (A)
        setWaveformMixerLevel(1, oscLevelB); //Osc 2 (B)
        setWaveformMixerLevel(3, 0);         //XOR
        break;
    }
}

void VoiceGroup::setOscFX(uint8_t value)
{
    oscFX = value;

    if (oscFX == 2)
    {
        if (oscLevelA == 1.0f && oscLevelB <= 1.0f)
        {
            setOscModMixerA(3, 1 - oscLevelB); //Feed from Osc 2 (B)
            setWaveformMixerLevel(0, ONE);     //Osc 1 (A)
            setWaveformMixerLevel(1, 0);       //Osc 2 (B)
        }
        else
        {
            setOscModMixerB(3, 1 - oscLevelA); //Feed from Osc 1 (A)
            setWaveformMixerLevel(0, 0);       //Osc 1 (A)
            setWaveformMixerLevel(1, ONE);     //Osc 2 (B)
        }
        //Set XOR type off
        setOscFXCombineMode(AudioEffectDigitalCombine::OFF);
        setWaveformMixerLevel(3, 0); //XOR
    }
    else if (oscFX == 1)
    {
        setOscModMixerA(3, 0); //XMod off
        setOscModMixerB(3, 0); //XMod off
        //XOR 'Ring Mod' type effect
        setOscFXCombineMode(AudioEffectDigitalCombine::XOR);
        setWaveformMixerLevel(3, (oscLevelA + oscLevelB) / 2.0f); //XOR on
    }
    else
    {
        setOscModMixerA(3, 0);                               //XMod off
        setOscModMixerB(3, 0);                               //XMod off
        setOscFXCombineMode(AudioEffectDigitalCombine::OFF); //Set XOR type off
        setWaveformMixerLevel(0, oscLevelA);                 //Osc 1 (A)
        setWaveformMixerLevel(1, oscLevelB);                 //Osc 2 (B)
        setWaveformMixerLevel(3, 0);                         //XOR off
    }
}

void VoiceGroup::setCutoff(float value)
{
    this->cutoff = value;

    VG_FOR_EACH_OSC(filter_.frequency(value))

    float filterOctave = 0.0;
    //Altering filterOctave to give more cutoff width for deeper bass, but sharper cuttoff at higher frequncies
    if (value <= 2000)
    {
        filterOctave = 4.0f + ((2000.0f - value) / 710.0f); //More bass
    }
    else if (value > 2000 && value <= 3500)
    {
        filterOctave = 3.0f + ((3500.0f - value) / 1500.0f); //Sharper cutoff
    }
    else if (value > 3500 && value <= 7000)
    {
        filterOctave = 2.0f + ((7000.0f - value) / 4000.0f); //Sharper cutoff
    }
    else
    {
        filterOctave = 1.0f + ((12000.0f - value) / 5100.0f); //Sharper cutoff
    }

    VG_FOR_EACH_OSC(filter_.octaveControl(filterOctave))
}

void VoiceGroup::setResonance(float value)
{
    resonance = value;
    VG_FOR_EACH_OSC(filter_.resonance(value))
}

void VoiceGroup::setFilterMixer(float value)
{
    filterMixer = value;

    float LP = 1.0f;
    float BP = 0;
    float HP = 0;

    if (value == BANDPASS)
    {
        //BP mode
        LP = 0;
        BP = 1.0f;
        HP = 0;
    }
    else
    {
        //LP-HP mix mode - a notch filter
        LP = 1.0f - value;
        BP = 0;
        HP = value;
    }

    VG_FOR_EACH_VOICE(
        voices[i]->patch().filterMixer_.gain(0, LP);
        voices[i]->patch().filterMixer_.gain(1, BP);
        voices[i]->patch().filterMixer_.gain(2, HP);)
}

void VoiceGroup::setFilterModMixer(int channel, float level)
{
    VG_FOR_EACH_OSC(filterModMixer_.gain(channel, level))
}

void VoiceGroup::setFilterEnvelope(float value)
{
    filterEnvelope = value;
    this->setFilterModMixer(0, filterEnvelope);
}

void VoiceGroup::setFilterAttack(float value)
{
    filterAttack = value;
    VG_FOR_EACH_OSC(filterEnvelope_.attack(value))
}

void VoiceGroup::setFilterDecay(float value)
{
    filterDecay = value;
    VG_FOR_EACH_OSC(filterEnvelope_.decay(value))
}

void VoiceGroup::setFilterSustain(float value)
{
    filterSustain = value;
    VG_FOR_EACH_OSC(filterEnvelope_.sustain(value))
}

void VoiceGroup::setFilterRelease(float value)
{
    filterRelease = value;
    VG_FOR_EACH_OSC(filterEnvelope_.release(value))
}

void VoiceGroup::setAmpAttack(float value)
{
    ampAttack = value;
    VG_FOR_EACH_OSC(ampEnvelope_.attack(value))
}

void VoiceGroup::setAmpDecay(float value)
{
    ampDecay = value;
    VG_FOR_EACH_OSC(ampEnvelope_.decay(value))
}

void VoiceGroup::setAmpSustain(float value)
{
    ampSustain = value;
    VG_FOR_EACH_OSC(ampEnvelope_.sustain(value))
}

void VoiceGroup::setAmpRelease(float value)
{
    ampRelease = value;
    VG_FOR_EACH_OSC(ampEnvelope_.release(value))
}

void VoiceGroup::setKeytracking(float value)
{
    // TODO: Move this out of params to avoid VoiceGroup::setting it directly without updating the mixer.
    _params.keytrackingAmount = value;
    setFilterModMixer(2, value);
}

void VoiceGroup::setFilterLfoRetrig(bool value)
{
    filterLfoRetrig = value;
    shared.filterLfo.sync();
}

void VoiceGroup::setFilterLfoRate(float value)
{
    filterLfoRate = value;
    shared.filterLfo.frequency(value);
}

void VoiceGroup::setFilterLfoAmt(float value)
{
    filterLfoAmt = value;
    shared.filterLfo.amplitude(value);
}

void VoiceGroup::setFilterLfoWaveform(int waveform)
{
    if (filterLfoWaveform == waveform)
        return;
    filterLfoWaveform = waveform;
    shared.filterLfo.begin(filterLfoWaveform);
}

void VoiceGroup::setPinkNoiseLevel(float value)
{
    pinkLevel = value;
    float gain;
    if (_params.unisonMode == 0)
        gain = 1.0;
    else
        gain = UNISONNOISEMIXERLEVEL;
    shared.noiseMixer.gain(0, pinkLevel * gain);
}

void VoiceGroup::setWhiteNoiseLevel(float value)
{
    whiteLevel = value;
    float gain;
    if (_params.unisonMode == 0)
        gain = 1.0;
    else
        gain = UNISONNOISEMIXERLEVEL;
    shared.noiseMixer.gain(1, whiteLevel * gain);
}

void VoiceGroup::setPitchLfoRetrig(bool value)
{
    pitchLfoRetrig = value;
    shared.pitchLfo.sync();
}

void VoiceGroup::setPitchLfoWaveform(uint32_t waveform)
{
    if (pitchLfoWaveform == waveform)
        return;
    pitchLfoWaveform = waveform;
    shared.pitchLfo.begin(waveform);
}

void VoiceGroup::setPitchLfoAmount(float value)
{
    pitchLfoAmount = value;
    shared.pitchLfo.amplitude(value + modWhAmount);
}

void VoiceGroup::setPitchLfoRate(float value)
{
    pitchLfoRate = value;
    shared.pitchLfo.frequency(value);
}

void VoiceGroup::setModWhAmount(float value)
{
    modWhAmount = value;
    shared.pitchLfo.amplitude(value + pitchLfoAmount);
}

void VoiceGroup::setEffectAmount(float value)
{
    effectAmount = value;
    shared.ensemble.lfoRate(effectAmount);
}

void VoiceGroup::setEffectMix(float value)
{
    effectMix = value;
    shared.effectMixerL.gain(0, 1.0f - effectMix); //Dry
    shared.effectMixerL.gain(1, effectMix);        //Wet
    shared.effectMixerR.gain(0, 1.0f - effectMix); //Dry
    shared.effectMixerR.gain(1, effectMix);        //Wet
}

void VoiceGroup::setMonophonic(uint8_t mode)
{
    if (mode != this->monophonic)
    {
        noteStack.clear();
    }
    this->monophonic = mode;
}

void VoiceGroup::setUnisonMode(uint8_t mode)
{
    if (mode == 0)
        allNotesOff();

    this->_params.unisonMode = mode;
    this->unisonNotesOn = 0;

    // Update noise gain
    setPinkNoiseLevel(pinkLevel);
    setWhiteNoiseLevel(whiteLevel);
}

Voice* VoiceGroup::pop()
{
    if (voices.size() == 0)
    {
        return nullptr;
    }
    Voice *result = voices.back();
    voices.pop_back();
    return result;
}

void VoiceGroup::add(Voice *v)
{
    Mixer *m = v->patch().connectTo(shared, voices.size());
    v->setMixer(m);
    voices.push_back(v);
}

void VoiceGroup::merge(VoiceGroup other)
{
}

void VoiceGroup::noteOn(uint8_t note, uint8_t velocity)
{
    if (this->monophonic)
    {
        handleMonophonicNoteOn(note, velocity);
        return;
    }
    noteOn(note, velocity, false);
}

void VoiceGroup::allNotesOn(uint8_t note, int velocity, uint8_t id)
{
    for (uint8_t i = 0; i < voices.size(); i++)
    {
        voices[i]->noteOn(note, velocity, this->_params, unisonNotesOn, id);
    }
}

void VoiceGroup::allNotesOff()
{
    this->unisonNotesOn = 0;
    for (uint8_t i = 0; i < voices.size(); i++)
    {
        voices[i]->noteOff();
    }
}

void VoiceGroup::updateVoices()
{
    for (uint8_t i = 0; i < voices.size(); i++)
    {
        voices[i]->updateVoice(this->_params, unisonNotesOn);
    }
}

void VoiceGroup::noteOff(uint8_t note)
{
    if (this->monophonic)
    {
        handleMonophonicNoteOff(note);
        return;
    }

    uint8_t num = 0;
    switch (this->_params.unisonMode)
    {
    case 0:
        num = noteOff(note, false); // changed for experiment
        break;
    default:
        num = noteOff(note, true);
        break;
    }

    // Decrement unison note count if noteOff modified a voice.
    if (this->unisonNotesOn > 0 && num > 0)
        this->unisonNotesOn--;
}

void VoiceGroup::pitchBend(float amount)
{
    shared.pitchBend.amplitude(amount);
}

void VoiceGroup::midiClockStart()
{
    midiClockSignal = true;
    shared.filterLfo.sync();
}

void VoiceGroup::midiClockStop()
{
    midiClockSignal = false;
}

void VoiceGroup::midiClock(float frequency)
{
    midiClockSignal = true;

    if (filterLfoMidiClockSync)
    {
        shared.filterLfo.frequency(frequency);
    }

    if (pitchLFOMidiClockSync)
    {
        shared.pitchLfo.frequency(frequency);
    }
}

void VoiceGroup::closeEnvelopes()
{
    VG_FOR_EACH_VOICE(
        voices[i]->patch().filterEnvelope_.close();
        voices[i]->patch().ampEnvelope_.close();)
}

void VoiceGroup::handleMonophonicNoteOn(uint8_t note, uint8_t velocity)
{
    noteStack.push(note, velocity);
    if (monophonic)
    {
        MonoNoteHistory::Element nextNote{note, velocity};
        // If there is more than 1 note check for retrigger.
        if (noteStack.size() > 1)
        {
            switch (monophonic)
            {
            case MONOPHONIC_FIRST:
                // Exit since there is already a note playing.
                return;
                break;
            case MONOPHONIC_LEGATO:
            case MONOPHONIC_LAST:
                // Always play the last note.
                break;
            case MONOPHONIC_HIGHEST:
            {
                // Play if higher than active note.
                if (note < monoNote)
                {
                    return;
                }
                break;
            }
            case MONOPHONIC_LOWEST:
            {
                if (note > monoNote)
                {
                    return;
                }
                break;
            }
            }
        }

        // Make sure any active note is turned off.
        for (uint8_t i = 0; i < voices.size(); i++)
        {
            voices[i]->noteOff();
        }
        this->monoNote = nextNote.note;
        noteOn(nextNote.note, nextNote.velocity, false);
    }
    else
    {
        noteOn(note, velocity, false);
    }
}

void VoiceGroup::handleMonophonicNoteOff(uint8_t note)
{
    if (!this->monophonic)
        return;

    // Remove turned-off note
    noteStack.erase(note);
    bool activeNoteTurnedOff = 0 != noteOff(note, true);

    // Active note still active, or no more notes.
    if (!activeNoteTurnedOff || noteStack.size() == 0)
        return;

    MonoNoteHistory::Element nextNote;
    switch (this->monophonic)
    {
    case MONOPHONIC_LAST:
        nextNote = noteStack.getLast();
        break;
    case MONOPHONIC_FIRST:
        nextNote = noteStack.getFirst();
        break;
    case MONOPHONIC_LOWEST:
        nextNote = noteStack.getLowest();
        break;
    case MONOPHONIC_HIGHEST:
        nextNote = noteStack.getHighest();
        break;
    case MONOPHONIC_LEGATO:
    default:
        // No retriggering.
        return;
        break;
    }
    this->monoNote = nextNote.note;
    noteOn(nextNote.note, nextNote.velocity, true);
    return;
}

// Get the oldest free voice, of none free get the oldest active voice.
Voice* VoiceGroup::getVoice()
{
    Voice *result = nullptr;

    for (uint8_t i = 0; i < voices.size(); i++)
    {
        if (result == nullptr || !voices[i]->on() || result->on())
        {
            if (result == nullptr || voices[i]->timeOn() < result->timeOn())
            {
                result = voices[i];
            }
        }
    }
    return result;
}

// Turn off one or more notes, return the number of notes turned off.
int VoiceGroup::noteOff(uint8_t note, bool all)
{
    int num = 0;
    for (uint8_t i = 0; i < voices.size(); i++)
    {
        if (voices[i]->note() == note && voices[i]->on() == true)
        {
            num++;
            voices[i]->noteOff();
            this->voicesOn--;
            if (!all)
            {
                return 1;
            }
        }
    }
    return num;
}

void VoiceGroup::noteOn(uint8_t note, uint8_t velocity, bool monoRetrigger)
{

    if (filterLfoRetrig)
    {
        shared.filterLfo.sync();
    }

    switch (this->_params.unisonMode)
    {
    case 0:
    {
        this->_params.mixerLevel = VOICEMIXERLEVEL;
        Voice *v = this->getVoice();
        if (!v->on())
            this->voicesOn++;
        v->noteOn(note, velocity, this->_params, unisonNotesOn, 0);
        break;
    }
    case 1:
    {
        if (!this->monophonic)
            this->unisonNotesOn++;

        this->_params.mixerLevel = UNISONVOICEMIXERLEVEL;

        uint8_t maxUnison = voices.size() / MINUNISONVOICES;
        uint8_t tally[maxUnison] = {};
        uint8_t oldestVoiceIndex = 0;

        // Figure out which note id to use.
        for (uint8_t i = 0; i < this->voices.size(); i++)
        {
            if (voices[i]->on())
            {
                tally[voices[i]->noteId()]++;
                if (voices[i]->timeOn() < voices[oldestVoiceIndex]->timeOn())
                {
                    oldestVoiceIndex = i;
                }
            }
        }

        uint8_t id = 0;
        for (uint8_t i = 0; i < maxUnison; i++)
        {
            if (tally[i] == 0)
            {
                id = i;
                break;
            }
        }

        // Replace oldest note if too many are playing.
        if (this->unisonNotesOn > maxUnison)
        {
            id = voices[oldestVoiceIndex]->noteId();
            noteOff(voices[oldestVoiceIndex]->note());
        }

        // Fill gaps if there are any.
        if (this->unisonNotesOn != 1 && this->voicesOn != this->voices.size())
        {
            for (uint8_t i = 0; i < this->voices.size(); i++)
            {
                if (!voices[i]->on())
                {
                    voices[i]->noteOn(note, velocity, this->_params, unisonNotesOn, id);
                    this->voicesOn++;
                }
            }
            return;
        }

        // Start all voices or...
        // Steal voices until each has the right amount.
        uint8_t max = this->voices.size() / unisonNotesOn;
        for (uint8_t i = 0; i < voices.size(); i++)
        {
            if (!voices[i]->on() || tally[voices[i]->noteId()] > max)
            {
                // underflow here when starting first unison note, but it still works.
                tally[voices[i]->noteId()]--;
                voices[i]->noteOn(note, velocity, this->_params, unisonNotesOn, id);
            }
        }
        voicesOn = voices.size();

        break;
    }
    case 2:
        this->_params.mixerLevel = UNISONVOICEMIXERLEVEL;
        allNotesOn(note, velocity, 0);
        break;
    }
    this->_params.prevNote = note;
}

