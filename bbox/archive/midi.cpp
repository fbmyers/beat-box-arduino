#include "MIDIUSB.h"
#include "midi.h"


// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
  MidiUSB.flush();  
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
  MidiUSB.flush();  
}

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).
void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();  
}

midiEventPacket_t read_midi() {
  // Receive MIDI tempo message
  midiEventPacket_t rx;
  do {
    rx = MidiUSB.read();
    if (rx.header != 0) {
      /*
      if (rx.header == 8) { //note on
        if (rx.byte2==0x40) //downbeat
          beat(0);
        else if (rx.byte2==0x3F) //2...
          beat(1);
        else if (rx.byte2==0x3E) //3...
          beat(2);
        else if (rx.byte2==0x3D) //4...
          beat(3);
      }
      */
      if (rx.byte1 == 0x90) { //hote off
        if ((rx.byte2 >= 1) && (rx.byte2 <= 4)) { //looper beat state
          int n = rx.byte2;
          looper[n].current_beat = rx.byte3;
          if (rx.byte3 > looper[n].total_beats)
            looper[n].total_beats = rx.byte3;

          //since looper state only comes on the beat, assume this is also the start of a new beat
          midi_time = 0;
        }

        if ((rx.byte2 >= 41) && (rx.byte2 <= 44)) { //looper rec/play state
          int n = rx.byte2 - 41;
          switch (rx.byte3) {
            case 1:
              break;
            case 2: // record
              looper[n].mode = RECORD;
              break;
            case 3: // playback
              looper[n].mode = PLAYING;
              looper[n].total_beats = 0; //need this info from ableton
              break;
          }
          //since looper state only comes on the beat, assume this is also the start of a new beat
          midi_time = 0;
        }

      if (rx.byte1 = 0xF8)
        midi_time++;

      }

      if (midi_time==1) {
        Serial.print("LOOP1 ");
        if (looper[0].mode == RECORD)
          Serial.print("REC ");
        if (looper[0].mode == PLAYING)
          Serial.print("PLY ");
        Serial.print(looper[0].current_beat);
        Serial.print("/");
        Serial.print(looper[0].total_beats);
      }


      Serial.print("Received: ");
      Serial.print(rx.byte1, HEX);
      Serial.print("-");
      Serial.print(rx.byte2, HEX);
      Serial.print("-");
      Serial.println(rx.byte3, HEX);
      
    }
  } while (rx.header != 0);
    
}
