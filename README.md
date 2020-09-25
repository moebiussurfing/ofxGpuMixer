ofxGpuMixer
=============================

# Overview
**ofxGpuMixer** is an **openFrameworks** addon coded by @jonasfehr.  
This is my personal fork with an example and some minimal modifications to fit my needs.  
Thanks *jonasfehr*!  
Original repository:  
https://github.com/jonasfehr/ofxGpuMixer  

## Screenshot
![image](readme_images/Capture.PNG?raw=true "image")

## Features
- Run the example...

## Usage
 
### ofApp.h
```.cpp
#include "ofxGpuMixer.h"

ofxGpuMixer::Mixer mixer;
    
ofFbo fboA;
ofFbo fboB;
//ofTexture texB;
    
//ofxGpuMixer::SimpleColorChannel colorChannel;
```

### ofApp.cpp
```.cpp
ofApp::setup(){
    fboA.allocate(ofGetWidth(),ofGetHeight());
    fboB.allocate(ofGetWidth(),ofGetHeight());
    //texB = fboB.getTexture(); // adding a texture insted of a fbo
    //colorChannel.setup("Background", ofColor(255,0,0), ofGetWidth(),ofGetHeight());

    mixer.addChannel(colorChannel, ofxGpuMixer::BLEND_ADD);
    mixer.addChannel(fboA,"A", ofxGpuMixer::BLEND_ADD);
    mixer.addChannel(fboB,"B", ofxGpuMixer::BLEND_ADD);
    //mixer.addChannel(texB,"B", ofxGpuMixer::BLEND_ADD);

    mixer.setup(); // Creates the shader in order to mix.
}

ofApp::update(){
	 fboA.begin();
    {
        ofBackground(0,255);
        //draw scene channel 1
    }
    fboA.end();
    
    fboB.begin();
    {
        ofBackground(0,0,0);
        //draw scene channel 2
    }
    fboB.end();
    
    mixer.update();
}

ofApp::draw(){
    ofBackground(0);
    ofSetColor(255);
    mixer.draw(0,0,ofGetWidth(), ofGetHeight());
}
```

## Dependencies
https://github.com/moebiussurfing/ofxSceneTEST (only for the example but easy to replace)

## Tested systems
- **Windows10** / **VS2017** / **OF 0.11**

## Author
Addon coded by **jonasfehr**. 
included example and some minimal modifications by **@moebiusSurfing**  
*(ManuMolina). 2020.*

## License
*MIT License.*
