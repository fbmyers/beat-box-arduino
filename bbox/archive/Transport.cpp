#include "Arduino.h"
#include "midi.h"
#include "Transport.h"
#include "pin_mapping.h"

//note: probably shoould be a singleton?

  // ----- Constructor -----
Transport::Transport() {
    setStartStop(true);
    setMetronome(true);
    setTempo(120);
}
  
void Transport::setStartStop(bool start) {
    if (start)
      controlChange(0,MIDI_TRANSPORT_PLAY,127);
    else
      controlChange(0,MIDI_TRANSPORT_STOP,127);  
    playing = start;
}

void Transport::setMetronome(bool en) {
    if (en)
      controlChange(0,MIDI_TRANSPORT_METRONOME,127);
    else
      controlChange(0,MIDI_TRANSPORT_METRONOME,0);  
    metronome_enabled = en;
}

void Transport::setTempo(int t) {
    if (t > 127)
        t = 127;
    else if (t < 0)
        t = 0;
    controlChange(0,MIDI_TRANSPORT_TEMPO,t);  
    tempo = t; 
}
