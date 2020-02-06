#pragma once

#define STRINGIFY(x) #x
#include "MixingShaderSnipets.h"
#include "SimpleColorChannel.h"
#include "Constants.h"

OFX_GPUMIXER_BEGIN_NAMESPACE


class TextureGroup {
public:

	ofParameter<float> hue{ "Hue",0.5, 0, 1. };
	ofParameter<float> saturation{ "Saturation",1, 0, 1. };
	ofParameter<float> brightness{ "Brightness",1, 0, 1. };
	ofParameter<float> tintAmt{ "TintAmt", 0, 0., 1 };
	ofParameter<float> contrast{ "Contrast", 1., 0., 2. };
	ofParameter<float> gain{ "Gain", 1.0, 1., 5. };
	ofParameter<float> opacity{ "Opacity", 1., 0., 1. };
	ofParameter<int> blendMode{ "blendMode", 1, 1, PASS_THROUGH };
	ofParameter<string> blendModeName{ "", "" };//to set blend mode gui name
	int blendMode_PRE;//to avoid use of callback that crashes..

	ofParameterGroup parametersTint{ "Tint", hue, saturation, brightness, tintAmt };
	ofParameterGroup parameters{ "Channel", parametersTint, contrast, gain, opacity, blendMode, blendModeName };

	string name;

	ofTexture texture;

	TextureGroup(string name, int blendMode, ofTexture texture) {
		parameters.setName(name);
		this->blendMode = blendMode;
		this->name = name;
		this->texture = texture;

		this->blendModeName.setSerializable(false);//to not include into xml settings
		this->blendMode_PRE = -1;
	}

	void parametersWithoutBlendMode() {
		parameters.clear();
		parameters.setName(name);
		parameters.add(parametersTint);
		parameters.add(contrast);
		parameters.add(gain);
		parameters.add(opacity);
	}
};

class Mixer {
public:

	vector<BasicChannel*> channels;
	ofParameter<bool> doPreview{ "doPreview", false };
	ofParameter<int> channelSelect{ "channelSelect", 0, 0, 0 };

	ofParameterGroup parameterPreview{ "Preview", doPreview, channelSelect };

	ofFbo fboMix;//to v flip

	//control gui by code
	int getLastChannel()
	{
		return channelSelect.getMax();
	}
	void selectChannel(int _channel)
	{
		if (_channel <= channelSelect.getMax() && _channel >= 0)
			channelSelect = _channel;
	}
	void setBlendMode(int _blendMode)
	{
		if (_blendMode <= texGroups[channelSelect.get()].blendMode.getMax())
			texGroups[channelSelect.get()].blendMode = _blendMode;
	}
	int getLastBlendMode()
	{
		return texGroups[channelSelect.get()].blendMode.getMax();
	}
	int getBlendMode()
	{
		return texGroups[channelSelect.get()].blendMode.get();
	}

	void setup() {

		//ParameterGroup
		parameterGroup.clear();
		parameterGroup.setName("Mixer");

		channelSelect.setMax(texGroups.size() - 1);
		parameterGroup.add(parameterPreview);

		for (int i = 0; i < texGroups.size(); i++) {
			parameterGroup.add(texGroups[i].parameters);
		}

		generateShader();
		generateShaderSingleChannel();

		setupFbo();//to v flip
	}

	void update() {
		//debug
		//int it = 0;
		//int ic = 0;
		for (auto & t : texGroups) {//must add '&' to enable (read and) write too
			for (auto c : channels) {
				if (t.name == c->name && t.opacity > 0) c->update();

				//ofLogNotice("TextureGroup") << "[" << ic << "] c: " << c->name;
				//ic++;
			}

			//to set blend mode gui name
			if (t.blendMode != t.blendMode_PRE)
			{
				//ofLogNotice("TextureGroup") << "[" << it << "] t: "<< t.name;
				//ofLogNotice("TextureGroup") << "[" << it << "] t.blendMode_PRE: " << t.blendMode_PRE;
				//ofLogNotice("TextureGroup") << "[" << it << "] t.blendMode: " << t.blendMode;
				t.blendMode_PRE = t.blendMode.get();
				t.blendModeName = getNameFromBlendMode(t.blendMode);
			}
			//it++;
		}

		//to v flip
		updateFbo();

		//ofLogNotice("TextureGroup") << "";
	}

