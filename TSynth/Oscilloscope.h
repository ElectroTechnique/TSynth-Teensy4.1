#ifndef Oscilloscope_h_
#define Oscilloscope_h_
#include "AudioStream.h"
#include "ST7735_t3.h"
#define NO_OF_BLOCKS 2

class Oscilloscope : public AudioStream {
  public:
    Oscilloscope(void) : AudioStream(1, inputQueueArray) {
    }
    virtual void update(void);
    void ScreenSetup(ST7735_t3*);
    void Display(void);
    void AddtoBuffer(int16_t*);

  private:
    audio_block_t *inputQueueArray[1];
    ST7735_t3 *display;
    int16_t buffer[AUDIO_BLOCK_SAMPLES * NO_OF_BLOCKS];
    int16_t old_val[AUDIO_BLOCK_SAMPLES];
    uint32_t count = 0;
};
#endif

void Oscilloscope::ScreenSetup(ST7735_t3 *screen) {
  __disable_irq();
  display = screen;
  __enable_irq();
}

void Oscilloscope::Display() {
  __disable_irq();
  boolean crossing = false;
  int16_t pixel_x = 0;
  uint32_t i = 0;
  do {
    int16_t wave_data = buffer[i];
    if (crossing || wave_data > 0) {
      i++;
      wave_data = buffer[i];
      if (crossing || wave_data <0 ) {
        crossing = true;
        int16_t pixel_y = map(wave_data, 32767, -32768, -100, 100) + 63;
        display->drawPixel(pixel_x + 15, old_val[pixel_x], 0);//Remove previous pixel
        display->drawPixel(pixel_x + 15, pixel_y, 0x07B0);//New pixel
        old_val[pixel_x] = {pixel_y};
        pixel_x++;
      }
    }
    i++;
  } while (i < AUDIO_BLOCK_SAMPLES * NO_OF_BLOCKS);

  __enable_irq();
}

void Oscilloscope::AddtoBuffer(int16_t *audio) {
  const int16_t *begin = audio;
  const int16_t *end = audio + AUDIO_BLOCK_SAMPLES;
  __disable_irq();
  do {
    buffer[count++] = *audio;
    audio++;
  } while (audio < end);
    if(count > (AUDIO_BLOCK_SAMPLES * NO_OF_BLOCKS) -1){
      count = 0;
      Display();
    }
  __enable_irq();
}

void Oscilloscope::update(void) {
  if (!display) return;
  audio_block_t *block;
  block = receiveReadOnly(0);
  if (block) {
    AddtoBuffer(block->data);
    release(block);
  }
}
