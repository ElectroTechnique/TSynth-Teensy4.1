#ifndef Oscilloscope_h_
#define Oscilloscope_h_
#include "AudioStream.h"
#include "ST7735_t3.h"

class Oscilloscope : public AudioStream
{
  public:
    Oscilloscope(void) : AudioStream(1, inputQueueArray) {
    }
    virtual void update(void);
    void ScreenSetup(ST7735_t3*);
    void AudioToPixel(int16_t*);

  private:
    audio_block_t *inputQueueArray[5];;
    volatile bool new_output;
    ST7735_t3 *display;
    int16_t old_val[AUDIO_BLOCK_SAMPLES];
};
#endif

void Oscilloscope::ScreenSetup(ST7735_t3 *screen) {
  __disable_irq();
  display = screen;
  __enable_irq();
}


void Oscilloscope::AudioToPixel(int16_t *audio) {
  const int16_t *begin = audio;
  const int16_t *end = audio + AUDIO_BLOCK_SAMPLES;
  __disable_irq();
  do {
    int16_t wave_data = *audio;
    int16_t pixel_y = map(wave_data, 32767, -32768, -100, 100) + 63;
    int16_t pixel_x = audio - begin;
    display->drawPixel(pixel_x + 15, old_val[pixel_x], 0);//Remove previous pixel
    display->drawPixel(pixel_x + 15, pixel_y, 0x07A0);//New pixel
    old_val[pixel_x] = {pixel_y};
    audio++;
  } while (audio < end);
  __enable_irq();
}

void Oscilloscope::update(void) {
  if (!display) return;
  audio_block_t *block;
  block = receiveReadOnly(0);
  if (block) {
    AudioToPixel(block->data);
    release(block);
  }
}
