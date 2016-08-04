//  ofApp.cpp

#include "ofApp.h"
#include <iostream>

//--------------------------------------------------------------
void ofApp::setup(){

    //midiOut.openPort(0);
    
    //sleep(5);
    //ofSleepMillis(1000);//seems to make it a bit more stable?
    
    ofSetFrameRate(45); // 1 frame : 1 tick
    //ofSetFrameRate(70);
    //seq.setup();
    blues.setup();
    OSC.setup();
    
    //ofSleepMillis(1000);
    
    //cout << "set up" << endl << endl << endl;
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
}


void ofApp::draw(){

    //ofColor(0);
    ofBackground(0);
    ofDrawBitmapString("ending: " + ofToString(blues.ending) + "\n" + "goal_reached: " + ofToString(blues.goal_reached), 10, 10);
}

void ofApp::keyPressed(int key){

    if (key == 'e') blues.ending = 1;
    if (key == 'r') blues.ending = 0;
    if (key == 'g') blues.goal_reached = 1;
}

void ofApp::keyReleased(int key){}