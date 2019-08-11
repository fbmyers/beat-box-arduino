/*
 * MIDIUSB_test.ino
 *
 * Created: 4/6/2015 10:47:08 AM
 * Author: gurbrinder grewal
 * Modified by Arduino LLC (2015)
 */ 
#include "pin_mapping.h"
#include "MIDIUSB.h"

//note: this needs to be in globals
enum looper_mode {RECORDING, PLAYING, STOPPED};

unsigned long time_since(unsigned long timestamp)
{
  return millis() - timestamp;
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Beat Box Controller");
  
  setup_controls();
  setup_leds();
  
  delay(1000);
  setup_ableton();

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
  process_controls();
  read_midi();
  process_leds();
  //handle_delayed_events();
  
  //
}
