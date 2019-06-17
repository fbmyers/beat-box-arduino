/*
 * MIDIUSB_test.ino
 *
 * Created: 4/6/2015 10:47:08 AM
 * Author: gurbrinder grewal
 * Modified by Arduino LLC (2015)
 */ 
#include "MIDIUSB.h"

#define PIN_TRACK_1_BTN 13
#define PIN_TRACK_1_LED 12

#define MIDI_CC_TRACK_1_ARM
#define MIDI_CC_TRACK_1_DISARM
#define MIDI_CC_TRACK_1_ON
#define MIDI_CC_TRACK_1_OFF

#define NUM_BUTTONS 1
#define NUM_LEDS 1

#define NUM_TRACKS 4

#define HOLD_THRESHOLD_MSEC 200
#define DEBOUNCE_TIME_MSEC 10

//probably need to add undo (if button released within 100ms while track_rec)
enum TRACK_STATES {TRACK_OFF, TRACK_ON, TRACK_ARM, TRACK_REC, TRACK_UNDO, TRACK_DISARM};
enum LED_STATES {LED_OFF, LED_ON, LED_PULSING};

const int BUTTON_PINS[] = {13, 11, 9, 7, A0, A2, A4, A6};
const int LED_PINS[] = {12, 10, 8, 6, A1, A3, A5};


//button messages 36, 37, 38, 39, 40
//41 = looper 1 (value 1,2,3,4)

//[track][msg], where msg is:
//0 = track on/off
//1 = arm & record looper
//2 = play looper
//3 = clear looper
//4 = undo looper
const int MIDI_CC_MSG[][5] = {{50,29,38,52,53},
                             {67,68,69,70,71},
                             {67,68,69,70,71},
                             {67,68,69,70,71}};

bool button_state[NUM_BUTTONS];
unsigned long button_timestamp[NUM_BUTTONS];

int led_state[NUM_LEDS];
int led_brightness[NUM_LEDS];

int track_state[NUM_TRACKS];

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

void set_track_state(int track, TRACK_STATES state)
{
  Serial.print("        Track "); Serial.print(track);
  switch (state) {
    case TRACK_OFF:
      controlChange(0, MIDI_CC_MSG[track][0], 0); // mute track
      set_led_state(track,LED_OFF);      
      Serial.println(" OFF");
      break;     
    case TRACK_ON:
      controlChange(0, MIDI_CC_MSG[track][0], 127); // unmute track and play looper
      set_led_state(track,LED_PULSING);
      Serial.println(" ON");
      break;
    case TRACK_ARM: // arm track and press record on looper
      controlChange(0, MIDI_CC_MSG[track][1], 127); 
      Serial.println(" ARM");
      break; 
    case TRACK_REC: // looper is now recording
      set_led_state(track,LED_ON);    
      Serial.println(" REC");
      break;
    case TRACK_UNDO: // clear recording, undo, and play looper
      controlChange(0, MIDI_CC_MSG[track][3], 127);
      controlChange(0, MIDI_CC_MSG[track][4], 127);
       controlChange(0, MIDI_CC_MSG[track][2], 127);
      
      set_track_state(track, TRACK_ON);
      Serial.println(" UNDO");
      break;      
    case TRACK_DISARM: // disarm looper and play
      controlChange(0, MIDI_CC_MSG[track][2], 127);     
      Serial.println(" DISARM");
      break;  
    default:
      Serial.println(" UNDEF");              
  }
  track_state[track] = state;
}

void set_led_state(int led, LED_STATES state)
{
  if ((state==LED_ON) || (state==LED_PULSING))
    led_brightness[led] = 1024;
  else if (state==LED_OFF)
    led_brightness[led] = 0;
    
  led_state[led] = state;
}

unsigned long time_since(unsigned long timestamp)
{
  return millis() - timestamp;
}

void button_change(int btn,bool state)
{
  Serial.print("Button "); Serial.print(btn);
  Serial.println(state ? " DOWN" : " UP");

  switch (btn) {
    case 0: //track 1
    case 1:
    case 2:
    case 3:
      if (state==true) { //button is down
        switch (track_state[btn]) {
          case TRACK_OFF:
            set_track_state(btn, TRACK_ON); //immediately turn on the track if it's off
            break;
          case TRACK_ON:
            set_track_state(btn, TRACK_ARM); //track is on and button was pressed (either turn off or start recording)
            break;    
        }
      }
      else { //button is up
        switch (track_state[btn]) {
          case TRACK_ARM:  
            set_track_state(btn, TRACK_OFF);
            break;
          case TRACK_REC:
            if (time_since(button_timestamp[btn]) < HOLD_THRESHOLD_MSEC) 
              set_track_state(btn, TRACK_UNDO);
            else
              set_track_state(btn, TRACK_DISARM);
            break;            
        }      
      }
      
    case 4:
    case 5:
    case 6:
    case 7:
      break;
  }
  button_state[btn] = state;
  button_timestamp[btn] = millis();
}

void setup() {
  Serial.begin(115200);
  
  for (int i=0;i<NUM_BUTTONS;i++) {
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);
    pinMode(LED_PINS[i], OUTPUT);
    button_state[i] = false;
  }

  for (int i=0;i<NUM_TRACKS;i++) {
    track_state[i] = TRACK_OFF;
  }
}

void read_buttons() {
  for (int i=0;i<NUM_BUTTONS;i++) {
    if (time_since(button_timestamp[i]) > DEBOUNCE_TIME_MSEC) {
      bool new_state = (digitalRead(BUTTON_PINS[i])==0);
      if (new_state != button_state[i]) 
        button_change(i,new_state);  
    }
  }
}

void handle_delayed_events() {
  for (int i=0;i<NUM_TRACKS;i++) {
    if (track_state[i]==TRACK_ARM) {
      if (time_since(button_timestamp[i]) > HOLD_THRESHOLD_MSEC)
        set_track_state(i, TRACK_REC);
    }
  }
}

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
}

void update_leds() {
  static unsigned long last_update = 0;

  if (time_since(last_update) < 10)
    return;
    
  for (int i=0;i<NUM_LEDS;i++) {
    if (led_state[i]==LED_PULSING)
      led_brightness[i] -= 10; 

    analogWrite(LED_PINS[i],led_brightness[i]);
  }

  last_update = millis();
}

void read_midi() {
  // Receive MIDI tempo message
  midiEventPacket_t rx;
  do {
    rx = MidiUSB.read();
    if (rx.header != 0) {
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
      /*    
      Serial.print("Received: ");
      Serial.print(rx.header, HEX);
      Serial.print("-");
      Serial.print(rx.byte1, HEX);
      Serial.print("-");
      Serial.print(rx.byte2, HEX);
      Serial.print("-");
      Serial.println(rx.byte3, HEX);
      */
    }
  } while (rx.header != 0);
    
}

void loop() {
  read_buttons();
  read_midi();
  update_leds();
  //handle_delayed_events();
  
  //
}
