
#ifndef Looper_h
#define Looper_h

#include "Arduino.h"

#define NUM_FX 5

enum looper_mode {RECORDING, PLAYING, STOPPED};

class Looper
{
public:
  // ----- Constructor -----
  Looper(int rec, int play, int en, int rev, int fx_sel, int fx_par);
  
  void setEnabled(bool en);
  void setReverse(bool rev);
  void setMode(looper_mode md);
  void setFXSelector(int fx_sel);
  void setFXParameter(int fx_par);

  bool enabled;
  bool reverse;
  looper_mode mode;
  int total_bars;
  int current_bar;
  int fx_selector;
  int fx_parameter;

private:
  int _cc_rec;
  int _cc_play;
  int _cc_rev;
  int _cc_enable;
  int _cc_fx_sel;
  int _cc_fx_par;

};

#endif

// End