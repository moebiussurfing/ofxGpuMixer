#pragma once

OFX_GPUMIXER_BEGIN_NAMESPACE

class BasicChannel{
public:
    ofFbo fbo;
    ofParameterGroup parameterGroup;
    string name;
	    
    BasicChannel(){};
    
    virtual void setup(){};
    virtual void update(){};
    
    virtual void setupParameterGroup(string name){}
    
    void draw(){
        fbo.draw(0,0);
    }
    void draw(int x, int y){
        fbo.draw(x, y);
    }
    void draw(int x, int y, int w, int h){
        fbo.draw(x,y,w,h);
    }
    
    ofTexture& getTexture(){ return fbo.getTexture(); }
    ofFbo& getFbo(){ return fbo; }
    ofParameterGroup& getParameterGroup(){ return parameterGroup; }
    string& getName(){ return name; }
    
private:
    
};



OFX_GPUMIXER_END_NAMESPACE
