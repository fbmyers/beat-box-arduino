#include "RotaryEncoder.h"
#include "Button.h"
#include "pin_mapping.h"

RotaryEncoder enc_track1_fx_sel(A0, A1);

Button btn_track1_rec(PIN_TRACK1_REC_BTN);
Button btn_track1_en(PIN_TRACK1_EN_BTN);

void setup_controls() {
  pinMode(PIN_TRACK1_REC_BTN, INPUT_PULLUP);
  pinMode(PIN_TRACK1_EN_BTN, INPUT_PULLUP);
              
}

// Read all buttons and rotary encoders
// Respond with appropriate MIDI messages
void process_controls() {
    if (btn_track1_en.tick()) {
        if (btn_track1_en.getState())
            setLooperEnabled(0,!looper[0].enabled);
        //Serial.print("TRACK1 EN ");
        //Serial.println(btn_track1_en.getState());
    }
    if (btn_track1_rec.tick()) {
        if (btn_track1_rec.getState()) // button down
            setLooperMode(0,RECORDING);
        else { // button up
            setLooperMode(0,PLAYING);  
            setLooperEnabled(0,true);
        }         
        //Serial.print("TRACK1 REC ");
        //Serial.println(btn_track1_rec.getState());    
    }

    if (enc_track1_fx_sel.tick()) {
        setLooperFXSelector(0,enc_track1_fx_sel.getPosition());
        enc_track1_fx_sel.setPosition(looper[0].fx_selector);
        Serial.println(enc_track1_fx_sel.getPosition());
    }

}
