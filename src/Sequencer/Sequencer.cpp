//
//  Sequencer.cpp
//
#include "Sequencer.h"
/*
Sequencer::Sequencer(Sequencer const& other){

    vector<int> timers = new vector<int>(other.timer);
}
*/


void Sequencer::setup(){
    
    midiOut.openPort(0);
    QN_subdivision = 3; // i.e. 16th triplets
    //QN_subdivision = 4; //i.e. 16th note
    tick = 0;
    semiquaver = 0; //semiquaver notes..
    beat = 0;
    bar = 0;
    cycle = 0;
    detach = 6; //detaching by 1/8 of semiquaver
    
    incr = 0; //to increment chord pitches
    metre = 4; //set the time signature (QNs for now..)
    cycle_len = 12;
    pitch = 50;
    metro_on = 1;
    chord_ch = 1;
    metro_ch = 2;
    
}


void Sequencer::update(){
    //vector<int> _t = timer();
    //needs clearing? (t[]) // delete[] t??
    //metronome(_t, metro_on, metre, true, true, {3,2});
    //play_chord_sequence(t);
/*
    //time(r) debug
    for (int i=0; i<t.size(); i++){
        cout << t[i] << " ";
    }
    cout << endl;
*/
}


//seq.time_placement(t, get_chord(), t, 4);

void Sequencer::time_placement(vector<int>& t, vector<int>& notes, vector<int>& pos, int dur, int ch){//dur in semiq (QN_subdivision..)
    //for "notes", cannot accept get_chord by reference..
    
    //debug
    /*
    cout << "timer (time_placement()): ";
    for (int i=0; i<t.size(); i++){
        cout << t[i] << " ";
    }
    cout << endl;
    */
    
    //dur: note/chord duration in 16ths..
    int _semiq, _beat, _bar, _cycle;
    _semiq = pos[1];//t[1];
    _beat = pos[2];//t[2];
    _bar = pos[3];//t[3];
    //needs clearing? pos[]
    
    //ON (t[0] is tick)
    if (t[0] == 0 && t[1] == pos[1] && t[2] == pos[2] && t[3] == pos[3]){
        
        //cout << endl << endl << "ON: " << t[0] << " " << t[1] << " " << t[2] << " " << t[3] << endl << endl;
        chord_on_off(1, notes, ch);
        
        if (only_on("bar", t)) {
            
            //chord_on_off(1, notes, ch);
            //cout << "ON: " << pos[0]<<" " << pos[1]<<" " << pos[2]<<" "<<endl;
            //cout << "ON" << endl;
        }
    }
    //translate dur to position in bar
    int new_semiq = _semiq+dur-1;// "-1" for detaching
    if (detach == 0) new_semiq += 1;
    int new_beat = _beat + int(new_semiq / QN_subdivision);
    int new_bar = _bar + int(new_beat / metre);
    //needs clearing? new_*
    _semiq = new_semiq % QN_subdivision;
    _beat = new_beat % metre;
    _bar = new_bar % cycle_len;
    //need clearing _*?
    
    //cout << endl << endl << "BEFORE OFF: " << _semiq << " " << _beat << " " << _bar << endl << endl;
    
    //OFF (based on dur)
    if (t[0] == detach && t[1] == _semiq && t[2] == _beat && t[3] == _bar){

        //cout << endl << endl << "OFF: " << t[0] << " " << t[1] << " " << t[2] << " " << t[3] << endl << endl;
        
            chord_on_off(0, notes, ch);
        //cout << "OFF: " << _semiq<<" " << _beat<<" " << _bar<<" " << endl;
        //cout << "OFF" << endl;
    }
}

//needless?
int Sequencer::on_off(int pitch, int dur, int vel, int channel){
    midiOut.sendNoteOn(2, pitch+incr, 100);
    return dur*4; //duration (converted from semiquavers to beats)
    //midiOut.sendNoteOff(2, pitch+incr, 100);
}

void Sequencer::chord_on_off(bool on_off, vector<int>& chord, int ch, int vel){//can receive one note too(not chord necessarily)
    if (on_off){
        for (int i=0; i<chord.size(); i++){
            midiOut.sendNoteOn(ch, chord[i], vel);
        }
    }
    else if (!on_off) {
        for (int i=0; i<chord.size(); i++){
            midiOut.sendNoteOff(ch, chord[i]);
        }
    }
}

