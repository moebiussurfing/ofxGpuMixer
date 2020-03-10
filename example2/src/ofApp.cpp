#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetFrameRate(25);
    ofSetVerticalSync(false);

    //-

    //1. prepare channels

    //channel 0
    colorBackground.set("BACKGROUND", ofColor(0), ofColor(0), ofColor(255));
    colorChannel.setup(colorBackground.getName(), colorBackground.get(), ofGetWidth(),ofGetHeight());

    //channel 1
    fboA.allocate(ofGetWidth(),ofGetHeight());
    
    //channel 2
    fboB.allocate(ofGetWidth(),ofGetHeight());
    texB = fboB.getTexture();//adding a texture insted of a fbo

    //-

    //2. configure mixer
    mixer.addChannel(colorChannel, ofxGpuMixer::BLEND_ADD);
    mixer.addChannel(fboA,"CHANNEL 1", ofxGpuMixer::BLEND_ADD);
    mixer.addChannel(texB,"CHANNEL 2", ofxGpuMixer::BLEND_ADD);

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
    //TODO:
    //crashes
    //    loadParams(params_mixerSettings, path_mixerSettings);

    refreshGui_Mixer();
}

//--------------------------------------------------------------
void ofApp::update()
{
    fboA.begin();
    {
        ofClear(0, 255);
        scene.drawChannel1();
    }
    fboA.end();
    
    fboB.begin();
    {
        ofClear(0, 255);
        scene.drawChannel2();
    }
    fboB.end();

    //-

    mixer.update();

    //easy callback
    if (mixer.isUpdated())
    {
        refreshGui_Mixer();
    }

    if (mixer.isChangedColor())
    {
        colorBackground = mixer.getColorChannel0();
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofBackground(0);
    
    mixer.drawFbo(0, 0, ofGetWidth(), ofGetHeight());//to v flip
                                                     //    mixer.draw(0, 0, ofGetWidth(), ofGetHeight());

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
    //scene
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

    //-

    if (key == OF_KEY_RIGHT)
    {
        if (mixer.channelSelect.get() != 0)//except channel 0 background
        {
            if (mixer.getBlendMode() >= mixer.getLastBlendMode())
            {
                mixer.setBlendMode(1);//first blendMode
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
            //                        mixer.setColorChannel0(colorBackground.get());
        }
    }
    else if (key == OF_KEY_LEFT)//block channel 0 background
    {
        if (mixer.channelSelect.get() != 0)
        {
            if (mixer.getBlendMode() <= 1)//first blendMode
            {
                mixer.setBlendMode(mixer.getLastBlendMode());
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
            //                        mixer.setColorChannel0(colorBackground.get());
        }
    }


    //select channel
    if (key == OF_KEY_UP)
    {
        if (mixer.channelSelect.get() <= 0)
        {
            mixer.selectChannel(0);
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


    else if (key == OF_KEY_RETURN)
    {
        refreshGui_Mixer();
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


//--------------------------------------------------------------
void ofApp::refreshGui_Mixer()
{
    ofLogNotice("ofApp") << "refreshGui_Mixer()";

    //collapse groups

    //internal

//    //mixer gui
////#ifdef INCLUDE_MIXER_MODE
//    //gui.minimizeAll();
//    //auto gMixer0 = gui.getGroup("MIXER SETTINGS");
//    //auto gMixer = gMixer0.getGroup("MIXER");
//    auto &gMixer = gui.getGroup("MIXER");
//    //gMixer.minimize();
//
//    auto &gCh0 = gMixer.getGroup("BACKGROUND");
//    auto &gCh1 = gMixer.getGroup("CHANNEL 1");
//    auto &gCh2 = gMixer.getGroup("CHANNEL 2");
//    auto &gPrv = gMixer.getGroup("PREVIEW");


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
//#endif

    //--

//    //blend and mask gui pannel (joined)
//    auto &gInternal = gui_Control.getGroup("INTERNAL");//1st level
//                                                       //gInternal.minimize();
//    auto &gGuiPos = gInternal.getGroup("GUI POSITION");//2nd level
//    gGuiPos.minimize();
//
//    //addon settings
//    auto &gSettings = gui_Control.getGroup("SETTINGS");//1st level
//
//    //debug
//    auto &gDebug = gSettings.getGroup("BACKGROUNDS");//2nd level
//    gDebug.minimize();
//
//    //modes
//    auto &gBlend = gSettings.getGroup("BLEND");//2nd level
//                                               //auto &gMask = gSettings.getGroup("MASK");//2nd level

//if (ENABLE_MIXER)
//    {
//        gBlend.minimize();
//        //gMask.minimize();
//#ifdef INCLUDE_MIXER_MODE
//        //gMixer.maximize();
//#endif
//    }

    //-

//    //workflow: set a default mode if all are disabled
//    if (!ENABLE_BLEND && !ENABLE_MASK && !ENABLE_MIXER)
//    {
//        MODE_AppMixer = 2;
//    }
}

