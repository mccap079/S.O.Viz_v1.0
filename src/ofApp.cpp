
// __________/\/\______/\/\______/\/\____/\/\/\/\/\____/\/\/\/\/\__
// _______/\/\/\/\____/\/\/\__/\/\/\__/\/\__________/\/\__________
// ____/\/\____/\/\__/\/\/\/\/\/\/\__/\/\__________/\/\__________
// ___/\/\/\/\/\/\__/\/\__/\__/\/\__/\/\__________/\/\__________
// __/\/\____/\/\__/\/\______/\/\____/\/\/\/\/\____/\/\/\/\/\__
// ___________________________________________________________
// __________ Copyright (c) 2015 Andrew McCausland __________
// ________________ <andrewmccausland.net> _________________
// ________________________________________________________
// The above copyright notice shall include all copies or
// substantial portions of this software and associated
// documentation files, except for where explicitly stated
// otherwise.

// Built with OpenFrameworks:
// <https://github.com/openframeworks/openFrameworks>,
// <https://github.com/openframeworks/openFrameworks/blob/master/LICENSE.md>.

//test: https://www.space-track.org/basicspacedata/query/class/boxscore/format/json
//full catalog: https://www.space-track.org/basicspacedata/query/class/tle_latest/ORDINAL/1/EPOCH/%3Enow-30/orderby/OBJECT_ID/format/json

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofEnableAntiAliasing(); //smooth
//    glEnable(GL_DEPTH_TEST); //activate 3d
    ofEnableAlphaBlending();
    ofDisableSmoothing();
    mesh.setMode(OF_PRIMITIVE_LINES);
    mesh.enableColors();
    ofSetFrameRate(100);
    ofSetCircleResolution(120);
    framerateMult = 1.0f;
    bgColor = 235;
    showEarth = false;
    dataAllocated = false;
    showLabels = true;
    
    DINBlaM.loadFont("DINBla", 10);
    DINBlaM.setLetterSpacing(1.0);
    DINBlaM.setSpaceSize(0.5);
    
    DINBlaL.loadFont("DINBla", 18);
    DINBlaL.setLetterSpacing(1.0);
    DINBlaL.setSpaceSize(0.5);
    
    DINBlaXL.loadFont("DINBla", 36);
    DINBlaXL.setLetterSpacing(1.0);
    DINBlaXL.setSpaceSize(0.5);
    
    earthRadTxt = "EARTH RADIUS";
    leo = "LEO";
    meo = "MEDIUM EARTH ORBIT";
    heo = "HIGH EARTH ORBIT";
    moonApogee = "MOON APOGEE";
    moonPerigee = "MOON PERIGEE";
    
    isJsonMade, areVarsInitiated = false;
    
    //---------- Setting up UI
    
    gui = new ofxUISuperCanvas("MAPSAT", OFX_UI_FONT_SMALL);
    gui->addSpacer();
    gui->addLabel("Zoom: ", OFX_UI_FONT_SMALL);
    zoomLevel.push_back("Low Earth Orbit");
    zoomLevel.push_back("Medium Earth Orbit");
    zoomLevel.push_back("High Earth Orbit");
    ofxUIRadio *zoomLevelRadio = gui->addRadio("ZOOM", zoomLevel, OFX_UI_ORIENTATION_VERTICAL);
    zoomLevelRadio->activateToggle("High Earth Orbit");
    gui->addSpacer();
    gui->addToggle("Show Labels", true);
    gui->addSpacer();
    gui->addLabel("View: ", OFX_UI_FONT_SMALL);
    viewDataset.push_back("Apogee");
    viewDataset.push_back("Perigee");
    viewDataset.push_back("Both");
    ofxUIRadio *viewDatasetRadio = gui->addRadio("VIEW DATASET", viewDataset, OFX_UI_ORIENTATION_VERTICAL);
    viewDatasetRadio->activateToggle("Apogee");
    gui->addSpacer();
    
    gui->autoSizeToFitWidgets();
    ofAddListener(gui->newGUIEvent,this,&ofApp::guiEvent);
    
    //---------- Tile Saver
    
    saver.init(10, true, 1680, 1000);
    //saver.init(int _steps, bool _bBigImage, float _width, float _height);
    //_bBigImage = if the tiles should be stitched together
    
    //---------- Scale
    
    scale = 570; //6,371 km ... 63.71 for 100px earth radius
    earthRadius = 6371/scale;
    meoDist = 2000;
    heoDist = 35786;
    moonApogeeDist = 405400; //405,504? (src = http://www.timeanddate.com/astronomy/moon/lunar-perigee-apogee.html )
    moonPerigeeDist = 363396;
    earthXpos = (6371/scale)*2;
    speed = 0.5f * scale;
    
    
    //the further out, the faster the speed
    //speed directly proportional to scale
    
    //---------- Satellite names display system setup
    
    startingXPos = 0;
    
    once = false;
    
    startZoom.resize(4);
    isZoomedAtLevel.resize(3);
    viewDatasets.resize(3);
    for(int i = 0; i < 3; i++){
        startZoom[i] = false;
        isZoomedAtLevel[i] = false;
        viewDatasets[i] = false;
    }
    isZoomedAtLevel[2] = true;
    viewDatasets[0] = true;
        
    //---------- Setting up classes
    
    _nameIndex.setup(earthRadius);
    _labels.resize(6);
    for(int i = 0; i < _labels.size(); i++){
        _labels[i] = new Labels();
        _labels[i]->setup(earthRadius, meoDist, heoDist, moonPerigeeDist, moonApogeeDist,DINBlaM, DINBlaL, DINBlaXL, bgColor);
    }

    //_labels[0] = moon apogee
    //_labels[1] = HEO
    //_labels[2] = MEO
    //_labels[3] = LEO
    //_labels[4] = earthRad
    //_labels[5] = arc hover (decade dates)
    
    //---------- Launch date stuff
    
    isFirstYear = true;
    isLastYear = false;
    isNewDecade = true;
    decadeCounter = 0;
    hoverDateStart = 0;
    hoverDateEnd = 0;
    angleAdder = -90;
    
    //angleAdder 0 = 0
    //angleAdder 1 = 0.166744 //1960 //remove this one
    //angleAdder 2 = 30.9667 //1970
    //angleAdder 3 = 90.6134 //1980
    //angleAdder 4 = 145.52 //1990
    //angleAdder 5 = 299.71 //2000
    //angleAdder 6 = 333.869 //2010
    //angleAdder 7 = 360
    
    dateAngles.resize(7);
    firstLaunchDates.resize(7);
    for(int i = 0; i < 7; i++){
        dateAngles[i] = 0;
        firstLaunchDates[i] = "null";
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //var used for any framerate-independent animations
    framerateMult = 60.0f/(1.0f/ofGetLastFrameTime());
    speed = 0.05f * scale;
    
    //visible window params
    width = ofGetWidth();
    height = ofGetHeight();
    center.x = width/2;
    center.y = height/2;
    mousePos.x = ofGetMouseX();
    mousePos.y = ofGetMouseY();
    
    //make sure earthRadius resizes with radio toggle
    earthRadius = 6371/scale;
    earthXpos = (6371/scale)*2;
    
    if (angle >= TWO_PI) {
		angle=0;
	}
    
    //zooming functionality
    if(startZoom[0]){
        if(scale <= 18){
            startZoom[0] = false;
            scale = 18;
        } else {
            scale -= speed * framerateMult;
        }
        
    } else if(startZoom[1]){ //if less than target
        
        if(scale >= 90){
            startZoom[1] = false;
            scale = 90;
        } else {
            scale += speed * framerateMult;
        }
        
    } else if(startZoom[2]){ //if greater than target
    
        if(scale <= 90){
            startZoom[2] = false;
            scale = 90;
        } else {
            scale -= speed * framerateMult;
        }
        
    } else if(startZoom[3]){
        if(scale >= 570){
            startZoom[3] = false;
            scale = 570;
        } else {
            scale += speed * framerateMult;
        }
    }
    
    //more zooming functionality
    if(scale == 18){
        isZoomedAtLevel[0] = true;
        isZoomedAtLevel[1] = false;
        isZoomedAtLevel[2] = false;
    } else if(scale == 90){
        isZoomedAtLevel[0] = false;
        isZoomedAtLevel[1] = true;
        isZoomedAtLevel[2] = false;
    } else if(scale == 570){
        isZoomedAtLevel[0] = false;
        isZoomedAtLevel[1] = false;
        isZoomedAtLevel[2] = true;
    }
    
//    for (int i = 0; i < numResults; i++){
//        if(viewDatasets[2]){
//            ofPushMatrix();{
////                ofTranslate(center);
////                ofVec3f bottom(0.0,-(earthRadius + (apogees[i]/scale)), 0.0);
////                ofVec3f top(0.0,-(earthRadius + (perigees[i]/scale)), 0.0);
//                
////                mesh.addVertex(bottom);
////                mesh.addColor(ofFloatColor(0.0, 0.0, 1.0));
////                mesh.addVertex(top);
////                mesh.addColor(ofFloatColor(1.0, 0.0, 0.0));
//                
//            }ofPopMatrix();
//        }
//    }
    
    for(int i = 0; i < _labels.size();i++){
        _labels[i]->update(scale, earthRadius);
    }
    
    if(dataAllocated){
        if(isZoomedAtLevel[0]){
            _arcs.update(center, mousePos, earthRadius);
        }
    }
    
    mouseMoved(mouseX, mouseY);

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(bgColor);
    
    //---------- Retrieving satellite data from interwebs and saving it to local file as JSON
    
    if(ofGetElapsedTimef() >= 5 && !areVarsInitiated){
//        ofDrawBitmapString(ofToString(status),center.x,200, 1);
        JsonInit(isJsonMade, areVarsInitiated);
        
        cout << "areVarsInitiated = " << areVarsInitiated << endl;
    }
    
    saver.begin();
    
    double apogee;
    double perigee;
    double scaledApogee;
    
    if(areVarsInitiated == true){
        ofPushMatrix();{
            
            ofTranslate(center);
            ofSetRectMode(OF_RECTMODE_CENTER);
            
            for (int i = 0; i < numResults; i++){
                
                if(!dataAllocated){
                    
                    //allocate external data to internal
                    
                    apogees[i] = ofToDouble(result[i]["APOGEE"].asString());
                    perigees[i] = ofToDouble(result[i]["PERIGEE"].asString());
                    launchDates[i] = ofToString(result[i]["OBJECT_ID"].asString());
                
                //add angles to vector dateAngles
                    
                    //i need to store the current angleAdder value at each isNewDecade = true WITH the cooresponding launchDate[i] string
                    //i need to store the launchDate[i] string that cooresponds to each isNewDecade = true loop
                    
                    if(isNewDecade && strncmp(&launchDates[i][3], "0", 1) == 0){ //if "0" for first time
                        
                        if(strncmp(&launchDates[i][0], "1960", 4) != 0){ //skip 1960 separation cuz 1950's are too small
                            
                            angleAdder = -90 + (angle * i);
                            dateAngles[decadeCounter] = angleAdder; //store current angle into our vector
                            firstLaunchDates[decadeCounter] = launchDates[i]; //store the date itself now
                            
                            decadeCounter++;
                            
                        }
                        
                        if(decadeCounter == dateAngles.size() - 1){
                            isLastYear = true;
                        }
                        
                        isNewDecade = false; //make sure this ^ loop doesnt happen twice for this decade
                        
                    } else if(isNewDecade && strncmp(&launchDates[i][3], "0", 1) != 0){
                        
                        if(isFirstYear){
                            dateAngles[decadeCounter] = angleAdder; //store the current angle into our vector
                            firstLaunchDates[decadeCounter] = launchDates[i]; //store the date itself now
                            decadeCounter++;
                            isFirstYear = false;
                        }
                        isNewDecade = false;
                    }
                        
                    if(!isNewDecade){
                        if(strncmp(&launchDates[i][3],"9", 1) == 0){
                            isNewDecade = true;
                        }
                        if(isLastYear){
                            dateAngles[decadeCounter] = 270;
                            isLastYear = false; //not necessary?
                        }
                    }
                    
                    //stop reading from json file when finished looping once
                    if(i == numResults - 1){
                        firstLaunchDates[6] = launchDates[i];
                        cout << "Satellite data allocated internally. No longer reading from JSON file. " << endl;
                        dataAllocated = true;
                    }
                }
                
                ofSetLineWidth(1);
                ofRotate(angle);
                
                //draw red highlights on arc hover
                if(_arcs.isAnyHovering){
                    if( ofToInt(launchDates[i].substr(0,4)) >= hoverDateStart && ofToInt(launchDates[i].substr(0,4)) < hoverDateEnd){
                        ofSetColor(255,0,0,50);
                    } else {
                        ofSetColor(50,20);
                    }
                }else {
                    ofSetColor(50,20);
                }
                
                //draw apogee
                if(viewDatasets[0] || viewDatasets[2]){
                    ofLine(0,-earthRadius,0,-(earthRadius + (apogees[i]/scale)));
                    ofPushStyle();{
                        ofSetColor(255,0,0);
                        ofRect(0,-(earthRadius + (apogees[i]/scale)),1,1);
                    }ofPopStyle();
                }

                //draw perigee
                if(viewDatasets[1] || viewDatasets[2]){
                    ofLine(0,-earthRadius,0,-(earthRadius + (perigees[i]/scale)));
                    ofSetColor(0,255,0);
                    ofRect(0,-(earthRadius + (perigees[i]/scale)),1,1);
                }

                //draw difference highlighter
                if(viewDatasets[2]){
                    ofPushStyle();{
                        ofSetColor(0,0,255);
//                        ofLine(0,-(earthRadius + (apogees[i]/scale)), 0,-(earthRadius + (perigees[i]/scale)));
                        
//                        mesh.draw(); 
                    }ofPopStyle();
                }
                
            }
        }ofPopMatrix();
    }

    saver.end();
    
    //draw earth orbital zones
    ofPushStyle();{
        
        ofFill();
        ofSetRectMode(OF_RECTMODE_CENTER);
        
        //draw orbit zones
        ofSetColor(20,50);
        ofCircle(center.x,center.y,earthRadius + (meoDist/scale)); //2,000km = end of LEO
        ofCircle(center.x,center.y,earthRadius + (heoDist/scale)); //35,786km = end of MEO
        ofSetColor(50);
        ofNoFill();
        ofCircle(center.x,center.y,earthRadius + (moonApogeeDist/scale));//405,400km = moon average apogee (src: https://en.wikipedia.org/wiki/Orbit_of_the_Moon )
        
        //draw date stuff
        if(dataAllocated){
            if(isZoomedAtLevel[0]){
                
                ofPushStyle();{
                    ofSetColor(20,255);
                    for(int i = 0; i < dateAngles.size(); i++){
                        if(i != 0){
                            ofPushMatrix();{
                                ofTranslate(center);
                                _arcs.draw(dateAngles, i);
                            }ofPopMatrix();
                            
                            if(_arcs.isHovering[i]){
                                
                                string date1, date2, id1, id2, to;
                                date1 = ofToString(firstLaunchDates[i-1]).substr(0,4); //1999####
                                date2 = ofToString(firstLaunchDates[i]).substr(0,4); //2000####
                                id1 = ofToString(firstLaunchDates[i-1]).substr(4,8); //####-01A
                                id2 = ofToString(firstLaunchDates[i]).substr(4,8); //####-01B
                                to = " to ";
                                std::stringstream ss;
                                ss << date1;
                                
                                ofRectangle decadeHoverBox = DINBlaXL.getStringBoundingBox(date1 + id1 + to + date2 + id2,
                                                                                           center.x - decadeHoverBox.getWidth()/2,
                                                                                           center.y);
                                
                                _labels[5]->draw(date1 + id1 + to + date2 + id2, 0, 1, decadeHoverBox);
                                
                                hoverDateStart = ofToInt(date1);
                                hoverDateEnd = ofToInt(date2);
                                
                            }
                        }
                    }
                }ofPopStyle();
            }
        }
        
        //draw zone labels bounding box
        
        ofFill();
        ofSetRectMode(OF_RECTMODE_CORNER);
        
        ofRectangle earthRadBox = DINBlaM.getStringBoundingBox(earthRadTxt,
                                                         center.x + earthRadius/2,
                                                         center.y);
        
        ofRectangle leoBox = DINBlaM.getStringBoundingBox(leo,
                                                          center.x + earthRadius + ((meoDist/scale)/2),
                                                          center.y);
        
        ofRectangle meoBox = DINBlaM.getStringBoundingBox(meo,
                                                         center.x + earthRadius + (meoDist/scale + ((heoDist/scale - meoDist/scale)/2)),
                                                         center.y);
        
        ofRectangle heoBox = DINBlaM.getStringBoundingBox(heo,
                                                         center.x + earthRadius + (heoDist/scale + ((moonApogeeDist/scale - heoDist/scale)/2)),
                                                         center.y);
        
        ofRectangle moonApogeeBox = DINBlaM.getStringBoundingBox(moonApogee,
                                                                center.x + earthRadius + (moonApogeeDist/scale),
                                                                center.y);
        ofRectangle moonPerigeeBox = DINBlaM.getStringBoundingBox(moonPerigee, center.x + earthRadius + (moonPerigeeDist), center.y);
        
        ofSetColor(bgColor);

        //draw zone labels
        if(showLabels && !_arcs.isAnyHovering){
            
            ofPushStyle();{
                ofEnableBlendMode(OF_BLENDMODE_SCREEN);
                ofFill();
                ofSetRectMode(OF_RECTMODE_CENTER);
                ofSetColor(200);
                
                //midline stripe
                ofRect(center.x,center.y,width,25);
                
                //hover highlights
                
                
                ofDisableBlendMode();
            }ofPopStyle();
            
            for(int i = 0; i < _labels.size(); i++){
                
            }

            if(isZoomedAtLevel[0]){
                
                //draw earthRad
                _labels[4]->draw(earthRadTxt, -1, -1, earthRadBox);
                
                //draw LEO
                _labels[3]->draw(leo, -1, -1, leoBox);
                
            } else if(isZoomedAtLevel[1]){
                
                //draw MEO
                _labels[2]->draw(meo, -1, -1, meoBox);
            
            } else if(isZoomedAtLevel[2]){
                
                //draw HEO
               _labels[1]->draw(heo, -1, -1, heoBox);
                
                //draw moonApogee
                _labels[0]->draw(moonApogee, 1, -1, moonApogeeBox);
            }
        }
    }ofPopStyle();
    
    //draw earth
    ofPushStyle();{
        ofSetColor(50);
        ofNoFill();
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofCircle(center,earthRadius); //earth
//        ofLine(center.x, center.y - earthRadius,center.x,center.y);
    }ofPopStyle();
    
    //draw framerate
    ofPushStyle();{
        ofSetColor(255, 0, 0);
        ofDrawBitmapString("FPS: ", 5, height - 5);
        ofDrawBitmapString(ofToString(ofGetFrameRate()), 45,height - 5);
    }ofPopStyle();
    
    //draw cursor
    ofPushMatrix();{
        
        ofSetColor(255,0,0,125);
        ofTranslate(0,0,1);
        ofLine(mousePos.x - 5, mousePos.y - 5, mousePos.x + 5, mousePos.y + 5);
        ofLine(mousePos.x + 5, mousePos.y - 5, mousePos.x - 5, mousePos.y + 5);
        ofLine(mousePos.x, 0, mousePos.x, height);
        ofLine(0, mousePos.y, width, mousePos.y);
        
        
        //testing difference highlighter
//        ofPushMatrix();
//        ofTranslate(width/2,100);
//        ofPath path;
//        path.setFilled(false);
//        path.setStrokeWidth(0.5);
//        path.setStrokeColor(ofColor(255,0,0));
//        path.moveTo(0,0);
//        path.lineTo(100, 0);
//        path.close();
//        path.draw();
//        ofPopMatrix();
        
    }ofPopMatrix();
    
}

