/*
 * MIDIUSB_test.ino
 *
 * Created: 4/6/2015 10:47:08 AM
 * Author: gurbrinder grewal
 * Modified by Arduino LLC (2015)
 */ 
#include "pin_mapping.h"
#include "MIDIUSB.h"

#define WAIT_FOR_ABLETON true

//note: this needs to be in globals
enum looper_mode {RECORDING, PLAYING, STOPPED};

int midi_time = 0;

unsigned long time_since(unsigned long timestamp)
{
  return millis() - timestamp;
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Beat Box Controller");
  
  setup_controls();
  setup_leds();
  
  if (WAIT_FOR_ABLETON) {
    unsigned long last_attempt = millis();
    while (midi_time == 0) {
      read_midi();
      if (time_since(last_attempt)<2000)
        continue;

      last_attempt = millis();
      Serial.println("Attempting to Initialize Ableton...");
      setup_ableton();
    }
  }
}

/*
void beat(int num)
{
  if (num==0) {
    for (int i=0;i<NUM_TRACKS;i++) {
      if (track_state[i]==TRACK_ARM)
        set_track_state(i,TRACK_REC);
      else if (track_state[i]==TRACK_DISARM)
        set_track_state(i,TRACK_ON); 
    }
    Serial.println("Downbeat");
  }

  //pulse lights
  for (int i=0;i<NUM_LEDS;i++)
    if (led_state[i]==LED_PULSING)
      led_brightness[i] = 255;
}*/


void loop() {
  static int iter;

  process_controls();
  read_midi();

  //note: process_leds takes longer than the fastest button press/rotary event, so we throttle it
  if (iter%100==0) {
    process_leds();
  }
  //handle_delayed_events();
  
  iter++;
}
