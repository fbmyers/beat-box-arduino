#include "RotaryEncoder.h"
#include "Button.h"
#include "pin_mapping.h"


RotaryEncoder enc_track1_fx_sel(PIN_TRACK1_FX_SEL_DT, PIN_TRACK1_FX_SEL_CLK);
RotaryEncoder enc_track2_fx_sel(PIN_TRACK2_FX_SEL_DT, PIN_TRACK2_FX_SEL_CLK);
RotaryEncoder enc_track3_fx_sel(PIN_TRACK3_FX_SEL_DT, PIN_TRACK3_FX_SEL_CLK);
RotaryEncoder enc_track4_fx_sel(PIN_TRACK4_FX_SEL_DT, PIN_TRACK4_FX_SEL_CLK);

RotaryEncoder enc_track1_fx_par(PIN_TRACK1_FX_PAR_DT, PIN_TRACK1_FX_PAR_CLK);
RotaryEncoder enc_track2_fx_par(PIN_TRACK2_FX_PAR_DT, PIN_TRACK2_FX_PAR_CLK);
RotaryEncoder enc_track3_fx_par(PIN_TRACK3_FX_PAR_DT, PIN_TRACK3_FX_PAR_CLK);
RotaryEncoder enc_track4_fx_par(PIN_TRACK4_FX_PAR_DT, PIN_TRACK4_FX_PAR_CLK);

RotaryEncoder enc_tempo(PIN_TEMPO_DT, PIN_TEMPO_CLK);

Button btn_track1_rec(PIN_TRACK1_REC_BTN);
Button btn_track2_rec(PIN_TRACK2_REC_BTN);
Button btn_track3_rec(PIN_TRACK3_REC_BTN);
Button btn_track4_rec(PIN_TRACK4_REC_BTN);

Button btn_track1_en(PIN_TRACK1_EN_BTN);
Button btn_track2_en(PIN_TRACK2_EN_BTN);
Button btn_track3_en(PIN_TRACK3_EN_BTN);
Button btn_track4_en(PIN_TRACK4_EN_BTN);

Button btn_track1_fx_sel(PIN_TRACK1_FX_SEL_BTN);
Button btn_track2_fx_sel(PIN_TRACK2_FX_SEL_BTN);
Button btn_track3_fx_sel(PIN_TRACK3_FX_SEL_BTN);
Button btn_track4_fx_sel(PIN_TRACK4_FX_SEL_BTN);

Button btn_track1_fx_par(PIN_TRACK1_FX_PAR_BTN);
Button btn_track2_fx_par(PIN_TRACK2_FX_PAR_BTN);
Button btn_track3_fx_par(PIN_TRACK3_FX_PAR_BTN);
Button btn_track4_fx_par(PIN_TRACK4_FX_PAR_BTN);

Button btn_tempo(PIN_TEMPO_BTN);

void setup_controls() {
    // unclear why, but putting these in the constructors for the encoder/button objects doesn't work
  pinMode(PIN_TRACK1_REC_BTN, INPUT_PULLUP);
  pinMode(PIN_TRACK1_EN_BTN, INPUT_PULLUP);
  pinMode(PIN_TRACK1_FX_SEL_CLK, INPUT_PULLUP);
  pinMode(PIN_TRACK1_FX_SEL_DT, INPUT_PULLUP);
  pinMode(PIN_TRACK1_FX_SEL_BTN, INPUT_PULLUP);
  pinMode(PIN_TRACK1_FX_PAR_CLK, INPUT_PULLUP);
  pinMode(PIN_TRACK1_FX_PAR_DT, INPUT_PULLUP);
  pinMode(PIN_TRACK1_FX_PAR_BTN, INPUT_PULLUP);
  
  pinMode(PIN_TRACK2_REC_BTN, INPUT_PULLUP);
  pinMode(PIN_TRACK2_EN_BTN, INPUT_PULLUP);
  pinMode(PIN_TRACK2_FX_SEL_CLK, INPUT_PULLUP);
  pinMode(PIN_TRACK2_FX_SEL_DT, INPUT_PULLUP);
  pinMode(PIN_TRACK2_FX_SEL_BTN, INPUT_PULLUP);
  pinMode(PIN_TRACK2_FX_PAR_CLK, INPUT_PULLUP);
  pinMode(PIN_TRACK2_FX_PAR_DT, INPUT_PULLUP);
  pinMode(PIN_TRACK2_FX_PAR_BTN, INPUT_PULLUP);

  pinMode(PIN_TRACK3_REC_BTN, INPUT_PULLUP);
  pinMode(PIN_TRACK3_EN_BTN, INPUT_PULLUP);
  pinMode(PIN_TRACK3_FX_SEL_CLK, INPUT_PULLUP);
  pinMode(PIN_TRACK3_FX_SEL_DT, INPUT_PULLUP);
  pinMode(PIN_TRACK3_FX_SEL_BTN, INPUT_PULLUP);
  pinMode(PIN_TRACK3_FX_PAR_CLK, INPUT_PULLUP);
  pinMode(PIN_TRACK3_FX_PAR_DT, INPUT_PULLUP);
  pinMode(PIN_TRACK3_FX_PAR_BTN, INPUT_PULLUP);

  pinMode(PIN_TRACK4_REC_BTN, INPUT_PULLUP);
  pinMode(PIN_TRACK4_EN_BTN, INPUT_PULLUP);
  pinMode(PIN_TRACK4_FX_SEL_CLK, INPUT_PULLUP);
  pinMode(PIN_TRACK4_FX_SEL_DT, INPUT_PULLUP);
  pinMode(PIN_TRACK4_FX_SEL_BTN, INPUT_PULLUP);
  pinMode(PIN_TRACK4_FX_PAR_CLK, INPUT_PULLUP);
  pinMode(PIN_TRACK4_FX_PAR_DT, INPUT_PULLUP);
  pinMode(PIN_TRACK4_FX_PAR_BTN, INPUT_PULLUP);

  pinMode(PIN_TEMPO_BTN, INPUT_PULLUP);
  pinMode(PIN_TEMPO_CLK, INPUT_PULLUP);
  pinMode(PIN_TEMPO_DT, INPUT_PULLUP);

}

