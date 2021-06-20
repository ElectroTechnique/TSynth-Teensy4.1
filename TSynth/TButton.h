#ifndef TButton_H
#define TButton_H

#include <Arduino.h>
#include <Bounce.h>

/**
 * Wrap a pin to help detect different types of button activation.
 * To use call update() for each loop of your program, use numClicks(),
 * and held() to see if there was an activation. The activation will
 * be cleared  on the next update() call to avoid triggering multiple
 * times.
 *
 * The following options are provided:
 * pin - which pin to read, pinMode is set for this pin automatically.
 * activeState - This should be something like HIGH or LOW.
 * holdThresh - Duration in milliseconds that a button must be active to trigger held().
 * debounceWindow - Duration in milliseconds to consider for debounce.
 * clickTime - Duration in milliseconds to check for clicks.
 */
class TButton
{
  private:
    Bounce pin;
    uint32_t _holdThresh;
    uint32_t _clickWindow;
    uint8_t _pressedState;
    uint8_t _currentState;
    uint32_t _windowStartTime;
    uint32_t _clicks;

    // cached state for accessing
    int clicks;
    boolean buttonHeld;

  public:
    TButton(uint8_t pin);
    TButton(uint8_t pin, uint32_t activeState);
    TButton(uint8_t pin, uint32_t activeState, uint32_t holdThresh, uint32_t debounceWindow, uint32_t clickTime);
    void update();
    inline int numClicks(){ return clicks; };
    inline int numClicksPending(){ return _clicks; };
    bool pressed(){ return _currentState == _pressedState; };
    bool held(){ return buttonHeld; };
};

#endif