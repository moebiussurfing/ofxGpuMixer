#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);

	//-

	//1. prepare channels

	//channel 0 (background for tint)
	colorBackground.set("BACKGROUND", ofColor(0, 0, 0), ofColor(0, 0, 0), ofColor(255, 255, 255));
	colorChannel.setup(colorBackground.getName(), colorBackground.get(), ofGetWidth(), ofGetHeight());

	//channel 1
	fbo1.allocate(ofGetWidth(), ofGetHeight());

	//channel 2
	fbo2.allocate(ofGetWidth(), ofGetHeight());
	//this is an alternative mode using textures insted ofFbo. not much tested but should work..
	//texB = fbo2.getTexture();//adding a texture insted of a fbo

	//-

	//2. configure mixer
	mixer.addChannel(colorChannel, ofxGpuMixer::BLEND_ADD);
	mixer.addChannel(fbo1, "CHANNEL 1", ofxGpuMixer::BLEND_ADD);
	mixer.addChannel(fbo2, "CHANNEL 2", ofxGpuMixer::BLEND_ADD);
	//mixer.addChannel(texB, "CHANNEL 3", ofxGpuMixer::BLEND_ADD);//alternative mode using textures instead of fbo

	//-

	//3. startup mixer & creates the shader in order to mix.
	mixer.setup();

	//-

	//gui

	//A. a simple gui build using main group of parameters
	gui.setup(mixer.getParameterGroup());

	////B. a custom gui
	////this is an example of a customized gui panel for this app
	////that's why we add the used/desired channels to our gui
	//gui.setup("MIXER");
	//gui.add(mixer.getParameterGroupPreview());
	//gui.add(mixer.getParameterGroupChannel(0));
	//gui.add(mixer.getParameterGroupChannel(1));
	//gui.add(mixer.getParameterGroupChannel(2));

	//-

	//settings
	//we handle the settings as usual in OF using an ofParameterGroup
	params_mixerSettings.add(colorBackground);//ch0 bg
	params_mixerSettings.add(mixer.getParameterGroup());//all params together
	
	//----

	//startup

	mixer.reset();//kind of startup reset

	//load settings
	loadParams(params_mixerSettings, path_mixerSettings);

	//sub panels are interactive to collapse
	refreshGui_Mixer();

	//our scene testing
	scene.setText1("testing");
	scene.setText2("ofxGpuMixer");

	//----
}

//--------------------------------------------------------------
void ofApp::update()
{
	//feed layer channel 1

	fbo1.begin();
	{
		ofClear(0, 255);
		scene.drawChannel1();
	}
	fbo1.end();

	//-

	//feed layer channel 2

	fbo2.begin();
	{
		ofClear(0, 255);
		scene.drawChannel2();
	}
	fbo2.end();

	//-

	mixer.update();

	//-

	//easy callbacks to make gui interactive

	if (mixer.isChangedSelector())
	{
		refreshGui_Mixer();
	}
	//background color changed
	if (mixer.isChangedColor())
	{
		colorBackground = mixer.getColorChannel0();
	}
	//if (mixer.isUpdated())
	//{
	//	refreshGui_Mixer();
	//}
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofBackground(0);

	mixer.drawFbo(0, 0, ofGetWidth(), ofGetHeight());

	//-

	gui.draw();
}

//--------------------------------------------------------------
void ofApp::exit()
{
	saveParams(params_mixerSettings, path_mixerSettings);
}

//keys
//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	//--

	//mixer
	//a browsing gui system to navigate sub menus
	
	if (key == OF_KEY_RIGHT)
	{
		if (mixer.channelSelect.get() != 0)//except channel 0 background
		{
			if (mixer.getBlendMode() >= mixer.getLastBlendMode())
			{
				mixer.setBlendMode(mixer.getLastBlendMode());//blocked
				//mixer.setBlendMode(1);//cycled. first blendMode
			}
			else
			{
				mixer.setBlendMode(mixer.getBlendMode() + 1);
			}
		}
		else//browse bg hue
		{
			ofColor c = colorBackground.get();
			int bgH = c.getHue();
			bgH += 5;
			if (bgH > 255)
				bgH = 255;
			c.setHue(bgH);
			colorBackground = c;
			mixer.setColorChannel0(colorBackground.get());
		}
	}
	else if (key == OF_KEY_LEFT)//block channel 0 background
	{
		if (mixer.channelSelect.get() != 0)
		{
			if (mixer.getBlendMode() <= 1)//first blendMode
			{
				mixer.setBlendMode(1);//blocked
				//mixer.setBlendMode(mixer.getLastBlendMode());//cycled
			}
			else
			{
				mixer.setBlendMode(mixer.getBlendMode() - 1);
			}
		}
		else//browse bg hue
		{
			ofColor c = colorBackground.get();
			int bgH = c.getHue();
			bgH -= 5;
			if (bgH < 0)
				bgH = 0;
			c.setHue(bgH);
			colorBackground = c;
			mixer.setColorChannel0(colorBackground.get());
		}
	}

	//select channel
	if (key == OF_KEY_UP)
	{
		if (mixer.channelSelect.get() <= 0)
		{
			mixer.selectChannel(0);//blocked
			//mixer.selectChannel(mixer.getLastChannel());//cycled
		}
		else
		{
			mixer.selectChannel(mixer.channelSelect.get() - 1);
		}
	}
	else if (key == OF_KEY_DOWN)
	{
		if (mixer.channelSelect.get() >= mixer.getLastChannel())
		{
			mixer.selectChannel(mixer.getLastChannel());
			//mixer.selectChannel(0);//cycle
		}
		else
		{
			mixer.selectChannel(mixer.channelSelect.get() + 1);
		}
	}

	//solo
	else if (key == 's')
	{
		mixer.toggleSolo();
	}

	//--

	//scene
	//scene.keyPressed(key);
	if (key == 'S')
	{
		scene.setGuiVisible(!scene.getGuiVisible());
	}
	else if (key == 'B')
	{
		scene.setModeColorsToggle();
	}
	else if (key == 'R')
	{
		scene.setModeRandomizeToggle();
	}
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	//TODO:
	//BUG:
	//we can't resize fbo's yet! it freezes contents
	//fbo1.allocate(w, h);
	//fbo2.allocate(w, h);

	mixer.fboResize(w, h);
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

//--------------------------------------------------------------
void ofApp::refreshGui_Mixer()
{
	ofLogVerbose("ofApp") << "refreshGui_Mixer()";

	auto &gCh0 = gui.getGroup("BACKGROUND");
	auto &gCh1 = gui.getGroup("CHANNEL 1");
	auto &gCh2 = gui.getGroup("CHANNEL 2");
	auto &gPrv = gui.getGroup("PREVIEW");

	gCh0.minimize();
	gCh1.minimize();
	gCh2.minimize();
	gPrv.minimize();

	gCh1.getGroup("TINT").minimize();
	gCh2.getGroup("TINT").minimize();

	gPrv.maximize();

	switch (mixer.channelSelect.get())
	{
	case 0:
		gCh0.maximize();
		break;
	case 1:
		gCh1.maximize();
		break;
	case 2:
		gCh2.maximize();
		break;
	default:
		break;
	}
}

