#ifndef Oscilloscope_h_
#define Oscilloscope_h_
#include "AudioStream.h"
#include "ST7735_t3.h"

#define AUDIO_BLOCKS  1

class Oscilloscope : public AudioStream
{
  public:
    Oscilloscope(void) : AudioStream(1, inputQueueArray) {
    }
    virtual void update(void);
    void ScreenSetup(ST7735_t3*);
    void AudioToPixel(int16_t*);
    void process(void);
    int count = 0;

  private:
    bool next_buffer = true;
    bool first_run;
    volatile bool process_buffer;
    audio_block_t *blocklist1[AUDIO_BLOCKS];
    audio_block_t *blocklist2[AUDIO_BLOCKS];
    audio_block_t *inputQueueArray[1];
    ST7735_t3 *display;
    int16_t old_val[AUDIO_BLOCK_SAMPLES];
};
#endif

void Oscilloscope::ScreenSetup(ST7735_t3 *screen) {
  __disable_irq();
  display = screen;
  next_buffer = true;
  __enable_irq();
}


void Oscilloscope::AudioToPixel(int16_t *audio) {
  const int16_t *begin = audio;
  const int16_t *end = audio + AUDIO_BLOCK_SAMPLES;
  __disable_irq();
  boolean rising = false;
  int16_t pixel_x = 0;
  do {
    int16_t wave_data = *audio;
    if (rising || (wave_data > 0 && wave_data < 10)) {
      audio++;
      wave_data = *audio;
      if (rising || wave_data > 10) {
        rising = true;
        int16_t pixel_y = map(wave_data, 32767, -32768, -100, 100) + 63;
        //int16_t pixel_x = audio - begin;
        display->drawPixel(pixel_x + 15, old_val[pixel_x], 0);//Remove previous pixel
        display->drawPixel(pixel_x + 15, pixel_y, 0x07A0);//New pixel
        old_val[pixel_x] = {pixel_y};
        pixel_x++;
      }
    }
    audio++;
  } while (audio < end);

  __enable_irq();
}

void Oscilloscope::process(void) {
//  const int16_t *begin = audio;
//  const int16_t *end = audio + AUDIO_BLOCK_SAMPLES;
//  __disable_irq();
//  boolean rising = false;
//  int16_t pixel_x = 0;
//  do {
//    int16_t wave_data = *audio;
//    if (rising || (wave_data > 0 && wave_data < 10)) {
//      audio++;
//      wave_data = *audio;
//      if (rising || wave_data > 10) {
//        rising = true;
//        int16_t pixel_y = map(wave_data, 32767, -32768, -100, 100) + 63;
//        //int16_t pixel_x = audio - begin;
//        display->drawPixel(pixel_x + 15, old_val[pixel_x], 0);//Remove previous pixel
//        display->drawPixel(pixel_x + 15, pixel_y, 0x07A0);//New pixel
//        old_val[pixel_x] = {pixel_y};
//        pixel_x++;
//      }
//    }
//    audio++;
//  } while (audio < end);
//
//  __enable_irq();
}

void Oscilloscope::update(void) {
  if ( !display )return;
//  audio_block_t *block;
//  block = receiveReadOnly();
//  if (!block) return;
//
//  if ( next_buffer ) {
//    blocklist1[state++] = block;
//    if ( !first_run && process_buffer ) process( );
//  } else {
//    blocklist2[state++] = block;
//    if ( !first_run && process_buffer ) process( );
//  }
//
//  if ( state >= AUDIO_BLOCKS ) {
//    if ( next_buffer ) {
//      if ( !first_run && process_buffer ) process( );
//      for ( int i = 0; i < AUDIO_BLOCKS; i++ ) copy_buffer( AudioBuffer + ( i * 0x80 ), blocklist1[i]->data );
//      for ( int i = 0; i < AUDIO_BLOCKS; i++ ) release( blocklist1[i] );
//      next_buffer = false;
//    } else {
//      if ( !first_run && process_buffer ) process( );
//      for ( int i = 0; i < AUDIO_BLOCKS; i++ ) copy_buffer( AudioBuffer + ( i * 0x80 ), blocklist2[i]->data );
//      for ( int i = 0; i < AUDIO_BLOCKS; i++ ) release( blocklist2[i] );
//      next_buffer = true;
//    }
//    process_buffer = true;
//    first_run = false;
//    state = 0;
//  }
}
