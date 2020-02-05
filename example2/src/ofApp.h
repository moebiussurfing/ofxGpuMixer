#pragma once

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
    
    ofxGpuMixer::Mixer mixer;
    
    ofFbo fboA;
    ofFbo fboB;
    ofTexture texB;
    
    ofxGpuMixer::SimpleColorChannel colorChannel;
    
    //-
    
    ofxSceneTEST scene;
    
    void loadParams(ofParameterGroup &g, string path);
    void saveParams(ofParameterGroup &g, string path);
    
    ofParameter<ofColor> colorBackground;
    ofParameterGroup params_mixerSettings{"mixerSettings"};
    string path_mixerSettings = "mixerSettings.xml";
};