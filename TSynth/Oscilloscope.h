#ifndef Oscilloscope_h_
#define Oscilloscope_h_
#include "core_pins.h" // AudioStream.h needs F_CPU_ACTUAL
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

