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
  uint8_t pixel_x = ceil(AUDIO_BLOCK_SAMPLES / 4);
  int16_t prev_pixel_y = 63;
  for (uint8_t i = 0; i < ceil(AUDIO_BLOCK_SAMPLES / 2) - 1; i++) {
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

  // Get a rise of the wave between the first and the third quarter of the samples
  uint8_t quarter_size = ceil(AUDIO_BLOCK_SAMPLES / 4);
  uint8_t rise_index = 0;
  prev_audio = *(audio + quarter_size);
  for (uint8_t i = quarter_size; i < ceil(AUDIO_BLOCK_SAMPLES * 3 / 4); i++) {
    if (prev_audio < -1 && *(audio + i) > 1) {
      rise_index = i;
      break;
    }
    prev_audio = *(audio + i);
  }

  // In case no rise is found, just use the middle
  if (rise_index == 0) {
    rise_index = ceil(AUDIO_BLOCK_SAMPLES / 2);
  }

  // Then get half samples arround this rise and add it to the buffer
  for (uint8_t i = rise_index - quarter_size; i < rise_index + quarter_size - 1; i++) {
    buffer[count++] = *(audio + i);
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
