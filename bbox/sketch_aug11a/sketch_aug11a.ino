#include <Adafruit_NeoPixel.h>
#define NUM_PIXELS 236

Adafruit_NeoPixel pixels(NUM_PIXELS, 14, NEO_GRB + NEO_KHZ800);


void setup() {
  // put your setup code here, to run once:
  pixels.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  pixels.clear();
  for (int i=52;i<57;i++) {
    //pixels.clear();
    pixels.setPixelColor(i,pixels.Color(100,100,100));
    //pixels.setPixelColor(i-1,pixels.Color(0,100,0));
    //pixels.setPixelColor(i-2,pixels.Color(0,0,100));
    //pixels.show();
    //delay(10);
  }
  pixels.show();
    delay(1000);
}
