#include "ofMain.h"
#include "ofApp.h"


//======================
//#include "ofAppGLFWWindow.h"


//======================

//void stop_all_midi_2();

//========================================================================
int main( ){
	ofSetupOpenGL(575,750,OF_WINDOW);		// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());
    
    //glfwSetWindowCloseCallback(OF_WINDOW, stop_all_midi_2());
    
    //get variable from ofAppGLFWWindow.h
}

/*
void stop_all_midi_2(){

    cout << "stopping midi..!!" << endl;
}
*/