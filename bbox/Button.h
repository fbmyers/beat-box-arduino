
#ifndef Button_h
#define Button_h

#include "Arduino.h"

#define DEBOUNCE_DELAY 10

class Button
{
public:
  // ----- Constructor -----
  Button(int pin);
  
  // retrieve the current position
  bool  getState();
  
  // call this function every some milliseconds or by using an interrupt for handling state changes of the rotary encoder.
  bool tick(void);

private:
  int _pin; // Arduino pins used for the button. 
  
  volatile bool _lastState;
  volatile bool _currentState;
  volatile long _lastDebounceTime;
  
};

#endif

// End
