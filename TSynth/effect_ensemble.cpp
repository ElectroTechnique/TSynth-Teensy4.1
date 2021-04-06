/* Audio Library for Teensy 3.X
   Copyright (c) 2019, Alexander Davis info@matrixwide.com

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

// My take on a string machine ensemble chorus sound like the Roland RS-202
// or Lowrey Symphonic Strings.
// It takes the approach of 6.0 Hz and 0.6 Hz sinewaves making up an LFO,
// which then modulates three delay lines 120 degrees apart in the LFO waveform.

#include <Arduino.h>
#include "effect_ensemble.h"
#include "utility/dspinst.h"
#include "arm_math.h"

AudioEffectEnsemble::AudioEffectEnsemble() : AudioStream(1, inputQueueArray){
  memset(delayBuffer, 0, sizeof(delayBuffer));
  memset(lfoTable, 0, sizeof(lfoTable));

  // input index
  inIndex = 0;
  // output indexes
  // default to center of buffer
  outIndex1 = 512;
  outIndex2 = 512;
  outIndex3 = 512;
  outIndex4 = 512;
  outIndex5 = 512;
  // lfo index
  // seprated by sixths to approximate 60 degree phase relationship
  lfoIndex1 = 0;
  lfoIndex2 = 122;
  lfoIndex3 = 245;
  lfoIndex4 = 368;
  lfoIndex5 = 490;
  // lfo rate counter
  lfoCount = 0;
  // input index offset
  offset1 = 0;
  offset2 = 0;
  offset3 = 0;
  offset4 = 0;
  offset5 = 0;
  offsetIndex1 = 0;
  offsetIndex2 = 0;
  offsetIndex3 = 0;
  offsetIndex4 = 0;
  offsetIndex5 = 0;
  offsetIndex1B = 0;
  offsetIndex2B = 0;
  offsetIndex3B = 0;
  offsetIndex4B = 0;
  offsetIndex5B = 0;
  // generate the LFO wavetable
  for (iC = 0; iC < LFO_SAMPLES; iC++){
    lfoTable[iC] = round(((sin(((2.0 * M_PI) / LFO_SAMPLES) * iC) * LFO_RANGE) / 2.0) + (((sin(((20.0 * M_PI) / LFO_SAMPLES) * iC)) * LFO_RANGE) / 3.0));
  }
  return;
}

// TODO: move this to one of the data files, use in output_adat.cpp, output_tdm.cpp, etc
static const audio_block_t zeroblock = {
  0, 0, 0, {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
#if AUDIO_BLOCK_SAMPLES > 16
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
#endif
#if AUDIO_BLOCK_SAMPLES > 32
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
#endif
#if AUDIO_BLOCK_SAMPLES > 48
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
#endif
#if AUDIO_BLOCK_SAMPLES > 64
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
#endif
#if AUDIO_BLOCK_SAMPLES > 80
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
#endif
#if AUDIO_BLOCK_SAMPLES > 96
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
#endif
#if AUDIO_BLOCK_SAMPLES > 112
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
#endif
  }
};

//Rate is in Hz
void AudioEffectEnsemble::lfoRate(float rate)
{
  //Assumes COUNTS_PER_LFO is giving 6Hz
  countsPerLfo = round((COUNTS_PER_LFO * 6) / rate);
  if (countsPerLfo < 1) {
    countsPerLfo = 1;
  }
}

void AudioEffectEnsemble::update(void)
{
  const audio_block_t *block;
  audio_block_t *outblock;
  audio_block_t *outblockB;
  uint16_t i;

  outblock = allocate();
  outblockB = allocate();
  if ((!outblock) || (!outblockB)) {
    audio_block_t *tmp = receiveReadOnly(0);
    if (tmp) release(tmp);
    return;
  }
  block = receiveReadOnly(0);
  if (!block)
    block = &zeroblock;

  // buffer the incoming block
  for (i = 0; i < AUDIO_BLOCK_SAMPLES; i++){
    // wrap the input index
    inIndex++;
    if (inIndex > (ENSEMBLE_BUFFER_SIZE - 1))
      inIndex = 0;
    delayBuffer[inIndex] = block->data[i];
  }
  // re-load the block with the delayed data
  for (i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
    // advance the wavetable indexes every counts per LFO
    // so the LFO modulates at the correct rate
    lfoCount++;
    if (lfoCount > countsPerLfo){
      // wrap the lfo index
      lfoIndex1++;
      if (lfoIndex1 > (LFO_SIZE - 1))
        lfoIndex1 = 0;
      lfoIndex2++;
      if (lfoIndex2 > (LFO_SIZE - 1))
        lfoIndex2 = 0;
      lfoIndex3++;
      if (lfoIndex3 > (LFO_SIZE - 1))
        lfoIndex3 = 0;

      // reset the counter
      lfoCount = 0;
    }

    // wrap the output index
    outIndex1++;
    if (outIndex1 > (ENSEMBLE_BUFFER_SIZE - 1))
      outIndex1 = 0;

    outIndex2++;
    if (outIndex2 > (ENSEMBLE_BUFFER_SIZE - 1))
      outIndex2 = 0;

    outIndex3++;
    if (outIndex3 > (ENSEMBLE_BUFFER_SIZE - 1))
      outIndex3 = 0;

    outIndex4++;
    if (outIndex4 > (ENSEMBLE_BUFFER_SIZE - 1))
      outIndex4 = 0;

    outIndex5++;
    if (outIndex5 > (ENSEMBLE_BUFFER_SIZE - 1))
      outIndex5 = 0;

    // get the delay from the wavetable
    offset1 = lfoTable[lfoIndex1];
    offset2 = lfoTable[lfoIndex2];
    offset3 = lfoTable[lfoIndex3];
    offset4 = lfoTable[lfoIndex4];
    offset5 = lfoTable[lfoIndex5];

    // add the delay to the buffer index to get the delay index
    offsetIndex1 = outIndex1 + offset1;
    offsetIndex2 = outIndex2 + offset2;
    offsetIndex3 = outIndex3 + offset3;
    offsetIndex4 = outIndex4 + offset4;
    offsetIndex5 = outIndex5 + offset5;
    offsetIndex1B = outIndex1 + offset1 + PHASE_90;
    offsetIndex2B = outIndex2 + offset2 + PHASE_90;
    offsetIndex3B = outIndex3 + offset3 + PHASE_90;
    offsetIndex4B = outIndex4 + offset4 + PHASE_90;
    offsetIndex5B = outIndex5 + offset5 + PHASE_90;

    // wrap the index if it goes past the end of the buffer
    if (offsetIndex1 > (ENSEMBLE_BUFFER_SIZE - 1))
      offsetIndex1 = offsetIndex1 - ENSEMBLE_BUFFER_SIZE;
    if (offsetIndex2 > (ENSEMBLE_BUFFER_SIZE - 1))
      offsetIndex2 = offsetIndex2 - ENSEMBLE_BUFFER_SIZE;
    if (offsetIndex3 > (ENSEMBLE_BUFFER_SIZE - 1))
      offsetIndex3 = offsetIndex3 - ENSEMBLE_BUFFER_SIZE;
    if (offsetIndex4 > (ENSEMBLE_BUFFER_SIZE - 1))
      offsetIndex4 = offsetIndex4 - ENSEMBLE_BUFFER_SIZE;
    if (offsetIndex5 > (ENSEMBLE_BUFFER_SIZE - 1))
      offsetIndex5 = offsetIndex5 - ENSEMBLE_BUFFER_SIZE;

    // wrap the index if it goes past the buffer the other way
    if (offsetIndex1 < 0)
      offsetIndex1 = ENSEMBLE_BUFFER_SIZE + offsetIndex1;
    if (offsetIndex2 < 0)
      offsetIndex2 = ENSEMBLE_BUFFER_SIZE + offsetIndex2;
    if (offsetIndex3 < 0)
      offsetIndex3 = ENSEMBLE_BUFFER_SIZE + offsetIndex3;

    if (offsetIndex4 < 0)
      offsetIndex4 = ENSEMBLE_BUFFER_SIZE + offsetIndex4;

    if (offsetIndex5 < 0)
      offsetIndex5 = ENSEMBLE_BUFFER_SIZE + offsetIndex5;

    // wrap the index if it goes past the end of the buffer
    if (offsetIndex1B > (ENSEMBLE_BUFFER_SIZE - 1))
      offsetIndex1B = offsetIndex1B - ENSEMBLE_BUFFER_SIZE;
    if (offsetIndex2B > (ENSEMBLE_BUFFER_SIZE - 1))
      offsetIndex2B = offsetIndex2B - ENSEMBLE_BUFFER_SIZE;
    if (offsetIndex3B > (ENSEMBLE_BUFFER_SIZE - 1))
      offsetIndex3B = offsetIndex3B - ENSEMBLE_BUFFER_SIZE;

    if (offsetIndex4B > (ENSEMBLE_BUFFER_SIZE - 1))
      offsetIndex4B = offsetIndex4B - ENSEMBLE_BUFFER_SIZE;

    if (offsetIndex5B > (ENSEMBLE_BUFFER_SIZE - 1))
      offsetIndex5B = offsetIndex5B - ENSEMBLE_BUFFER_SIZE;

    // wrap the index if it goes past the buffer the other way
    if (offsetIndex1B < 0)
      offsetIndex1B = ENSEMBLE_BUFFER_SIZE + offsetIndex1B;
    if (offsetIndex2B < 0)
      offsetIndex2B = ENSEMBLE_BUFFER_SIZE + offsetIndex2B;
    if (offsetIndex3B < 0)
      offsetIndex3B = ENSEMBLE_BUFFER_SIZE + offsetIndex3B;
    if (offsetIndex4B < 0)
      offsetIndex4B = ENSEMBLE_BUFFER_SIZE + offsetIndex4B;
    if (offsetIndex5B < 0)
      offsetIndex5B = ENSEMBLE_BUFFER_SIZE + offsetIndex5B;

    // combine delayed samples into output
    // add the delayed and scaled samples
    outblock->data[i] = int(round((delayBuffer[offsetIndex1] + delayBuffer[offsetIndex2] + delayBuffer[offsetIndex3] + delayBuffer[offsetIndex4] + delayBuffer[offsetIndex5]) / 3.0));
    outblockB->data[i] = int(round((delayBuffer[offsetIndex1B] + delayBuffer[offsetIndex2B] + delayBuffer[offsetIndex3B] + delayBuffer[offsetIndex4B] + delayBuffer[offsetIndex5B]) / 3.0));

  }

  transmit(outblock, 0);
  transmit(outblockB, 1);
  release(outblock);
  release(outblockB);
  if (block != &zeroblock) release((audio_block_t *)block);


  return;

}
