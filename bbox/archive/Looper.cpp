#include "Arduino.h"
#include "midi.h"
#include "Looper.h"

//note: better way to do this would be to assign a separate midi channel for transport and each looper, and then just pass the CC messages directly to the object with that channel assignment.

  Looper::Looper(int rec, int play, int en, int rev, int fx_sel, int fx_par) {
    _cc_rec = rec;
    _cc_play = play;
    _cc_rev = rev;
    _cc_enable = en;
    _cc_fx_sel = fx_sel;
    _cc_fx_par = fx_par;

    setEnabled(false);
    setReverse(false);
    setMode(PLAYING);
    setFXSelector(0);
    setFXParameter(64);

  }
  
  void  Looper::setEnabled(bool en) {
    int val = en ? 127 : 0;
    controlChange(0,_cc_enable,val);
    enabled = en;
  }

  void  Looper::setReverse(bool rev) {
    int val = rev ? 127 : 0;
    controlChange(0,_cc_rev,val);
    reverse = rev;
  }

  void  Looper::setMode(looper_mode md) {
    if (md = RECORDING)
      controlChange(0,_cc_rec,127);
    else if (md = PLAYING)
      controlChange(0,_cc_play,127);   
    //mode = md; this is set upon actual start of recording  
  }
  
  void  Looper::setFXSelector(int fx_sel) {
    if (fx_sel >= NUM_FX)
        fx_sel = 0;
    else if (fx_sel < 0)
        fx_sel = NUM_FX-1;
    controlChange(0,_cc_fx_sel,fx_sel);
    fx_selector = fx_sel;  
  }
  
  void  Looper::setFXParameter(int fx_par) {
    if (fx_par > 127)
        fx_par = 127;
    else if (fx_par < 0)
        fx_par = 0;
    controlChange(0,_cc_fx_par,fx_par);
    fx_parameter = fx_par;  
  }
