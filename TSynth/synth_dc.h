/* Audio Library for Teensy 3.X
 * Portions Copyright (c) 2014, Paul Stoffregen, paul@pjrc.com
 * Portions Copyright (c) 2021, Vince R. Pearson (exponential glide functionality)
 * 
 * Development of this audio library was funded by PJRC.COM, LLC by sales of
 * Teensy and Audio Adaptor boards.  Please support PJRC's efforts to develop
 * open source software by purchasing Teensy or other PJRC products.
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
 
 // Fraction of glide step (expTarget-expMagnitude) added to target to reduce long exp settling tail.
 // Equivalent to 1.01*(1-exp(-a*t)) stopping at 1.0 with value of 0.01.
#define EXP_TARGET_MARGIN 0.01
#ifndef synth_dc_h_
#define synth_dc_h_
#include "Arduino.h"
#include "AudioStream.h"
#include "utility/dspinst.h"

// compute (a - b) / c
// handling 32 bit integer overflow at every step
// without resorting to slow 64 bit math
#if defined(__ARM_ARCH_7EM__)
static inline int32_t substract_int32_then_divide_int32(int32_t a, int32_t b, int32_t c) __attribute__((always_inline, unused));
static inline int32_t substract_int32_then_divide_int32(int32_t a, int32_t b, int32_t c)
{
   int r;
  r = substract_32_saturate(a,b);
  if ( !get_q_psr() ) return (r/c);
  clr_q_psr();
  if ( c==0 ) r=0;
  if (__builtin_abs(c)<=1) return r;
  return (a/c)-(b/c);
}
#else
// compute (a - b) / c  ... handling 32 bit interger overflow without slow 64 bit math
static inline int32_t substract_int32_then_divide_int32(int32_t a, int32_t b, int32_t c) __attribute__((always_inline, unused));
static inline int32_t substract_int32_then_divide_int32(int32_t a, int32_t b, int32_t c)
{
  uint32_t diff;
  uint8_t negative;

  if (a >= 0) {
    if (b >= 0) {
      return (a - b) / c;  // no overflow if both a & b are positive
    } else {
      diff = a + (b * -1); // assumes 0x80000000 * -1 == 0x80000000
      negative = 0;
    }
  } else {
    if (b >= 0) {
      diff = (a * -1) + b; // assumes 0x80000000 * -1 == 0x80000000
      negative = 1;
    } else {
      return (a - b) / c;  // no overflow if both a & b are negative
    }
  }
  if (c >= 0) {
    diff = diff / (uint32_t)c;
  } else {
    diff = diff / (uint32_t)(c * -1);
    negative ^= 1;
  }
  if (negative) {
    if (diff > 0x7FFFFFFF) return 0x80000000;
    return (int32_t)diff * -1;
  } else {
    if (diff > 0x7FFFFFFF) return 0x7FFFFFFF;
    return (int32_t)diff;
  }
}

#endif

class AudioSynthWaveformDcTS : public AudioStream
{
public:
  AudioSynthWaveformDcTS() : AudioStream(0, NULL)
  {
    mode=modePending=GLIDE_EXP;
    state=0;
    magnitude=expMagnitude=0;
    stepDirection=1;
    target=expTarget=0;
    expTargetB=0;
    kf=0x7ff00000; // Arbitrary positive coefficient until set. 
  }
  // immediately jump to the new DC level
  FLASHMEM void amplitude(float n) {
    if (n > 1.0f) n = 1.0f;
    else if (n < -1.0f) n = -1.0f;
    int32_t m = (int32_t)(n * 2147418112.0f); // 0x7fff0000
    __disable_irq();
    magnitude = m;
    expMagnitude=m>>2;
    ysum=(int64_t)expMagnitude<<32;
    state = 0;
    __enable_irq();
  }
  // slowly transition to the new DC level
  FLASHMEM void amplitude(float n, float milliseconds) {
    if (milliseconds <= 0.0f) {
      amplitude(n);
      return;
    }
    if (n > 1.0f) n = 1.0f;
    else if (n < -1.0f) n = -1.0f;
    int32_t c = (int32_t)(milliseconds*(AUDIO_SAMPLE_RATE_EXACT/1000.0f));
    if (c == 0) {
      amplitude(n);
      return;
    }
    int32_t t = (int32_t)(n * 2147418112.0f);
    //
    int32_t k=(0x80000000UL)*(1.0f-exp(-2.0f/(milliseconds*AUDIO_SAMPLE_RATE_EXACT/1000.0f)));
    // Create 
    int32_t et = t>>2;
    int32_t bt=et+EXP_TARGET_MARGIN*((int64_t)expTarget-(int64_t)expMagnitude);
    // Need to keep track of step direction for threshold crossing detection
    int8_t sd;
    if(expTarget>expMagnitude)
      sd=1;
    else
      sd=-1;
    
    int8_t st;
    if(((target==magnitude)&&(mode==GLIDE_LIN)) || ((expTarget==expMagnitude)&&(mode==GLIDE_EXP)))
      st=0;
    else st=1;

    __disable_irq();
    target = t;
    expTarget= et;
    expTargetB=bt;
    ysum=(int64_t)expMagnitude<<32;
    kf=k;
    stepDirection=sd;
    
    if(st==0){
      state=0;   
      __enable_irq();
      return;
    }
    
    increment = substract_int32_then_divide_int32(target, magnitude, c);
    if (increment == 0) {
      increment = (target > magnitude) ? 1 : -1;
    }
    state = 1;
    __enable_irq();
  }
  FLASHMEM float read(void) {
    int32_t m = magnitude;
    if(mode!=GLIDE_EXP)
        m=magnitude;
    else
        m=expMagnitude<<2;
    return (float)m * (float)(1.0 / 2147418112.0);
  }
  enum {GLIDE_LIN=0,GLIDE_EXP=1};
  FLASHMEM void setMode(uint8_t mode)
  {
    // For now the mode should only change when the state is zero so set it to pending.
    // Changing mode from GLIDE_EXP to GLIDE_LIN may mess up the calculations.
    if(mode!= GLIDE_EXP) 
      this->modePending=GLIDE_LIN; //default to linear for all other values.
    else this->modePending=GLIDE_EXP; // may have to wait until next state==0 for this to be set.
  }
  uint8_t getMode() { return mode;};
  
  virtual void update(void);
private:

  uint8_t  state;     // 0=steady output, 1=transitioning
  int32_t  magnitude; // current output
  int32_t  target;    // designed output (while transitiong)
  int32_t  increment; // adjustment per sample (while transitiong)
  
  // Exponential glide variables
  uint8_t  mode,modePending; // Linear or exponential mode. Pending allows a glide to finish with the mode it started with. 
  int8_t   stepDirection; // Used to track step direction for target threshold detection.
  int64_t  ysum;    // filter accumulator
  int32_t  expMagnitude; // Magnitude value. This is scaled down by 4 from the linear case.
  int32_t  expTarget;   // Target value. This is scaled down by 4 from the linear case.
  int32_t  expTargetB;  // Biased target value. This is set just beyond the target so the target can be hit earlier.
  int32_t  kf; // FIlter coefficient.
};

#endif
