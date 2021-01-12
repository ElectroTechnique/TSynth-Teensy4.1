/* Audio Library for Teensy 3.X
 * Copyright (c) 2019, Alexander Davis info@matrixwide.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef effect_ensemble_h_
#define effect_ensemble_h_

#include <Arduino.h>
#include "AudioStream.h"
#define ENSEMBLE_BUFFER_SIZE 1024
// to put a channel 90 degrees out of LFO phase for stereo spread
#define PHASE_90 367
#define LFO_SIZE 1470
#define COUNTS_PER_LFO 200

// LFO wavetable parameters
#define LFO_SAMPLES 1470
#define LFO_RANGE 110

class AudioEffectEnsemble : public AudioStream
{
public:
    AudioEffectEnsemble(void);
	virtual void update(void);
	void lfoRate(float rate);
	
private:
	audio_block_t *inputQueueArray[1];
    // buffers
    int16_t delayBuffer[ENSEMBLE_BUFFER_SIZE];
    
    // LFO wavetable until I resolve progmem issues
    int16_t lfoTable[LFO_SAMPLES];
    
    // input index
    int16_t inIndex;
    // output indexes
    // default to csenter of buffer
    int16_t outIndex1;
    int16_t outIndex2;
    int16_t outIndex3;
    // lfo index
    // seprated by thirds to approximate 120 degree phase relationship
    int16_t lfoIndex1;
    int16_t lfoIndex2;
    int16_t lfoIndex3;

    // lfo rate counter
    int16_t lfoCount;
    // output index offset
    int16_t offset1;
    int16_t offset2;
    int16_t offset3;
    int16_t offsetIndex1;
    int16_t offsetIndex2;
    int16_t offsetIndex3;
    int16_t offsetIndex1B;
    int16_t offsetIndex2B;
    int16_t offsetIndex3B;
    
    int16_t iC;
	
	//Default countsPerLfo
    int countsPerLfo = COUNTS_PER_LFO;
};
#endif
