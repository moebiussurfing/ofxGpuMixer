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
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofxPanel gui;

	//-
    
    ofxGpuMixer::Mixer mixer;
    ofxGpuMixer::SimpleColorChannel colorChannel;

    ofFbo fboA;
    ofFbo fboB;
    //ofTexture texB;
    
    void refreshGui_Mixer();
        
    ofParameter<ofColor> colorBackground;
    
    //-
    
    void loadParams(ofParameterGroup &g, string path);
    void saveParams(ofParameterGroup &g, string path);

    ofParameterGroup params_mixerSettings{"mixerSettings"};
    string path_mixerSettings = "mixerSettings.xml";
    
	//-

    ofxSceneTEST scene;
};
