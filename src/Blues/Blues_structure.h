//
//  Blues_structure.h
//  
//
//  Created by Christodoulos Aspromallis on 04/03/2016.
//
//

class Sequencer;
//class G_parser;

#ifndef ____Blues_structure__
#define ____Blues_structure__

#include <stdio.h>
//#include "ofMain.h" // for vector
#include "ofxMidiOut.h" // for vector and midi
#include "Sequencer.h"
#include "Grammar_parser.h"

class Blues_structure{
    
public:
    
    //constructors
    
    //string grammar_path = "/Users/christodoulosaspromallis/Desktop/UCL/PhD Year 2/Development/OF/of_v0.9.1_osx_release/apps/midiWorkspace/blues/parser_project/bin/data/blues_grammar.txt";
    //ifstream grammar();
    
    Blues_structure(){}
    ~Blues_structure(){}
    //constructor making a note bass and a chord would be best..)
    
    //functions
    void setup();
    void update();
    
    void update_state();
    
    void play_drums();
    void play_drums_normal();
    void play_drums_cad();
    void play_drums_fin();
    void play_drums_finale();
    
    void play_bass(vector<int>& chord);
    void play_bass_fin(vector<int>& chord);
    void play_bass_finale(vector<int>& chord);
    
    void play_chords(vector<int>& chord);
    
    void play_main(vector<int>& chord);
    void play_finale(vector<int>& chord);
    
    void time_misplace_chord();
    
    vector<int> get_chord(int root_pitch = 48, int type = 1, int invert = 0, bool position = false);
    // type: 1: maj. triad, 2: m, 3: maj7, 4:maj6, 5: m7, 6: m6, 7: 7
    // inversion: "root", "1st", "2nd", ("3rd", if available)
    // position: 0/1 --> open/closed
    
    vector<int> terminal_to_midi(string& terminal);
    void map_symbol();
    
    Sequencer seq;
    
    G_parser parser;

    //variables
    vector<int> t;
    vector<int> cad_t;
    vector<int> fin_t;
    //vector<int> curr_t;//for time_placement pos
    
    bool ending, goal_reached;
    bool stopping;
    bool cadencing, finishing;
    
    int QN_dur;
    
    int ch_chords;
    int ch_drums;
    int ch_bass;
    
    vector<int> chord;
    vector<int> drum_notes;
    vector<int> notes_v;
    vector<int> pos_drums;
    vector<int> multiple_bars;
    
    map<string, vector<int>> static chord_translation;
    //{rootpitch, type}
    
      
};

#endif /* defined(____Blues_structure__) */
