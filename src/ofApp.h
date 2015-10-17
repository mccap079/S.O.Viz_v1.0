#pragma once

#include "ofMain.h"

//addons
#include "ofxJSON.h"
#include "ofxUI.h"
#include "ofxTileSaver.h"
#include "ofxTween.h"
//#include "ofxSSL.h"
//#include "ofxCurl.h"

//classes
#include "nameIndex.h"
#include "Labels.h"
#include "arcs_1.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button, int i);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void guiEvent(ofxUIEventArgs &e);
    
    nameIndex _nameIndex;
    vector <Labels*> _labels;
    Arcs_1 _arcs;
    
//    ofxSSL curl;
//    ofxCurl curl2;
    ofxJSONElement result;
    ofxUISuperCanvas *gui;
    ofxTileSaver saver;
    ofxTween tweenOpen;
    ofxEasingLinear easingLinear;
    ofTrueTypeFont DINReg, DINMed, DINBlaM, DINBlaL, DINBlaXL;
    
    vector <string> satNames, launchDates, firstLaunchDates;
    vector <string> zoomLevel, viewDataset;
    vector <ofRectangle> showNamesButtons;
    vector <bool> showNames;
    vector <bool> startZoom, isZoomedAtLevel;
    vector <bool> viewDatasets, isWithinHoveredArc;
    vector <double> apogees, perigees;
    vector <float> dateAngles;
    
    float width, height;
    float earthRadius, moonApogeeDist, moonPerigeeDist, heoDist, meoDist, scale, earthXpos, framerateMult;
    float speed;
    float perigeeYpos, apogeeYpos;
    float nameOffsetX, nameOffsetY;
    float startingXPos;
    double angle, angleAdder;
    
    int r, decadeCounter, hoverDateStart, hoverDateEnd;
    float numResults;
    
    string frameRate, launchYear, name;
    string earthRadTxt, leo, meo, heo, moonApogee, moonPerigee;
    
    bool dataFetched, dataAllocated, showEarth, showLabels;
    bool once, buttonHovered;
    bool isFirstYear, isNewDecade, isLastYear;
    
    ofPoint center;
    ofPoint mousePos;
    ofColor bgColor;
    
    ofMesh mesh;
    ofVbo dataVbo;
    
    void JsonInit(bool _isJsonMade, bool _areVarsInitiated);
    bool isJsonMade, areVarsInitiated;
		
};