void Sequencer::stop_all_MIDI(){

    //channels 1 to 3
    for (int i=1; i>4; i++){
    
        //ALL midi notes
        for (int j=0; j<127; j++) midiOut.sendNoteOff(i, j);
    }
}

void Sequencer::metronome(vector<int>& t, bool metro_on, int bar_len, bool bar_click, bool group_on, vector<int> groupings){
    //bar_len not necessary for now..managed by timer
    if (!metro_on){
        return;
    }
    if (t[0]==0){
        midiOut.sendNoteOn(metro_ch, 64, 100);//main click
        if (t[1]==0){
            midiOut.sendNoteOn(metro_ch, 65, 100);//bar click
            //cout << "bar_click!" << endl;
        if (group_on){
            if (t[1]==0) midiOut.sendNoteOn(metro_ch, 66, 100);//grouping click
            if (t[1]==(groupings[0]-1)) midiOut.sendNoteOn(metro_ch, 66, 100);//grouping click
            /*
            for (int i=0; i<groupings.size(); i++){
            
            }
            */
            }
        }
    }
    if (t[0]==2){
        midiOut.sendNoteOff(metro_ch, 64);//actual-=36
        midiOut.sendNoteOff(metro_ch, 65);//actual-=36
    }
}

bool Sequencer::only_on(string division, vector<int>& t){//t for timer()
    
    //normal structure
    if (division=="semiq"){if (t[0]==0) return true; }
    if (division=="beat"){if (t[0]==0 && t[1]==0) return true;}
    if (division=="bar"){if (t[0]==0 && t[1]==0 && t[2]==0) return true;}
    if (division=="cycle"){if (t[0]==0 && t[1]==0 && t[2]==0 && t[3]==0) return true;}
    
    /*
     if (division=="semiq"){if (t[0]==0) return true;}
     if (division=="beat"){if (only_on("semiq", t) && t[1]==0) return true;}
     if (division=="bar"){if (only_on("beat", t) && t[2]==0) return true;}
     if (division=="cycle"){if (only_on("bar", t) && t[3]==0) return true;}
    */
    
    //debug structure
    /*
    if (division=="semiq"){
        if (t[0]==0){
            for (int i=0; i<t.size(); i++){
                cout << t[i] << " ";
            }
            cout << "semiq" << endl;
            return true;
        }
    }
    if (division=="beat"){
        if (t[0]==0 && t[1]==0){
            for (int i=0; i<t.size(); i++){
                cout << t[i] << " ";
            }
            cout << "beat" << endl;
            return true;
        }
    }
    if (division=="bar"){
        if (t[0]==0 && t[1]==0 && t[2]==0){
            for (int i=0; i<t.size(); i++){
                cout << t[i] << " ";
            }
            cout << "bar" << endl;
            return true;
        }
    }
    if (division=="cycle"){
        if (t[0]==0 && t[1]==0 && t[2]==0 && t[3]==0){
            for (int i=0; i<t.size(); i++){
                cout << t[i] << " ";
            }
            cout << "cycle" << endl;
            return true;
        }
    }
    */

    return false;
}

vector<int> Sequencer::timer(){
    //int* Sequencer::timer(){
    //semiquaver is ofSetFrameRate();
    
    vector<int> timers;
    timers.push_back(tick);
    timers.push_back(semiquaver);
    timers.push_back(beat);
    timers.push_back(bar);
    timers.push_back(cycle);
    
    tick++;
    if (tick==8){ // 8 in every semiq
        //play_chord_cycle(t);
        tick = 0;
        semiquaver++;
        if (semiquaver==QN_subdivision){
            semiquaver = 0;
            beat++;
            if (beat==metre){
                beat = 0;
                bar++;
                if (bar == cycle_len){
                    bar = 0;
                    cycle++;
                }
            }
        }
    }
    /*
     for (int i=0; i<timers.size(); i++){
     cout << timers[i] << " ";
     }
     cout << endl;
     */
    //if (timers[1]==0 && timers[0]==0) cout << "metre!" << endl;
    
    return timers;
    //needs_clearing? (timers)
}