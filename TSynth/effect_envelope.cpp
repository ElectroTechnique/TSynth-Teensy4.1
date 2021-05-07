/* Audio Library for Teensy 3.X
 * Portions Copyright (c) 2017, Paul Stoffregen, paul@pjrc.com
 * Portions Copyright (c) 2021 Vince R. Pearson
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

#include <Arduino.h>
#include "effect_envelope.h"

#define RELEASE_BIAS 256 // based on a 1.31 fixed point integer. This is the level below zero that is the release target and causes the output to go to zero earlier. Otherwise it can take a relatively long time.
// Difference equation for exponential envelope.
// y(n+1) = k1*y(n)+k2 using unsigned 1.31 fixed point format
#define EXP_ITERATION(y,k1,k2) ((uint32_t)(((uint64_t)(y)*(k1))>>31)+(k2))
// y(n+1)=k1*(y(n)-x)+x
#define EXP_ITERATION2(y,k1,k2)      (uint32_t)(((uint64_t)(y)*(k1)+(uint64_t)k1*k2)>>32)
//(uint32_t)((uint64_t)(y)*(k1)+(uint64_t)k2*(EXP_ENV_ONE-k1)>>32)

// STATE_IDLE_NEXT is added for exponential envelope. Variables are set in STATE_IDLE and then
// transitions to STATE_IDLE_NEXT while note is off so the variables are not reinitialized every loop while idle.
/*
#define STATE_IDLE  0  
#define STATE_DELAY 1
#define STATE_ATTACK  2
#define STATE_HOLD  3
#define STATE_DECAY 4
#define STATE_SUSTAIN 5
#define STATE_RELEASE 6
#define STATE_FORCED  7
#define STATE_IDLE_NEXT 8 // STATE_IDLE_NEXT saves some processing time when note is off.
*/

void AudioEffectEnvelopeTS::noteOn(void)
{
  __disable_irq();
  // Include STATE_IDLE_NEXT
  if (state == STATE_IDLE || state==STATE_IDLE_NEXT|| state == STATE_DELAY || release_forced_count == 0 || STATE_IDLE_NEXT) {
    mult_hires = 0;
    count = delay_count;
    if (count > 0) {
      state = STATE_DELAY;
      inc_hires = 0;
    } else {
      state = STATE_ATTACK;
      count = attack_count;
      inc_hires = 0x40000000 / (int32_t)count;
    }
  } else if (state != STATE_FORCED) {
    state = STATE_FORCED;
    count = release_forced_count;
    inc_hires = (-mult_hires) / (int32_t)count;
  }
  __enable_irq();
}

void AudioEffectEnvelopeTS::noteOff(void)
{
  __disable_irq();
  if (state != STATE_IDLE && state != STATE_FORCED && state!=STATE_IDLE_NEXT /*|| state!=STATE_RELEASE*/) {
    // Technically noteOff() should not occur when in STATE_RELEASE but added cach for that so count does not get reloaded.
    state = STATE_RELEASE;
    count = release_count;
    inc_hires = (-mult_hires) / (int32_t)count;
  }
  __enable_irq();
}

