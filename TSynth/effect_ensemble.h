/* Audio Library for Teensy 3.X
   Portions Copyright (c) 2019, Alexander Davis info@matrixwide.com
   Portions Copyright (c) 2021, Vince R. Pearson

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice, development funding notice, and this permission
   notice shall be included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
*/

// V. Pearson: Implemented delay buffer interpolation with floating point fractional indices and using a larger table
// to reduce audible delay sweep quantization noise.
// The fractional indices are stored in flash memory instead of being calculated at runtime and stored in RAM.
// #define LARGE_ENSEMBLE_LFO_TABLE uses a larger lfo table with only delay buffer interpolation.
// This is more precise and is slightly faster but takes more flash memory.
// With it undefined, lfo table and delay buffer interpolation are implemented with some extra processing and a smaller lfo table. 

#define LARGE_ENSEMBLE_LFO_TABLE

#ifndef effect_ensemble_h_
#define effect_ensemble_h_

#include <Arduino.h>
#include "AudioStream.h"
#define ENSEMBLE_BUFFER_SIZE 1024
// to put a channel 90 degrees out of LFO phase for stereo spread
#define PHASE_90 367

// LFO wavetable parameters
#ifdef LARGE_ENSEMBLE_LFO_TABLE
  #define LFO_SAMPLES (1470*4) // number of samples in tablle
  #define LFO_SIZE (1470*4) // number of effective values (no interpolation so same as table size).
  #define COUNTS_PER_LFO (200/4) // Adjust for larger table so the rate is the same.
#else
  #define LFO_SAMPLES 1470 // number of samples in table
  #define LFO_SIZE (1470*4) // number of effective values interpolated from table
  #define COUNTS_PER_LFO (200/4) // Adjust for larger effective table so the rate is the same.
#endif

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

    // LFO wavetable
    const static float PROGMEM lfoTable[];

    // input index
    int16_t inIndex;
    // output indexes
    // default to csenter of buffer
    int16_t outIndex1;
    int16_t outIndex2;
    int16_t outIndex3;
    int16_t outIndex4;
    int16_t outIndex5;
    int16_t outIndex6;
    // lfo index
    // seprated by thirds to approximate 120 degree phase relationship
    int16_t lfoIndex1;
    int16_t lfoIndex2;
    int16_t lfoIndex3;
    int16_t lfoIndex4;
    int16_t lfoIndex5;
    int16_t lfoIndex6;
    // lfo rate counter
    int16_t lfoCount;
    // output index offset
    float offset1;
    float offset2;
    float offset3;
    float offset4;
    float offset5;
    float offset6;
    //Default countsPerLfo
    int countsPerLfo = COUNTS_PER_LFO;
    int16_t interpBuffer(float findex);
    
    #ifndef LARGE_ENSEMBLE_LFO_TABLE
    float lfoLookup(int16_t lfoIndex);
    #endif

};


#endif
