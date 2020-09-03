#pragma once

#include "BasicChannel.h"
//#include "ofxAutoReloadedShader.h"

OFX_GPUMIXER_BEGIN_NAMESPACE

//--------------------------------------------------------------
class SimpleColorChannel: public BasicChannel
{
public:

    ofParameter<ofColor> color;
    
    SimpleColorChannel(){}
    
	//--------------------------------------------------------------
	void setup(string channelName, ofColor color, int width, int height)
	{
        this->name = channelName;
        
        fbo.allocate(width, height, GL_RGBA32F_ARB);
        fbo.begin();
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ofBackground(color);
        fbo.end();
        
        setupParameterGroup(name, color);
    } 
    
	//--------------------------------------------------------------
	void update()
	{
        fbo.begin();
        {
            glClearColor(0.0, 0.0, 0.0, 0.0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            ofBackground(color);
        }
        fbo.end();
    }
    
	//--------------------------------------------------------------
	void setupParameterGroup(string name, ofColor color)
	{
            parameterGroup.add(this->color.set("COLOR", color, ofColor::black, ofColor::white));
    }
    
private:
    bool hasMappingImg = false;
};


OFX_GPUMIXER_END_NAMESPACE
