#ifndef Oscilloscope_h_
#define Oscilloscope_h_
#include "AudioStream.h"
#include "ST7735_t3.h"

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
    int16_t buffer[AUDIO_BLOCK_SAMPLES];
};
#endif

void Oscilloscope::ScreenSetup(ST7735_t3 *screen) {
  display = screen;
}

void Oscilloscope::Display() {
  uint8_t pixel_x = 0;
  int16_t prev_pixel_y = 63;
  for (uint8_t i = 0; i < AUDIO_BLOCK_SAMPLES - 1; i++) {
    int16_t pixel_y = map(buffer[i], 32767, -32768, -120, 120) + 63;
    if (pixel_y < 30) pixel_y = 30;
    if (pixel_y > 100)pixel_y = 100;
    display->drawLine(pixel_x + 15, prev_pixel_y, pixel_x + 16, pixel_y, 0x07B0);
    prev_pixel_y = pixel_y;
    pixel_x++;
  }
}

void Oscilloscope::AddtoBuffer(int16_t *audio) {
  int16_t prev_audio = 0;
  uint32_t count = 0;
  prev_audio = *audio;
  audio++;
  if (prev_audio > 1 && *audio < -1 ) {
    for (uint8_t i = 0; i < AUDIO_BLOCK_SAMPLES - 1; i++) {
      buffer[count++] = *audio;
      audio++;
    }
  }
}

void Oscilloscope::update(void) {
  if (!display) return;
  audio_block_t *block;
  block = receiveReadOnly(0);
  if (block) {
    AddtoBuffer(block->data);
    release(block);
    Display();
  }
}
