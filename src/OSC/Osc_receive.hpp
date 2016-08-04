//
//  Osc_receive.hpp
//
// blues_osc
//
// for git..
//
//  Created by Christodoulos Aspromallis on 13/06/2016.
//
// mercurial test comment

#ifndef Osc_receive_hpp
#define Osc_receive_hpp

//#include <stdio.h>
#include "ofMain.h"
#include "ofxOsc.h"

#define PORT 9000

class Osc_receive{

public:
    void setup();
    void update();

    ofxOscReceiver receive;
    //bool _ending = true;//, goal_reched;
    bool _ending, _goal_reached;
    float oscX = 0.0;
    float oscY = 0.0;

};

#endif /* Osc_receive_hpp */
