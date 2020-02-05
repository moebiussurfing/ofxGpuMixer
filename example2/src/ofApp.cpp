#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetFrameRate(25);
    ofSetVerticalSync(false);

    colorBackground.set("Background", ofColor(255,0,0), ofColor(0), ofColor(255));

    fboA.allocate(ofGetWidth(),ofGetHeight());
    fboB.allocate(ofGetWidth(),ofGetHeight());
    texB = fboB.getTexture(); // adding a texture insted of a fbo
    colorChannel.setup(colorBackground.getName(), colorBackground.get(), ofGetWidth(),ofGetHeight());

    mixer.addChannel(colorChannel, ofxGpuMixer::BLEND_ADD);
    mixer.addChannel(fboA,"A", ofxGpuMixer::BLEND_ADD);
    mixer.addChannel(texB,"B", ofxGpuMixer::BLEND_ADD);

    mixer.setup(); // Creates the shader in order to mix.
    
    
    // GUI
    gui.setup( mixer.getParameterGroup() );


    //settings
    params_mixerSettings.add(mixer.getParameterGroup());
    params_mixerSettings.add(colorBackground);
    loadParams(params_mixerSettings, path_mixerSettings);
}

//--------------------------------------------------------------
void ofApp::update(){
    int s = ofGetHeight()/3;
    fboA.begin();
    {
        ofBackground(0,255);

        ofSetColor(255);
        //ofDrawRectangle(ofGetWidth()/2-s-100, ofGetHeight()/2-s, s*2, s*2);

        scene.drawChannel1();
    }
    fboA.end();
    
    fboB.begin();
    {
        ofBackground(0,0,0);

        ofSetColor(255);
        //ofDrawCircle(ofGetWidth()/2, ofGetHeight()/2, s);

        scene.drawChannel2();
    }
    fboB.end();
    
    mixer.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    ofSetColor(255);
    mixer.draw(0,0,ofGetWidth(), ofGetHeight());
    
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::exit(){
    saveParams(params_mixerSettings, path_mixerSettings);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    //scene
    if (key == 's')
    {
        scene.setGuiVisible(!scene.getGuiVisible());
    }
    else if (key == 'b')
    {
        scene.setModeColorsToggle();
    }
    else if (key == 'r')
    {
        scene.setModeRandomizeToggle();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}


//--------------------------------------------------------------
void ofApp::loadParams(ofParameterGroup &g, string path)
{
    ofLogNotice("ofApp") << "loadParams: " << path;
    ofXml settings;
    settings.load(path);
    ofDeserialize(settings, g);
}

//--------------------------------------------------------------
void ofApp::saveParams(ofParameterGroup &g, string path)
{
    ofLogNotice("ofApp") << "saveParams: " << path;
    ofXml settings;
    ofSerialize(settings, g);
    settings.save(path);
}
