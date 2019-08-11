
#ifndef Transport_h
#define Transport_h

#include "Arduino.h"

#define MIDI_TRANSPORT_PLAY 1
#define MIDI_TRANSPORT_STOP 2
#define MIDI_TRANSPORT_METRONOME 3
#define MIDI_TRANSPORT_TEMPO 5

class Transport
{
public:
  // ----- Constructor -----
  Transport();
  
  void setStartStop(bool start);
  void setMetronome(bool en);
  void setTempo(int tempo);
  
  bool playing;
  bool metronome_enabled;
  int tempo;

};

#endif