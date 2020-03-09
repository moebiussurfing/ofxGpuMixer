#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetFrameRate(25);
    ofSetVerticalSync(false);

	//-

	//1. prepare channels

	//channel 0
    colorBackground.set("Background", ofColor(255,0,0), ofColor(0), ofColor(255));
    colorChannel.setup(colorBackground.getName(), colorBackground.get(), ofGetWidth(),ofGetHeight());

	//channel 1
    fboA.allocate(ofGetWidth(),ofGetHeight());
    
	//channel 2
	fboB.allocate(ofGetWidth(),ofGetHeight());
    //texB = fboB.getTexture();//adding a texture insted of a fbo

	//-

	//2. configure mixer
    mixer.addChannel(colorChannel, ofxGpuMixer::BLEND_ADD);
    mixer.addChannel(fboA,"A ch1", ofxGpuMixer::BLEND_ADD);
    mixer.addChannel(texB,"B ch2", ofxGpuMixer::BLEND_ADD);

	//-

	//3. startup mixer & creates the shader in order to mix.
    mixer.setup(); 
    
	//-
    
    //gui
    gui.setup( mixer.getParameterGroup() );
	
    //settings
    params_mixerSettings.add(colorBackground);
    params_mixerSettings.add(mixer.getParameterGroup());

	//load
    loadParams(params_mixerSettings, path_mixerSettings);
}

//--------------------------------------------------------------
void ofApp::update()
{
    fboA.begin();
    {
        ofClear(0,255);
        scene.drawChannel1();
    }
    fboA.end();
    
    fboB.begin();
    {
		ofClear(0, 255);
		scene.drawChannel2();
    }
    fboB.end();
    
    mixer.update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofBackground(0);
    //ofSetColor(255);
    
	mixer.drawFbo(0, 0, ofGetWidth(), ofGetHeight());

    gui.draw();
}

//--------------------------------------------------------------
void ofApp::exit()
{
    saveParams(params_mixerSettings, path_mixerSettings);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
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
	////BUG:
	//fboA.allocate(w, h);
	//fboB.allocate(w, h);
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