// Read all buttons and rotary encoders
// Respond with appropriate MIDI messages
void process_controls() {
    
    if (btn_track1_en.tick()) {
        if (btn_track1_en.getState())
            setLooperEnabled(0,!looper[0].enabled);
        Serial.print("TRACK1 EN ");
        Serial.println(btn_track1_en.getState());
    }
    if (btn_track1_rec.tick()) {
        if (btn_track1_rec.getState()) // button down
            setLooperMode(0,RECORDING);
        else { // button up
            setLooperMode(0,PLAYING);  
            setLooperEnabled(0,true);
        }         
        Serial.print("TRACK1 REC ");
        Serial.println(btn_track1_rec.getState());    
    }

    if (btn_track2_en.tick()) {
        if (btn_track2_en.getState())
            setLooperEnabled(1,!looper[1].enabled);
        Serial.print("TRACK2 EN ");
        Serial.println(btn_track2_en.getState());
    }
    if (btn_track2_rec.tick()) {
        if (btn_track2_rec.getState()) // button down
            setLooperMode(1,RECORDING);
        else { // button up
            setLooperMode(1,PLAYING);  
            setLooperEnabled(1,true);
        }         
        Serial.print("TRACK2 REC ");
        Serial.println(btn_track2_rec.getState());    
    }

    if (btn_track3_en.tick()) {
        if (btn_track3_en.getState())
            setLooperEnabled(2,!looper[2].enabled);
        Serial.print("TRACK3 EN ");
        Serial.println(btn_track3_en.getState());
    }
    if (btn_track3_rec.tick()) {
        if (btn_track3_rec.getState()) // button down
            setLooperMode(2,RECORDING);
        else { // button up
            setLooperMode(2,PLAYING);  
            setLooperEnabled(2,true);
        }         
        Serial.print("TRACK3 REC ");
        Serial.println(btn_track3_rec.getState());    
    }

    if (btn_track4_en.tick()) {
        if (btn_track4_en.getState())
            setLooperEnabled(3,!looper[3].enabled);
        Serial.print("TRACK4 EN ");
        Serial.println(btn_track4_en.getState());
    }
    if (btn_track4_rec.tick()) {
        if (btn_track4_rec.getState()) // button down
            setLooperMode(3,RECORDING);
        else { // button up
            setLooperMode(3,PLAYING);  
            setLooperEnabled(3,true);
        }         
        Serial.print("TRACK4 REC ");
        Serial.println(btn_track4_rec.getState());    
    }


    if (enc_track1_fx_sel.tick()) {
        setLooperFXSelector(0,enc_track1_fx_sel.getPosition());
        enc_track1_fx_sel.setPosition(looper[0].fx_selector);
        Serial.print("TRACK1 SEL ");        
        Serial.println(enc_track1_fx_sel.getPosition());
    }
    if (enc_track1_fx_par.tick()) {
        setLooperFXParameter(0,enc_track1_fx_par.getPosition());
        enc_track1_fx_par.setPosition(looper[0].fx_parameter);
        Serial.print("TRACK1 PAR ");        
        Serial.println(enc_track1_fx_par.getPosition());
    }
    if (btn_track1_fx_sel.tick()) {
        if (btn_track1_fx_sel.getState())
            setLooperFXSelector(0,0);
        Serial.print("TRACK1 SELBTN ");
        Serial.println(btn_track1_fx_sel.getState());
    }
    if (btn_track1_fx_par.tick()) {
        setLooperReverse(0,btn_track1_fx_par.getState());
        Serial.print("TRACK1 PARBTN ");
        Serial.println(btn_track1_fx_par.getState());
    }

    if (enc_track2_fx_sel.tick()) {
        setLooperFXSelector(1,enc_track2_fx_sel.getPosition());
        enc_track2_fx_sel.setPosition(looper[1].fx_selector);
        Serial.print("TRACK2 SEL ");        
        Serial.println(enc_track2_fx_sel.getPosition());
    }
    if (enc_track2_fx_par.tick()) {
        setLooperFXParameter(1,enc_track2_fx_par.getPosition());
        enc_track2_fx_par.setPosition(looper[1].fx_parameter);
        Serial.print("TRACK2 PAR ");        
        Serial.println(enc_track2_fx_par.getPosition());
    }
    if (btn_track2_fx_sel.tick()) {
        if (btn_track2_fx_sel.getState())
            setLooperFXSelector(1,0);
        Serial.print("TRACK2 SELBTN ");
        Serial.println(btn_track2_fx_sel.getState());
    }
    if (btn_track2_fx_par.tick()) {
        setLooperReverse(1,btn_track2_fx_par.getState());
        Serial.print("TRACK2 PARBTN ");
        Serial.println(btn_track2_fx_par.getState());
    }

    if (enc_track3_fx_sel.tick()) {
        setLooperFXSelector(2,enc_track3_fx_sel.getPosition());
        enc_track3_fx_sel.setPosition(looper[2].fx_selector);
        Serial.print("TRACK3 SEL ");        
        Serial.println(enc_track3_fx_sel.getPosition());
    }
    if (enc_track3_fx_par.tick()) {
        setLooperFXParameter(2,enc_track3_fx_par.getPosition());
        enc_track3_fx_par.setPosition(looper[2].fx_parameter);
        Serial.print("TRACK3 PAR ");        
        Serial.println(enc_track3_fx_par.getPosition());
    }
    if (btn_track3_fx_sel.tick()) {
        if (btn_track3_fx_sel.getState())
            setLooperFXSelector(2,0);
        Serial.print("TRACK3 SELBTN ");
        Serial.println(btn_track3_fx_sel.getState());
    }
    if (btn_track3_fx_par.tick()) {
        setLooperReverse(2,btn_track3_fx_par.getState());
        Serial.print("TRACK3 PARBTN ");
        Serial.println(btn_track3_fx_par.getState());
    }  

    if (enc_track4_fx_sel.tick()) {
        setLooperFXSelector(3,enc_track4_fx_sel.getPosition());
        enc_track4_fx_sel.setPosition(looper[3].fx_selector);
        Serial.print("TRACK4 SEL ");        
        Serial.println(enc_track4_fx_sel.getPosition());
    }
    if (enc_track4_fx_par.tick()) {
        setLooperFXParameter(3,enc_track4_fx_par.getPosition());
        enc_track4_fx_par.setPosition(looper[3].fx_parameter);
        Serial.print("TRACK4 PAR ");        
        Serial.println(enc_track4_fx_par.getPosition());
    }
    if (btn_track4_fx_sel.tick()) {
        if (btn_track4_fx_sel.getState())
            setLooperFXSelector(3,0);
        Serial.print("TRACK4 SELBTN ");
        Serial.println(btn_track4_fx_sel.getState());
    }
    if (btn_track4_fx_par.tick()) {
        setLooperReverse(3,btn_track4_fx_par.getState());
        Serial.print("TRACK4 PARBTN ");
        Serial.println(btn_track4_fx_par.getState());
    }  


   if (enc_tempo.tick()) {
        setTempo(enc_tempo.getPosition());
        enc_tempo.setPosition(tempo);
        Serial.print("TEMPO ");        
        Serial.println(enc_tempo.getPosition());
    }
    if (btn_tempo.tick()) {
        setLooperReverse(0,btn_tempo.getState());
        setLooperReverse(1,btn_tempo.getState());
        setLooperReverse(2,btn_tempo.getState());
        setLooperReverse(3,btn_tempo.getState());

        Serial.print("TEMPO BTN ");
        Serial.println(btn_tempo.getState());
    }      
}
