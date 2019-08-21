#include "pin_mapping.h"

#define MIDI_TRANSPORT_PLAY 1
#define MIDI_TRANSPORT_STOP 2
#define MIDI_TRANSPORT_METRONOME 3
#define MIDI_TRANSPORT_TEMPO 5

#define DEFAULT_TEMPO 40

#define NUM_FX 6
#define FX_PAR_STEP_SIZE 3

const int MIDI_LOOPER_REC[] = {10,11,12,13};
const int MIDI_LOOPER_PLAY[] = {20,21,22,23};
const int MIDI_LOOPER_EN[] = {30,31,32,33};
const int MIDI_LOOPER_FX_SEL[] = {40,41,42,43};
const int MIDI_LOOPER_FX_PAR[] = {50,51,52,53};
const int MIDI_LOOPER_REV[] = {60,61,62,63};


struct looper_state {
    bool enabled = false;
    bool reverse = false;
    looper_mode mode = STOPPED;
    int total_beats = 0;
    int current_beat = 0;
    int fx_selector = 0;
    int fx_parameter = 64;
} looper[4];

bool playing;
bool metronome_enabled;
int tempo;


void setup_ableton() {

    //TODO: turn OFF transport, reset all loopers, then restart
    setMetronome(true);
    setTempo(DEFAULT_TEMPO);

    for (int i=0;i<4;i++) {
        setLooperMode(i,PLAYING);
        setLooperEnabled(i,false);
        setLooperFXParameter(i,64);
        setLooperFXSelector(i,0);
        setLooperReverse(i,false);
    }

    setStartStop(true);
}
  
void setStartStop(bool start) {
    if (start)
      controlChange(0,MIDI_TRANSPORT_PLAY,127);
    else
      controlChange(0,MIDI_TRANSPORT_STOP,127);  
    playing = start;
}

void setMetronome(bool en) {
    if (en)
      controlChange(0,MIDI_TRANSPORT_METRONOME,127);
    else
      controlChange(0,MIDI_TRANSPORT_METRONOME,0);  
    metronome_enabled = en;
}

void setTempo(int t) {
    if (t > 127)
        t = 127;
    else if (t < 0)
        t = 0;
    controlChange(0,MIDI_TRANSPORT_TEMPO,t);  
    tempo = t; 
}


void  setLooperEnabled(int num, bool en) {
    int val = en ? 127 : 0;
    controlChange(0,MIDI_LOOPER_EN[num],val);
    looper[num].enabled = en;
}

void  setLooperReverse(int num, bool rev) {
    int val = rev ? 127 : 0;
    controlChange(0,MIDI_LOOPER_REV[num],val);
    looper[num].reverse = rev;
}

void  setLooperMode(int num, looper_mode md) {
    if (md == RECORDING)
        controlChange(0,MIDI_LOOPER_REC[num],127);
    else if (md == PLAYING)
        controlChange(0,MIDI_LOOPER_PLAY[num],127);   
    //mode = md; this is set upon actual start of recording  
}

void  setLooperFXSelector(int num, int fx_sel) {
    if (fx_sel >= NUM_FX)
        fx_sel = 0;
    else if (fx_sel < 0)
        fx_sel = NUM_FX-1;
    controlChange(0,MIDI_LOOPER_FX_SEL[num],fx_sel);
    looper[num].fx_selector = fx_sel;  
}

void  setLooperFXParameter(int num, int fx_par) {
    if (fx_par > (127/FX_PAR_STEP_SIZE))
        fx_par = (127/FX_PAR_STEP_SIZE);
    else if (fx_par < 0)
        fx_par = 0;
    controlChange(0,MIDI_LOOPER_FX_PAR[num],fx_par*FX_PAR_STEP_SIZE);
    looper[num].fx_parameter = fx_par;  
}