//--------------------------------------------------------------
void ofApp::JsonInit(bool _isJsonMade, bool _areVarsInitiated){
    
    if(!_isJsonMade){
        cout << "Retrieving satellite data from interwebs and saving it to local file as JSON" << endl << endl;
        
//        curl.setURL("https://www.space-track.org/ajaxauth/login");
//        string email = "mapsat2015@gmail.com";
//        string pw = "kpeHmKY8pPXx4nHW";
//        curl.setOpt(CURLOPT_USERNAME, email);
//        curl.setOpt(CURLOPT_USERPWD, pw);
//        
//        curl.setURL("https://www.space-track.org/basicspacedata/query/class/tle_latest/ORDINAL/1/EPOCH/%3Enow-30/orderby/OBJECT_ID/format/json");
        
        char command[] = "curl -c cookies.txt -b cookies.txt -k https://www.space-track.org/ajaxauth/login -d 'identity=mapsat2015@gmail.com&password=kpeHmKY8pPXx4nHW&query=https://www.space-track.org/basicspacedata/query/class/tle_latest/ORDINAL/1/EPOCH/%3Enow-30/orderby/OBJECT_ID/format/json' > ~/Documents/of_v0.8.3_osx_release/apps/satellites/satellites3/bin/data/spaceTrackQuery.json";
        
        //-c = --cookie-jar <filename> = Specify to which local file you want curl to write all cookies after a completed operation. Curl writes all cookies previously read from a specified file as well as all cookies received from remote server(s). If no cookies are known, no data will be written.
        
        //-b = --cookie <name=data> = Pass the data from ^ that command to the HTTP server as a cookie.
        
        //-k = --insecure = disables ssl verification (space-track documentation says to do this...)
        
        //-d = --data-ascii = Sends the specified data in a POST request to the HTTP server (like hitting submit button)
        
        system( command );
        
//        cout << "STATUS = " << status << endl << endl;
        cout << "Data retrieval finished. " << endl << "Stored at ~/Documents/of_v0.8.3_osx_release/apps/satellites/satellites3/bin/data/spaceTrackQuery.json" << endl;
        
        //---------- Retrieving locally saved JSON file
        
        std::string file = "spaceTrackQuery.json";
        
        bool parsingSuccessful = result.open(file);
        
        if (!parsingSuccessful){
            ofLogError("ofApp::setup")  << "Failed to parse JSON" << endl;
        } else {
            _isJsonMade = true;
        }
    }
    
    // ---------- Setting up global vars dependent on json data
    
    if(_isJsonMade && !_areVarsInitiated){
        
        // ----- stuff based on json results
        
        numResults = result.size();
        cout << "result size = " << result.size() << endl;
        angle = 360/numResults;
        
        apogees.resize(numResults);
        perigees.resize(numResults);
        launchDates.resize(numResults);
        isWithinHoveredArc.resize(numResults);
        for(int i = 0; i < numResults; i++){
            apogees[i] = 0;
            perigees[i] = 0;
            isWithinHoveredArc[i] = false;
        }
        
        // ----- classes
        
        _arcs.setup();
        
        // ----- is that all the vars?
        _areVarsInitiated = true;
        areVarsInitiated = true;
    }

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch (key){
        case 'g': {
            gui->toggleVisible();
        } break;
        case 'f': {
            ofToggleFullscreen();
        } break;
        case 'k': { //print satellite names
            if (!once){
                for(int i = 0; i < 100;i++){
                    cout << i << " = " << launchDates[i] << endl;
                }
                once = !once;
            }
        } break;

    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key=='s') saver.finish("frame_" + ofToString(ofGetFrameNum()) + "_high.png", true);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    
    x = mouseX;
    y = mouseY;
    
//    if(x > showNamesButtons[i].getLeft() && x < showNamesButtons[i].getRight() && y > showNamesButtons[i].getTop() && y < showNamesButtons[i].getBottom()){
//        
//        showNames[i] = true;
//    } else {
//        showNames[i] = false;
//    }
    
    if(dataAllocated){
        
        for(int i = 0; i < _labels.size(); i++){
            _labels[i]->mouseOver(x, y);
        }
        
        if(isZoomedAtLevel[0]){
            _arcs.mouseOver(x, y);
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button, int i){

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
void ofApp::guiEvent(ofxUIEventArgs &e){
    string name = e.getName();
//	int kind = e.getKind();
    
    if(name == "ZOOM") {
		ofxUIRadio *zoomLevelRadio = (ofxUIRadio *) e.widget;
        cout << zoomLevelRadio->getName() << " value: " << zoomLevelRadio->getValue() << " zooming to: " << zoomLevelRadio->getActiveName() << endl;
        if(zoomLevelRadio->getValue() == 0){ //LEO
            
                startZoom[0] = true;
                //scale = 18;
            
        } else if(zoomLevelRadio->getValue() == 1){ //MEO
            
            if(scale < 90){ //if less than target
                startZoom[1] = true;
            } else if (scale > 90){ //if greater than target
                startZoom[2] = true;
            }
//            scale = 90;
            
        } else if(zoomLevelRadio->getValue() == 2){ //HEO
            
            startZoom[3] = true;
//            scale = 570;
        }
        
    } else if(name == "Show Labels") {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        cout << toggle->getName() << " value: " << toggle->getValue() << endl;
        if(toggle->getValue() == false){
            showLabels = false;
        } else if (toggle->getValue() == true){
            showLabels = true;
        }
    } else if(name == "VIEW DATASET") {
        ofxUIRadio *viewDatasetRadio = (ofxUIRadio *) e.widget;
        cout << viewDatasetRadio->getName() << " value: " << viewDatasetRadio->getValue() << " viewing data: " << viewDatasetRadio->getActiveName() << endl;
        if(viewDatasetRadio->getValue() == 0){ //apogee
            viewDatasets[0] = true;
            viewDatasets[1] = false;
            viewDatasets[2] = false;
        } else if(viewDatasetRadio->getValue() == 1){ //perigee
            viewDatasets[0] = false;
            viewDatasets[1] = true;
            viewDatasets[2] = false;
        } else if(viewDatasetRadio->getValue() == 2){ //both
            viewDatasets[0] = false;
            viewDatasets[1] = false;
            viewDatasets[2] = true;
        }
    }
}

//--------------------------------------------------------------
void ofApp::exit(){
    delete gui;
    
    for(int i = 0; i < _labels.size(); i++){
        delete _labels[i];
    }
    
    
}