	void draw(int x, int y, int w, int h) {
		ofPushMatrix();

		if (doPreview) {

			shaderSingleChannel.begin();
			{

				shaderSingleChannel.setUniform2f("iResolution", w, h);
				shaderSingleChannel.setUniform1f("iGlobalTime", ofGetElapsedTimef()); //tempo p nr 1

				int i = channelSelect;
				shaderSingleChannel.setUniformTexture("tex0", texGroups[i].texture, i);
				shaderSingleChannel.setUniform1f("u_H_0", texGroups[i].hue);
				shaderSingleChannel.setUniform1f("u_S_0", texGroups[i].saturation);
				shaderSingleChannel.setUniform1f("u_B_0", texGroups[i].brightness);
				shaderSingleChannel.setUniform1f("u_tintAmt_0", texGroups[i].tintAmt);
				shaderSingleChannel.setUniform1f("u_contrast_0", texGroups[i].contrast);
				shaderSingleChannel.setUniform1f("u_gain_0", texGroups[i].gain);
				shaderSingleChannel.setUniform1f("u_opacity_0", texGroups[i].opacity);
				shaderSingleChannel.setUniform1i("u_blendMode_0", 1);
				shaderSingleChannel.setUniform2f("resolution_0", texGroups[i].texture.getWidth(), texGroups[i].texture.getHeight());


				ofSetColor(255, 255, 255);
				ofFill();
				ofDrawRectangle(0, 0, w, h);
			}
			shaderSingleChannel.end();

		}
		else
		{
			// ofTranslate(x,y);
			shader.begin();
			{

				shader.setUniform2f("iResolution", w, h);
				shader.setUniform1f("iGlobalTime", ofGetElapsedTimef()); //tempo p nr 1

				for (int i = 0; i < texGroups.size(); i++) {

					shader.setUniformTexture("tex" + ofToString(i), texGroups[i].texture, i);
					shader.setUniform1f("u_H_" + ofToString(i), texGroups[i].hue);
					shader.setUniform1f("u_S_" + ofToString(i), texGroups[i].saturation);
					shader.setUniform1f("u_B_" + ofToString(i), texGroups[i].brightness);
					shader.setUniform1f("u_tintAmt_" + ofToString(i), texGroups[i].tintAmt);
					shader.setUniform1f("u_contrast_" + ofToString(i), texGroups[i].contrast);
					shader.setUniform1f("u_gain_" + ofToString(i), texGroups[i].gain);
					shader.setUniform1f("u_opacity_" + ofToString(i), texGroups[i].opacity);
					shader.setUniform1i("u_blendMode_" + ofToString(i), texGroups[i].blendMode);
					shader.setUniform2f("resolution_" + ofToString(i), texGroups[i].texture.getWidth(), texGroups[i].texture.getHeight());

				}

				ofSetColor(255, 255, 255);
				ofFill();
				ofDrawRectangle(0, 0, w, h);
			}
			shader.end();
		}

		ofPopMatrix();

	}




	void addChannel(ofFbo& fbo, string name, int blendMode) {
		addChannel(fbo.getTexture(), name, blendMode);
	}


	void addChannel(ofTexture texture, string name, int blendMode) {
		TextureGroup texGroup = TextureGroup(name, blendMode, texture);
		if (isFirst) {
			texGroup.parametersWithoutBlendMode();
			texGroup.blendMode = PASS_THROUGH;
			isFirst = false;
		}
		texGroups.push_back(texGroup);
	}

	/*    void addChannel(ShaderChannel & channel, int blendMode){
	 addChannel(channel.getFbo(), channel.getName(), blendMode);
	 channels.push_back(&channel);
	 }
	 */
	void addChannel(SimpleColorChannel &  channel, int blendMode) {
		bool isFirstCol = isFirst;
		addChannel(channel.getFbo(), channel.getName(), blendMode);
		texGroups.back().parameters.clear();
		texGroups.back().parameters.add(channel.color);
		texGroups.back().parameters.add(texGroups.back().opacity);
		if (!isFirstCol) texGroups.back().parameters.add(texGroups.back().blendMode);
		channels.push_back(&channel);
	}



	ofParameterGroup& getParameterGroup() { return parameterGroup; }

	vector<ofParameterGroup*> getVectorOfParameterSubgroups() {

		vector<ofParameterGroup*> paramSubGroups;

		for (int i = 0; i < texGroups.size(); i++) {
			paramSubGroups.push_back(&texGroups[i].parameters);
		}
		return paramSubGroups;
	}

private:
	vector <TextureGroup> texGroups;
	ofShader shader;
	ofShader shaderSingleChannel;

