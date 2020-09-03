#pragma once

//TODO:
//+ split groups gui to allow minimize / workflow

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxGpuMixer.h"

#include "ofxSceneTEST.h"

class ofApp : public ofBaseApp{
    
public:

    void setup();
    void update();
    void draw();
    void exit();
    void keyPressed(int key);
    void windowResized(int w, int h);

    ofxPanel gui;

	//-
    
    ofxGpuMixer::Mixer mixer;
    ofxGpuMixer::SimpleColorChannel colorChannel;

    ofFbo fbo1;
    ofFbo fbo2;
    //ofTexture texB;
    
    void refreshGui_Mixer();
        
    ofParameter<ofColor> colorBackground;//this a local color as example of how to handle externally to the addon
    
    //-
    
    void loadParams(ofParameterGroup &g, string path);
    void saveParams(ofParameterGroup &g, string path);
    ofParameterGroup params_mixerSettings{"mixerSettings"};
    string path_mixerSettings = "mixerSettings.xml";
    
	//-

	//testing scene
    ofxSceneTEST scene;
};
