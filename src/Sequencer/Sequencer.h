//
//  Sequencer.h
//  
//
//  Created by Christodoulos Aspromallis on 01/03/2016.
//
//

#ifndef ____Sequencer__
#define ____Sequencer__

#include <stdio.h>
#include "ofxMidiOut.h"
//#include "ofxMidi.h" //??

//class Blues_structure;

class Sequencer{
    
public:
    
    //constructors
    //Sequencer(Sequencer const &);//http://stackoverflow.com/questions/25794057/c-pointer-being-freed-was-not-allocated
    //Sequencer& operator=(Sequencer const &);
    Sequencer(){}
    ~Sequencer(){}
    
    //functions
    void setup(); //setup() --> midiOut.openPort(0);
    void update(); //update()
    void metronome(vector<int>& t, bool metro_on=false, int bar_len=4, bool bar_click=true, bool group_on=true, vector<int> groupings={});//playing a metronome
    vector<int> timer();//returning semiquavers, beats, bars, cycles
    bool only_on(string division, vector<int>& t);//e.g. beat, t
    
    int on_off(int pitch, int dur, int vel=100, int channel=1);//sendNoteOn/Off according to duration - dur in beats
    
    //CHORDS
    void play_chord_sequence(vector<int>& t);//when, which chord etc..
    void time_placement(vector<int>& t, vector<int>& notes, vector<int>& pos, int dur=4, int ch=1);//managing duration..
    void chord_on_off(bool on_off, vector<int>& chord, int ch = 1, int vel = 100);
    void choose_chord(vector<int> metros, vector<int> history);//compute what chord to play next, return selection of elements (mixed type? or convert get_chord args to ints?) for get_chord().
    
    void stop_all_MIDI();
    
    //variables
    ofxMidiOut midiOut;
    
    //metro vars
    int QN_subdivision;
    int tick; //8 per 16th note?
    int semiquaver; //smallest grid value
    int beat;
    int bar;
    int cycle;
    int detach;
    int incr;
    int metro_on=0;

    int metre; //time signature.. (in QNs)
    int cycle_len;
    
    //note vars
    int pitch;
    int chord_ch; //chords channel
    int metro_ch; //metronome channel
            
    //vector<int> t;
    //vector<int> v_p;
    //vector<int> v_place;
    
    //int ar[];
};

#endif /* defined(____Sequencer__) */