	ofParameterGroup parameterGroup;

	bool isFirst = true;

	void generateShader() {
		// GENERATE THE SHADER
		stringstream shaderScript;
		shaderScript << "#version 120" << endl;
		shaderScript << uniformsHeader;
		for (int i = 0; i < texGroups.size(); i++) {
			string snipet = uniforms;
			ofStringReplace(snipet, "$0", ofToString(i));
			shaderScript << snipet;
		}
		shaderScript << functions;
		shaderScript << mainHeader;
		for (int i = 0; i < texGroups.size(); i++) {
			string snipet = channel;
			ofStringReplace(snipet, "$0", ofToString(i));
			shaderScript << snipet;
		}
		shaderScript << output;

		shader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderScript.str());
		shader.linkProgram();
	}

	void generateShaderSingleChannel() {
		// GENERATE THE SHADER
		stringstream shaderScript;
		shaderScript << "#version 120" << endl;
		shaderScript << uniformsHeader;
		string snipet = uniforms;
		ofStringReplace(snipet, "$0", "0");
		shaderScript << snipet;
		shaderScript << functions;
		shaderScript << mainHeader;
		snipet = channel;
		ofStringReplace(snipet, "$0", "0");
		shaderScript << snipet;
		shaderScript << output;

		shaderSingleChannel.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderScript.str());
		shaderSingleChannel.linkProgram();
	}

	//WORKAROUND: ineficient but avoids the calback crashes.
	//works on every frame...
	//--------------------------------------------------------------
	std::string getNameFromBlendMode(int blendMode)
	{
		std::string s;

		//BLEND_PASS = 0,
		//BLEND_ADD = 1,
		//BLEND_MULTIPLY = 2,
		//BLEND_LIGHTEN = 3,
		//BLEND_DARKEN = 4,
		//BLEND_SUBTRACT = 5,
		//BLEND_SCREEN = 6,
		//BLEND_AVERAGE = 7,
		//BLEND_SOFT_LIGHT = 8,
		//BLEND_OVERLAY = 9,
		//PASS_THROUGH = 10

		switch (blendMode)
		{
		case 0:
			s = "BLEND_PASS";
			break;
		case 1:
			s = "BLEND_ADD";
			break;
		case 2:
			s = "BLEND_MULTIPLY";
			break;
		case 3:
			s = "BLEND_LIGHTEN";
			break;
		case 4:
			s = "BLEND_DARKEN";
			break;
		case 5:
			s = "BLEND_SUBTRACT";
			break;
		case 6:
			s = "BLEND_SCREEN";
			break;
		case 7:
			s = "BLEND_AVERAGE";
			break;
		case 8:
			s = "BLEND_SOFT_LIGHT";
			break;
		case 9:
			s = "BLEND_OVERLAY";
			break;
		case 10:
			s = "PASS_THROUGH";
			break;
		default:
			s = "unknown blendMode";
			ofLogError("TextureGroup") << "unknown blendMode: " << blendMode;
			break;
		}
		ofLogNotice("TextureGroup") << "blendModeName: [" << blendMode << "] " << s;

		return s;
	}


	//WORKAROUND: to flip drawing easy

	//--------------------------------------------------------------
	void setupFbo()
	{
		ofFbo::Settings setting;
		setting.width = ofGetWidth();
		setting.height = ofGetHeight();
		setting.internalformat = GL_RGBA;

		//allocate
		fboMix.allocate(setting);

		//clear
		fboMix.begin();
		ofClear(0, 0, 0, 255);
		fboMix.end();
	}

	//--------------------------------------------------------------
	void updateFbo()
	{
		fboMix.begin();
		ofClear(0, 0, 0, 255);
		draw(0, 0, ofGetWidth(), ofGetHeight());
		//ofClearAlpha();//
		fboMix.end();
	}

public:

	//--------------------------------------------------------------
	void drawFbo()
	{
		fboMix.draw(0, 0, ofGetWidth(), ofGetHeight());
	}
	//--------------------------------------------------------------
	void drawFbo(int x, int y, int w, int h)
	{
		fboMix.draw(x, y, w, h);
	}
	//--------------------------------------------------------------
	void drawFbo(int x, int y)
	{
		fboMix.draw(x, y, ofGetWidth(), ofGetHeight());
	}
};


OFX_GPUMIXER_END_NAMESPACE