void AudioEffectEnvelopeTS::update(void)
{
  audio_block_t *block;
  uint32_t *p, *end;
  uint32_t sample12, sample34, sample56, sample78, tmp1, tmp2;
  uint32_t exp_mult[8];

  block = receiveWritable();
  if (!block) return;
  if (state == STATE_IDLE) {
    release(block);
    return;
  }
  p = (uint32_t *)(block->data);
  end = p + AUDIO_BLOCK_SAMPLES/2;
  if(env_type==-128)
  { // Original AudioEffectEnvelope class linear envelop.
    while (p < end) {
      // we only care about the state when completing a region
      if (count == 0) {
        if (state == STATE_ATTACK) {
          count = hold_count;
          if (count > 0) {
            state = STATE_HOLD;
            mult_hires = 0x40000000;
            inc_hires = 0;
          } else {
            state = STATE_DECAY;
            count = decay_count;
            inc_hires = (sustain_mult - 0x40000000) / (int32_t)count;
          }
          continue;
        } else if (state == STATE_HOLD) {
          state = STATE_DECAY;
          count = decay_count;
          inc_hires = (sustain_mult - 0x40000000) / (int32_t)count;
          continue;
        } else if (state == STATE_DECAY) {
          state = STATE_SUSTAIN;
          count = 0xFFFF;
          mult_hires = sustain_mult;
          inc_hires = 0;
        } else if (state == STATE_SUSTAIN) {
          count = 0xFFFF;
        } else if (state == STATE_RELEASE) {
          state = STATE_IDLE;
          while (p < end) {
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
          }
          break;
        } else if (state == STATE_FORCED) {
          mult_hires = 0;
          count = delay_count;
          if (count > 0) {
            state = STATE_DELAY;
            inc_hires = 0;
          } else {
            state = STATE_ATTACK;
            count = attack_count;
            inc_hires = 0x40000000 / (int32_t)count;
          }
        } else if (state == STATE_DELAY) {
          state = STATE_ATTACK;
          count = attack_count;
          inc_hires = 0x40000000 / count;
          continue;
        }
      }

      int32_t mult = mult_hires >> 14;
      int32_t inc = inc_hires >> 17;
      // process 8 samples, using only mult and inc (16 bit resolution)
      sample12 = *p++;
      sample34 = *p++;
      sample56 = *p++;
      sample78 = *p++;
      p -= 4;
      mult += inc;
      tmp1 = signed_multiply_32x16b(mult, sample12);
      mult += inc;
      tmp2 = signed_multiply_32x16t(mult, sample12);
      sample12 = pack_16b_16b(tmp2, tmp1);
      mult += inc;
      tmp1 = signed_multiply_32x16b(mult, sample34);
      mult += inc;
      tmp2 = signed_multiply_32x16t(mult, sample34);
      sample34 = pack_16b_16b(tmp2, tmp1);
      mult += inc;
      tmp1 = signed_multiply_32x16b(mult, sample56);
      mult += inc;
      tmp2 = signed_multiply_32x16t(mult, sample56);
      sample56 = pack_16b_16b(tmp2, tmp1);
      mult += inc;
      tmp1 = signed_multiply_32x16b(mult, sample78);
      mult += inc;
      tmp2 = signed_multiply_32x16t(mult, sample78);
      sample78 = pack_16b_16b(tmp2, tmp1);
      *p++ = sample12;
      *p++ = sample34;
      *p++ = sample56;
      *p++ = sample78;
      // adjust the long-term gain using 30 bit resolution (fix #102)
      // https://github.com/PaulStoffregen/Audio/issues/102
      mult_hires += inc_hires;
      count--;
    }
  }
  else  //Exponential ADSR Vince R. Pearson
  {
    // Exponential envelope generator
    uint16_t i;
    while (p < end) 
    {
      for(i=0;i<8;i++)
      {
        switch(state)
        {
          case STATE_IDLE:
            // Since delay counts may not necessarily start at aligned boundaries of 8 samples,
            // the loop count has to be multiplied by 8
            // and counted down in the for loop, not in the while loop.
            exp_count=((uint32_t)(delay_count))*8;
            ysum=0;
            state=STATE_IDLE_NEXT; // Do this so reinitialization is only done once at every idle state.
          case STATE_IDLE_NEXT:
            break; //ysum is zero here
          case STATE_DELAY:
            if(exp_count--) break; // ysum is zero here
            state=STATE_ATTACK;
            break;
          case STATE_ATTACK:
            ysum=EXP_ITERATION(ysum,attack_k,attack_target);
            if(ysum>=EXP_ENV_ONE)  
            {   // The maximum 32 bit value of the envelope has been reached.
              ysum=EXP_ENV_ONE;
              if(hold_count)
              {
                exp_count=((uint32_t)hold_count)*8;
                state=STATE_HOLD;
              }
              else 
              {
                state=STATE_DECAY;
                exp_count=((uint32_t)delay_count)*8; // Only used to arbitrarioly define where sustain begins.
              }
            }
            break;
          case STATE_HOLD:
            if((exp_count--)==0) 
            {
               state=STATE_DECAY; // ysum is maximum here (ENV_MAX_32).
               exp_count=((uint32_t)delay_count)*8;
            }
            break;
          case STATE_DECAY:
            if((exp_count--)==0) state=STATE_SUSTAIN;
            // Sustain is only needed to support isRelease(). This happens after delay_count is decremented to zero.
            // The point at which sustain begins after the start of decay is arbitrary on an exponential curve
            // Here it occurs after one time constant (delay_count*8) which is 63.2% down the decay curve.
          case STATE_SUSTAIN: 
            ysum=EXP_ITERATION(ysum,decay_k,decay_target);
            break;
          case STATE_RELEASE:
            ysum=EXP_ITERATION(ysum,release_k,0);
            // Subtract bias value to end release a bit sooner. Must check value for underflow since unsigned integers are used.
            // This has more of an effect at the longest release times since it is a fixed value.
            if(ysum>RELEASE_BIAS) ysum-=RELEASE_BIAS; 
            if((ysum>>15)==0) // All of the useful bits are zero so no reason to stay in this state.
              state=STATE_IDLE;
            break;
          case STATE_FORCED:
            ysum=EXP_ITERATION(ysum,release_forced_k,0);
            if((ysum>>15)==0) 
              state=STATE_ATTACK;// revert to IDLE state when useful bits are zero.
          default:
            break;  
        }
        exp_mult[i]=ysum>>15;
      }
      // multiply audio samples with 8 envelope samples
      sample12 = *p++;
      sample34 = *p++;
      sample56 = *p++;
      sample78 = *p++;
      p -= 4;
      tmp1 = signed_multiply_32x16b(exp_mult[0], sample12);
      tmp2 = signed_multiply_32x16t(exp_mult[1], sample12);
      sample12 = pack_16b_16b(tmp2, tmp1);
      tmp1 = signed_multiply_32x16b(exp_mult[2], sample34);
      tmp2 = signed_multiply_32x16t(exp_mult[3], sample34);
      sample34 = pack_16b_16b(tmp2, tmp1);
      tmp1 = signed_multiply_32x16b(exp_mult[4], sample56);
      tmp2 = signed_multiply_32x16t(exp_mult[5], sample56);
      sample56 = pack_16b_16b(tmp2, tmp1);
      tmp1 = signed_multiply_32x16b(exp_mult[6], sample78);
      tmp2 = signed_multiply_32x16t(exp_mult[7], sample78);
      sample78 = pack_16b_16b(tmp2, tmp1);
      *p++ = sample12;
      *p++ = sample34;
      *p++ = sample56;
      *p++ = sample78;
    }
  }
  transmit(block);
  release(block);
}

bool AudioEffectEnvelopeTS::isActive()
{
  uint8_t current_state = *(volatile uint8_t *)&state;
  if ((current_state == STATE_IDLE)|| (current_state==STATE_IDLE_NEXT)) return false; // Added STATE_IDLE_NEXT. VRP
  return true;
}

bool AudioEffectEnvelopeTS::isSustain()
{
  uint8_t current_state = *(volatile uint8_t *)&state;
  if (current_state == STATE_SUSTAIN) return true;
  return false;
}
