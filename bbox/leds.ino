#include <Adafruit_NeoPixel.h>
#include "pin_mapping.h"

//#define NUM_LEDS 8
#define SM_RING 4
#define LG_RING 1
#define FX_SEL_LENGTH 5
#define BAR_LENGTH 4

#define NEOPIXEL_CONSOLE_LENGTH 30

Adafruit_NeoPixel pixels(NEOPIXEL_CONSOLE_LENGTH, PIN_NEOPIXEL_CONSOLE, NEO_GRBW + NEO_KHZ800);

//NOTE: would be nice to do this as a 2D array (or just indicate start locations)
const int NEO_TEMPO_RING = 28;
const int NEO_FX_SEL[] = {1,24,24,24};
const int NEO_FX_PAR[] = {5,24,24,24};
const int NEO_BARS[] = {14,24,24,24};
const int NEO_BOX_TOP = 29;

const int PIN_REC_LED[] = {8,40,41,42};
const int PIN_EN_LED[] = {6,43,44,45};

int led_min = 30;
int led_max = 120;

void setup_leds() {
  pixels.begin();
}

void update_leds() {
  /*
  static unsigned long last_update = 0;

  if (time_since(last_update) < 10)
    return;

  for (int i=0;i<NUM_LEDS;i++) {
    if (led_state[i]==LED_PULSING)
      led_brightness[i] -= 10; 

    analogWrite(LED_PINS[i],led_brightness[i]);
  }

  last_update = millis();
  */

  //for (int i=0;i<NUM_LEDS;i++) {
  //  analogWrite(LED_PINS[i],led_max[i]);
  //}
}

void setBargraph(int n) {
  if (looper[n].mode == PLAYING) {
    int progress = ((float)looper[n].current_beat / looper[n].total_beats) * BAR_LENGTH;

    for (int i=0;i<BAR_LENGTH;i++) {
      if (i < progress)
        pixels.setPixelColor(NEO_BARS[n]+i,pixels.Color(150,0,0));
      else
        pixels.setPixelColor(NEO_BARS[n]+i,pixels.Color(5,0,0));
    }
  }
  else if (looper[n].mode == RECORDING) {
    for (int i=0;i<BAR_LENGTH;i++)
      pixels.setPixelColor(NEO_BARS[n]+i,pixels.Color(255,0,0));
  }
}

void setFXRing(int n) {
    int pos = ((float)looper[n].fx_parameter / 127) * SM_RING;

    for (int i=0;i<SM_RING;i++) {
      if (i == pos)
        pixels.setPixelColor(NEO_FX_PAR[n]+i,pixels.Color(0,150,0));
      else
        pixels.setPixelColor(NEO_FX_PAR[n]+i,pixels.Color(0,5,0));
    }
}

//TODO: combine ring and selector into 1 fxn
void setFXSelLED(int n) {
  int selection = looper[n].fx_selector;
  if (selection==0)
    return;
  else
    pixels.setPixelColor(NEO_FX_SEL[n]+selection-1,pixels.Color(0,0,255));
}

void setTempoLED(float pulse) {
  int val = (int)pulse;
  for (int i=0;i<LG_RING;i++)
    pixels.setPixelColor(NEO_TEMPO_RING+i,pixels.Color(val,val,val));  
}

void setBeetTV() {

}

// Update internal LED state variables based on tempo, state, and LED effects
// Send updated LED state to analog and digital (Neopixel) LEDs
void process_leds() {
  pixels.clear();

  float pulse = (1.0-midi_time/24.0)*(led_max-led_min)+led_min;

  setTempoLED(pulse);
  setBeetTV();

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

    if (looper[i].enabled)
      analogWrite(PIN_EN_LED[i],255);
    else
      analogWrite(PIN_EN_LED[i],0);

    setBargraph(i);
    setFXRing(i);
    setFXSelLED(i);
  }
  
  //pixels.setPixelColor(enc_track1_fx_sel.getPosition(),pixels.Color(150,0,0));
  pixels.show();

}
