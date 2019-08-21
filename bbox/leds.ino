#include <Adafruit_NeoPixel.h>
#include "pin_mapping.h"

//#define NUM_LEDS 8
#define SM_RING_SIZE 12
#define LG_RING_SIZE 24
#define BAR_SIZE 16
#define FX_SEL_SIZE 5
#define BEET_BODY_SIZE 64
#define BEET_LEAF_SIZE 8
#define BAR_LENGTH 16

#define NEOPIXEL_CONSOLE_LENGTH 236
#define NEOPIXEL_BOX_LENGTH 60

Adafruit_NeoPixel console_pixels(NEOPIXEL_CONSOLE_LENGTH, PIN_NEOPIXEL_CONSOLE, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel box_pixels(NEOPIXEL_BOX_LENGTH, PIN_NEOPIXEL_BOX, NEO_GRB + NEO_KHZ800);

// Starting locations for various indicators
const int NEO_TEMPO_RING = 0;
const int NEO_FX_SEL[] = {52,85,118,151};
const int NEO_FX_PAR[] = {40,73,106,139};
const int NEO_BARS[] = {24,57,90,123};

const int NEO_BEET_BODY = 156;
const int NEO_BEET_LEAVES[] = {220,228};
const int NEO_BOX_TOP = 0;

const int PIN_REC_LED[] = {PIN_TRACK1_REC_LED,PIN_TRACK2_REC_LED,PIN_TRACK3_REC_LED,PIN_TRACK4_REC_LED};
const int PIN_EN_LED[] = {PIN_TRACK1_EN_LED,PIN_TRACK2_EN_LED,PIN_TRACK3_EN_LED,PIN_TRACK4_EN_LED};

int led_min = 30;
int led_max = 120;

void setup_leds() {
  console_pixels.begin();
  test_leds();
}

void test_leds() {
  console_pixels.clear();
  for (int j=0;j<LG_RING_SIZE;j++) {
    console_pixels.setPixelColor(NEO_TEMPO_RING+j,console_pixels.Color(100,100,100));
    console_pixels.show();
    delay(20);
  }

  for (int i=0;i<4;i++) {
    console_pixels.clear();
    for (int j=0;j<BAR_SIZE;j++) {
      console_pixels.setPixelColor(NEO_BARS[i]+j,console_pixels.Color(100,0,0));
      console_pixels.show();
      delay(20);
    }

    console_pixels.clear();
    for (int j=0;j<SM_RING_SIZE;j++) {
      console_pixels.setPixelColor(NEO_FX_PAR[i]+j,console_pixels.Color(0,100,0));
      console_pixels.show();
      delay(20);
    }

    console_pixels.clear();
    for (int j=0;j<FX_SEL_SIZE;j++) {
      console_pixels.setPixelColor(NEO_FX_SEL[i]+j,console_pixels.Color(0,0,100));
      console_pixels.show();
      delay(50);
    }
  }

  console_pixels.clear();
  for (int j=0;j<BEET_BODY_SIZE;j++) {
    console_pixels.setPixelColor(NEO_BEET_BODY+j,console_pixels.Color(20,0,20));
    console_pixels.show();
    delay(20);
  }

  for (int i=0;i<2;i++) {
    for (int j=0;j<BEET_LEAF_SIZE;j++) {
      console_pixels.setPixelColor(NEO_BEET_LEAVES[i]+j,console_pixels.Color(0,100,0));
      console_pixels.show();
      delay(20);
    }
  }

  console_pixels.clear();
  console_pixels.show();

  for (int i=0;i<4;i++) {
    analogWrite(PIN_EN_LED[i],255);
    delay(200);
    //analogWrite(PIN_EN_LED[i],0);
  }

  for (int i=0;i<4;i++) {
    analogWrite(PIN_REC_LED[i],255);
    delay(200);
    //analogWrite(PIN_REC_LED[i],0);
  }

}

void setBargraph(int n) {
  if (looper[n].mode == PLAYING) {
    int progress = ceil(((float)looper[n].current_beat / looper[n].total_beats) * BAR_LENGTH);

    for (int i=0;i<BAR_LENGTH;i++) {
      if (i < progress)
        console_pixels.setPixelColor(NEO_BARS[n]+i,console_pixels.Color(150,0,0));
      else
        console_pixels.setPixelColor(NEO_BARS[n]+i,console_pixels.Color(5,0,0));
    }
  }
  else if (looper[n].mode == RECORDING) {
    for (int i=0;i<BAR_LENGTH;i++)
      console_pixels.setPixelColor(NEO_BARS[n]+i,console_pixels.Color(255,0,0));
  }
}

void setFXRing(int n) {
    int pos = ((float)looper[n].fx_parameter / 127) * SM_RING_SIZE;

    for (int i=0;i<SM_RING_SIZE;i++) {
      if (i == pos)
        console_pixels.setPixelColor(NEO_FX_PAR[n]+i,console_pixels.Color(0,150,0));
      else
        console_pixels.setPixelColor(NEO_FX_PAR[n]+i,console_pixels.Color(0,5,0));
    }
}

//TODO: combine ring and selector into 1 fxn
void setFXSelLED(int n) {
  int selection = looper[n].fx_selector;
  if (selection==0)
    return;
  else
    console_pixels.setPixelColor(NEO_FX_SEL[n]+selection-1,console_pixels.Color(0,0,255));
}

void setTempoLED(float pulse) {
  int val = (int)pulse;
  for (int i=0;i<LG_RING_SIZE;i++)
    console_pixels.setPixelColor(NEO_TEMPO_RING+i,console_pixels.Color(val,val,val));  
}

void setBeetTV(float pulse) {
  int val = (int)pulse/2;
  for (int i=0;i<BEET_BODY_SIZE;i++)
    console_pixels.setPixelColor(NEO_BEET_BODY+i,console_pixels.Color(val,0,val)); 
  for (int i=0;i<BEET_LEAF_SIZE;i++) {
    console_pixels.setPixelColor(NEO_BEET_LEAVES[0]+i,console_pixels.Color(0,val,0)); 
    console_pixels.setPixelColor(NEO_BEET_LEAVES[1]+i,console_pixels.Color(0,val,0)); 
  }
     
}

// Update internal LED state variables based on tempo, state, and LED effects
// Send updated LED state to analog and digital (Neopixel) LEDs
void process_leds() {
  console_pixels.clear();

  float pulse = (1.0-midi_time/24.0)*(led_max-led_min)+led_min;

  setTempoLED(pulse);
  setBeetTV(pulse);

  //looper lights
  for (int i=0;i<4;i++) {
    if (looper[i].mode == RECORDING) {
      analogWrite(PIN_REC_LED[i],255);
    }
    else if (looper[i].mode == PLAYING) {
      analogWrite(PIN_REC_LED[i],(int)pulse);
    }
    else {
      analogWrite(PIN_REC_LED[i],0);
    }

    if (looper[i].enabled) {
      analogWrite(PIN_EN_LED[i],255);
      setBargraph(i);
      setFXRing(i);
      setFXSelLED(i);
    }
    else
      analogWrite(PIN_EN_LED[i],0);
  }
  
  //pixels.setPixelColor(enc_track1_fx_sel.getPosition(),pixels.Color(150,0,0));
  console_pixels.show();

}
