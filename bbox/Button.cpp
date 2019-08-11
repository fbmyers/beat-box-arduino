#include "Arduino.h"
#include "Button.h"


Button::Button(int pin) {

  _pin = pin;
  //pinMode(pin, INPUT_PULLUP); //for some reason, this has the opposite effect when placed here in the constructor
  _currentState = false;
} 

bool Button::getState() {
    return _currentState;
}

//Returns true if the button's state has changed
bool Button::tick() {
    bool reading = (digitalRead(_pin)==0);
    if (reading!=_lastState) {
        _lastDebounceTime = millis();
        _lastState = reading;
    }

    if ((millis() - _lastDebounceTime) > DEBOUNCE_DELAY) {
        if (reading != _currentState) {
            _currentState = reading;
            return true;
        }
    }

    return false;
}
