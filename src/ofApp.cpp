//  ofApp.cpp

#include "ofApp.h"
#include <iostream>

//--------------------------------------------------------------
void ofApp::setup(){

    //midiOut.openPort(0);
    
    //sleep(5);
    //ofSleepMillis(1000);//seems to make it a bit more stable?
    
    ofSetFrameRate(60); // 1 frame : 1 tick
    //ofSetFrameRate(70);
    //seq.setup();
    blues.setup();
    OSC.setup();
    
    //ofSleepMillis(1000);
    
    //cout << "set up" << endl << endl << endl;
    
    openSans.load("/Users/christodoulosaspromallis/Desktop/UCL/PhD Year 2/Development/OF/of_v0.9.1_osx_release/apps/midiWorkspace/blues_osc_parserUpd/bin/data/open-sans/OpenSans-Regular.ttf", 19.5);
    
    //arrow shape
    arrow.addVertex(0, 0);
    arrow.addVertex(0, 23);
    arrow.addVertex(-8, 15);
    arrow.addVertex(0, 23);
    arrow.addVertex(8, 15);
    arrow.addVertex(0, 23);
}


void ofApp::update(){
    
    //sleep(10); //debug plays only 1 note with this in..(??)
    //ofSleepMillis();//??
    //seq.update();
    
    //stop blues updates when:
    //string s = curr_cycle[t[3]].name;
    //if (goal_reached && (s=="i" || s=="i6" || s=="i7")) break;

    blues.update();
    
    /*
    //updating OSC on every beat only (no more needed for now..)
    if(blues.seq.only_on("beat", blues.t)){
        
        OSC.update();
        blues.ending = OSC._ending;
        blues.goal_reached = OSC._goal_reached;
        //cout << "ending: " << ending << ", g_r: " << goal_reached << endl;
    }
     */
    
    //if (ofApp_is_stopped) blues.seq.stop_all_MIDI();
    
    /*
     # - For compensation - #
     getTimeNow(do this first)
     do actions
     work out how much time elapsed
     sleep for period - elapsed
     ---
     */
    
    //failed try to not alter the lib files..
    /*
    GLFWwindow* = w_p;
    glfwSetWindowCloseCallback(w_p, stop_all_midi_2());
    
    //glfwCreateWindow
     */
}

/*
 make last chord (if v7 and previous == "fin" be also "fin"
 connect with prediction engine
 make vid
 make pp
 rehearse
 */

void ofApp::draw(){

    //ofColor(0);
    ofBackground(0);
    
    
    //PREDICTION ENGINE STATE FRAME
    ofSetColor(255);
    openSans.drawString("PREDICTION ENGINE STATE:", ofGetWidth()/2 - 157, 30);
    ofSetColor(110);
    ofSetLineWidth(2.5);
    ofNoFill();
    ofDrawRectangle(140, 60, 300, 170);
    
    string p_e_input; //input from prediction engine
    
    if (show_p_e_input){
    
        if (blues.ending) {
        
            ofSetColor(0, 255, 0);
            openSans.drawString("ENDING", 215, 110);
        }
        else if (show_recovery) {
            
            ofSetColor(255, 0, 0);
            openSans.drawString("RECOVERING", 215, 110);
        }
        if (blues.goal_reached) {
            
            ofSetColor(30, 144, 255);
            openSans.drawString("\n\nGOAL_REACHED", 215, 110);
        }
    
    }
    
     openSans.drawString(p_e_input, 215, 110);
    
    
    //CYCLE STATE FRAME
    ofSetColor(255);
    openSans.drawString("CYCLE STATE:", ofGetWidth()/2 - 80, 360);
    ofSetColor(110);
    ofSetLineWidth(2.5);
    ofNoFill();
    ofDrawRectangle(4, 385, 567, 300);
    
    
    //DRAW CYCLE STATES
    string chord_str;
    
    for (int i=0; i<12; i++){
     
        chord_str = blues.parser.curr_cycle[i].name;
        
        //coloured special non-terminals
        if (chord_str=="cad") ofSetColor(0, 255, 0);
        else if (chord_str=="rec") ofSetColor(255, 0, 0);
        else if (chord_str=="fin") ofSetColor(30, 144, 255);
        else ofSetColor(255);
        
        //show "fin" instead of v7 at end..
        if (blues.t[3]!=11 && chord_str == "v7" && i==11){
        
            chord_str = "fin";
            ofSetColor(30, 144, 255);
        }
        
        for (int j=0; j<i/4; j++) chord_str = "\n\n\n" + chord_str;
        openSans.drawString(chord_str, ((i%4)+1)*135-95, 453);
    }
    
    
    //DRAW BARLINES
    ofSetColor(102, 102, 0);
    string barline;
    
    for (int i=0; i<15; i++){
        
        barline = "|";
        
        if (i==0 || i==14) barline = "||";
        
        for (int j=0; j<i/5; j++) barline = "\n\n\n" + barline;
        openSans.drawString (barline, ((i%5)+1)*135-130, 450);
    }
    
    
    //ARROW
    ofSetColor(255, 222, 0);
    ofSetLineWidth(2.5);
    
    //openSans.drawString(ofToString(blues.t[3]), 50, 50);
    
    ofPushMatrix();
    ofTranslate(51, 390);
    
    for (int j=0; j<blues.t[3]/4; j++) ofTranslate(0, 105);
    ofTranslate(((blues.t[3]%4))*133, 0);
    
    arrow.draw();
    ofPopMatrix();
}

void ofApp::keyPressed(int key){

    if (key == 'e' || key == 'E') {
        
        blues.ending = 1;
        show_p_e_input = 1;
    }
    if (key == 'r' || key == 'R') {
     
        blues.ending = 0;
        show_recovery = 1;
    }
    if (key == 'g' || key == 'G'){
     
        blues.goal_reached = 1;
        show_p_e_input = 1;
    }
    if (key == 's' || key == 'S') blues.seq.stop_all_MIDI();
}

void ofApp::keyReleased(int key){}

void ofApp::stop_all_midi_2(){
    
    cout << "stopping midi..!!" << endl;
}