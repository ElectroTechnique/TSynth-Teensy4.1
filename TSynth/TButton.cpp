#include "TButton.h"

TButton::TButton(uint8_t pin): TButton(pin, LOW)
{
}

TButton::TButton(uint8_t pin, uint32_t activeState): TButton(pin, activeState, 1000, 20, 150)
{
}

TButton::TButton(uint8_t pin, uint32_t activeState, uint32_t holdThresh, uint32_t debounceWindow, uint32_t clickTime):
    pin(pin, debounceWindow),
    _holdThresh(holdThresh),
    _clickWindow(clickTime),
    _pressedState(activeState),
    _currentState(!activeState),
    _windowStartTime(0),
    _clicks(0),
    clicks(0),
    buttonHeld(false)
{
  pinMode(pin, INPUT);
}


void TButton::update()
{
    pin.update();
    uint32_t newState = pin.read();
    uint32_t now = millis();

    // State changed.
    bool changed = newState != _currentState;
    _currentState = newState;
    bool activated = this->pressed();

    // make sure button held resets
    buttonHeld = false;

    // Each new activation, increment the pending _clicks + reset window.
    if (activated && changed) {
        _windowStartTime = now;
        _clicks++;
    }
    
    uint32_t dur = now - this->_windowStartTime;

    // Apply (or reset) clicks.
    if (!activated && dur > _clickWindow) {
        clicks = _clicks;
        _clicks = 0;
        return;
    }

    // Apply held (and reset clicks).
    if (activated && dur > _holdThresh) {
        buttonHeld = true;
        _clicks = 0;
        return;
    }
}
