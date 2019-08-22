#include <Adafruit_NeoPixel.h>
#include "pin_mapping.h"

#define TEST_LEDS false

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
  if (TEST_LEDS)
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
  if ((looper[n].mode == PLAYING) && looper[n].enabled) {
    int progress = ceil(((float)looper[n].current_beat / looper[n].total_beats) * BAR_LENGTH);
    uint16_t hues[] = {0,10000,22000,44000};

    for (int i=0;i<BAR_LENGTH;i++) {
      if (looper[n].reverse==false) {
        if (i < progress)
          console_pixels.setPixelColor(NEO_BARS[n]+i,console_pixels.ColorHSV(hues[n],255,30));
        else
          console_pixels.setPixelColor(NEO_BARS[n]+i,console_pixels.ColorHSV(hues[n],255,1));
      }
      else {
        if (i > (BAR_SIZE-progress))
          console_pixels.setPixelColor(NEO_BARS[n]+i,console_pixels.ColorHSV(hues[n],255,30));
        else
          console_pixels.setPixelColor(NEO_BARS[n]+i,console_pixels.ColorHSV(hues[n],255,1));        
      }
    }
  }
  else if (looper[n].mode == RECORDING) {
    for (int i=0;i<BAR_LENGTH;i++)
      console_pixels.setPixelColor(NEO_BARS[n]+i,console_pixels.ColorHSV(1000,255,255));
  }
}

void setFXRing(int n) {
  if (((looper[n].mode == PLAYING) && !looper[n].enabled) || (looper[n].mode == STOPPED))
    return;

  uint16_t hues[] = {59000,43000,18000,31500,7000};

  int selection = looper[n].fx_selector;
  if (selection==0)
    return;

  int pos = looper[n].fx_parameter % SM_RING_SIZE;

  for (int i=0;i<SM_RING_SIZE;i++) {
    if (i == pos)
      console_pixels.setPixelColor(NEO_FX_PAR[n]+i,console_pixels.ColorHSV(hues[selection-1],100,100));
    else
      console_pixels.setPixelColor(NEO_FX_PAR[n]+i,console_pixels.ColorHSV(hues[selection-1],200,10));
  }

}

//TODO: combine ring and selector into 1 fxn
void setFXSelLED(int n) {
  if (((looper[n].mode == PLAYING) && !looper[n].enabled) || (looper[n].mode == STOPPED))
    return;

  uint16_t hues[] = {59000,43000,18000,31500,7000};
  uint8_t val = 50*sin(2*PI*millis()/500) + 150;

  int selection = looper[n].fx_selector;
  if (selection==0)
    return;
  else
    console_pixels.setPixelColor(NEO_FX_SEL[n]+selection-1,console_pixels.ColorHSV(hues[selection-1],150,val));
}

void setTempoLED() {
  float pulse = (1.0-current_tick/24.0)*0.7 + 0.3; //goes from 100% to 30%

  uint32_t col;
  if (current_beat==0)
    col = console_pixels.ColorHSV(11000,100,70*pulse);
  else
    col = console_pixels.ColorHSV(11000,200,50*pulse);
  
  for (int i=0;i<LG_RING_SIZE;i++)
    console_pixels.setPixelColor(NEO_TEMPO_RING+i,col);  

  if (tempo<70)
    console_pixels.setPixelColor(NEO_TEMPO_RING+current_tick-1,console_pixels.ColorHSV(11000,100,100));

}

void setBeetTV() {
  static int last_tick = 0;

  float pulse = (1.0-current_tick/24.0)*0.7 + 0.3; 

  //float pulse = (1.0-current_tick/24.0)*(led_max-led_min)+led_min;

  //int val = (int)pulse/2;

  for (int i=0;i<BEET_BODY_SIZE;i++) {

    //set them all to white at downbeat
    if (current_beat==0) {
      console_pixels.setPixelColor(NEO_BEET_BODY+i,console_pixels.ColorHSV(64000,100,70*pulse)); 
    }
    else {
      console_pixels.setPixelColor(NEO_BEET_BODY+i,console_pixels.ColorHSV(64000,230,70*pulse)); 
    }

  }

  for (int i=0;i<BEET_LEAF_SIZE;i++) {
    if (i<current_tick/3) {
      console_pixels.setPixelColor(NEO_BEET_LEAVES[0]+i,console_pixels.Color(0,100,0)); 
      console_pixels.setPixelColor(NEO_BEET_LEAVES[1]+i,console_pixels.Color(0,100,0)); 
    }
    else {
      console_pixels.setPixelColor(NEO_BEET_LEAVES[0]+i,console_pixels.Color(0,30,0)); 
      console_pixels.setPixelColor(NEO_BEET_LEAVES[1]+i,console_pixels.Color(0,30,0));      
    }
  }
     
}

void setButtons(int n) {

    if (looper[n].mode == RECORDING) {
      analogWrite(PIN_REC_LED[n],255);
      analogWrite(PIN_EN_LED[n],0);     
    }
    else if ((looper[n].mode) == PLAYING && looper[n].enabled) {
      analogWrite(PIN_REC_LED[n],255);
      analogWrite(PIN_EN_LED[n],255);
    }
    else {
      analogWrite(PIN_REC_LED[n],10);
      analogWrite(PIN_EN_LED[n],3);
    }
}

// Update internal LED state variables based on tempo, state, and LED effects
// Send updated LED state to analog and digital (Neopixel) LEDs
void process_leds() {
  console_pixels.clear();

  setTempoLED();
  setBeetTV();

  //looper lights
  for (int i=0;i<4;i++) {
    setButtons(i);
    setBargraph(i);
    setFXRing(i);
    setFXSelLED(i);
  }
  
  console_pixels.show();

}
