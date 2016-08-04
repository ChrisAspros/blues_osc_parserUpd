//  ofApp.h

#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
//#include "ofxMidiOut.h" //??
#include "Blues_structure.h"//??
//#include "Sequencer.h"
#include "Osc_receive.hpp"

class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();
    void keyPressed(int key);
    void keyReleased(int key);
    
    int channel, note, velocity;

private:
    //Sequencer seq;
    Blues_structure blues;
    Osc_receive OSC;
    //Sequencer seq;
    
    //bool ending, goal_reached;
    
};

/*
 >    (- get everything more tidy and functional with the correct use of
 >classes etc.)
 >    - give some rhythmical placement variety for the chords (based on
 >style), rather than play only at beginning/middle of bars.
 >    - probabilistically generate some chords and versions of others - in
 >a context-sensitive probabilistic grammar manner, the way I understand it
 >so far..
 >    - give it some instructions regarding all potential "time placements"
 >of transition prediction, i.e. at each beat of every bar (?). I was
 >thinking of cadencing it to all 12 tonal centres, based on chord
 >similarity (which I have not decided how to model yet).
 >    - make some melodies on top (even though this will require more
 >time). I will look again at how Bob Keller did it with the Impro-Visor
 >(they used CFPG for the melodies).
*/
/*
 == Sequencer-->time_placement-->get_chord()..?
 == to sequencer (t) na trexei ksexwrista? h ola apo blues update()?
 == sto blues ta chords (get_chord() klp) kai to grammar
 == pws 8a steilw ti sto grammar (time in)
 == ti 8a steilw eksw apto grammar
 == mh parafortwsw arguments sto time placement (arg. get_chord()?)
 == grammar (3 styles) --> is it pcsg or HMM (http://conf.ling.cornell.edu/kgrove/mcfgcky/mcfgcky002.html )
        http://www.dam.brown.edu/people/geman/Homepage/Computational%20linguistics/Encyclopedia-grammars.pdf
        https://www.quora.com/Natural-Language-Processing/What-is-Probabilistic-Context-Free-Grammar-PCFG
    - cycle start: decide /populate chords (terminals)
        - or maybe decide on every step (or every 4 bars etc..) based on grammatical rules (so that workload is spread)
        - initially give it some time to think..??
    - each cycle should be aware of the previous one or two (should not be too different, despite same grammar)
        ((encode in larger (wider) grammar..?!?))
    - context-sensitive micro-grammar for transition..!
 == rest chords (2 styles)
 == connect to python
    == ofx in
    - https://www.youtube.com/watch?v=TczI-tSOIpY
    - https://github.com/mrichardson23/ofxOscReceive
    - https://github.com/hideyukisaito/ofxThreadedOSCReceiver
    - https://github.com/hideyukisaito/ofxOsc
    == python / eclipse?
    - http://opensoundcontrol.org/implementation/python-simple-osc
    - http://muio.org/interface/software/of
 == model stoppages (transitions)
    - (grammar?) expanding a tree to the transition, based on current rules, harmonic rhythm etc.. so that it feels non-disruptive..
 == implement prediction in c++..??
 == //needs clearing?
    if use new, use delete to avoid memory leaks
 == make (some) privates
 == rhythmic misplacment (or connection / rhythmic re-triggering if chord repeated)
    - intervene between choice of chords and time_placement()
 == voice leading
    - apply voicing (chord structure) rules
    - grammar make 
 == melody
 == r/s..
 == insert timeline?? https://github.com/YCAMInterlab/ofxTimeline
*/

/*
 DETACHED==
 TO SEPERATE NOTES:
 http://www.music-software-development.com/midi-tutorial.html
 https://www.cs.cmu.edu/~music/cmsip/readings/MIDI%20tutorial%20for%20programmers.html
 http://www.juce.com/forum/topic/creating-midi-some-basic-questions
 http://stackoverflow.com/questions/5612358/audio-midi-c-library-for-a-real-time-application
 http://wiki.linuxaudio.org/apps/categories/midi_software
 ========================================================================================================================
 https://www.google.co.uk/search?client=safari&rls=en&q=connected+midi+notes+click&ie=UTF-8&oe=UTF-8&gfe_rd=cr&ei=Z4nlVq-pAZCx8weH_rOQBA#q=program+consecutive+midi+notes+gap
 http://music.stackexchange.com/questions/16677/two-consecutive-midi-note-on-of-the-same-pitch-misbehaving
 https://www.google.ch/patents/US20090227200
 http://www.fonema.se/organ/control/midi.htm
 http://homepages.inf.ed.ac.uk/s1331854/pdf/VoiceSeparation.pdf
 http://www.gvst.co.uk/gsnap_manual.htm
 http://www.juce.com/forum/topic/creating-midi-some-basic-questions
 https://moodle.ucl.ac.uk/pluginfile.php/2716593/mod_resource/content/6/11.%20Music%20Representation%20Complete.pdf
 */