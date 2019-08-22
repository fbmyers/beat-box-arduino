#include "MIDIUSB.h"


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
  delay(10);  
}

midiEventPacket_t read_midi() {
  // Receive MIDI tempo message
  midiEventPacket_t rx;
  do {
    rx = MidiUSB.read();
    if (rx.header != 0) {

      // TODO: only allow one looper to record at a time
      if (rx.byte1 == 0x90) { //hote on
        /* 
        if ((rx.byte2 >= 1) && (rx.byte2 <= 4)) { //looper beat state
          int n = rx.byte2 - 1;
          looper[n].current_beat = rx.byte3;
          if (rx.byte3 > looper[n].total_beats)
            looper[n].total_beats = rx.byte3;

          //since looper state only comes on the beat, assume this is also the start of a new beat
          midi_time = 0;
        }
        */
        if (rx.byte2 == 0x6C) {
          current_beat = 0; //downbeat
          current_tick = 0;
        }
        else if (rx.byte2 == 0x6D) { 
          current_beat = 1; //beat 2
          current_tick = 0;
        }
        else if (rx.byte2 == 0x6E) { 
          current_beat = 2; //beat 3
          current_tick = 0;
        }
        else if (rx.byte2 == 0x6F) { 
          current_beat = 3; //beat 4
          current_tick = 0;
        }
        else if ((rx.byte2 >= 41) && (rx.byte2 <= 44)) { //looper rec/play state
          int n = rx.byte2 - 41;
          switch (rx.byte3) {
            case 1:
              break;
            case 2: // record
              looper[n].mode = RECORDING;
              looper[n].total_beats = 0;
              looper[n].current_beat = 0;
              break;
            case 3: // playback
              looper[n].mode = PLAYING;
              looper[n].current_beat = 0;
              break;
          }

        }

      }

      if (rx.byte1 == 0xF8) {
        current_tick++;
        Serial.print("-");
      }

      if (current_tick==1) { //since I get so many messages at each beat, wait for the first tick
        for (int i=0;i<4;i++)
            if (looper[i].mode == RECORDING)
                looper[i].total_beats++;
            else if (looper[i].mode == PLAYING) {
                looper[i].current_beat++;
                if (looper[i].current_beat > looper[i].total_beats)
                    looper[i].current_beat = 1;
            }
      }

      
      if (current_tick==1) {
        Serial.print(" LOOP1 ");
        if (looper[0].mode == RECORDING)
          Serial.print("REC ");
        if (looper[0].mode == PLAYING)
          Serial.print("PLY ");
        Serial.print(looper[0].current_beat);
        Serial.print("/");
        Serial.print(looper[0].total_beats);
        if (looper[0].enabled)
          Serial.println(" ON");
        else
            Serial.println(" OFF");
      }

      /*
      Serial.print("Received: ");
      Serial.print(rx.byte1, HEX);
      Serial.print("-");
      Serial.print(rx.byte2, HEX);
      Serial.print("-");
      Serial.println(rx.byte3, HEX);
      */
    }
  } while (rx.header != 0);
    
}
