//
//  Labels.cpp
//  satellites6
//
//  Created by Andrew McCausland on 8/27/15.
//
//

#include "Labels.h"

int Labels::l_id = 0;

//--------------------------------------------------------------
Labels::Labels() : label_id(Labels::l_id++) {
    
    cout << "label_id = " << label_id << endl;
    
}

//--------------------------------------------------------------
void Labels::setup(float _earthRad, float _meoDist, float _heoDist, float _moonPerigeeDist, float _moonApogeeDist, ofTrueTypeFont _fontM, ofTrueTypeFont _fontL, ofTrueTypeFont _fontXL, ofColor _bgColor){

    fontM = _fontM;
    fontL = _fontL;
    fontXL = _fontXL;
    bgColor = _bgColor;
    isHovering = false;
    hoverRectWidth = 0;
    meoDist = _meoDist;
    heoDist = _heoDist;
    moonPerigeeDist = _moonPerigeeDist;
    moonApogeeDist = _moonApogeeDist;
    
}

//--------------------------------------------------------------
void Labels::exit(){
    
    cout << "Labels deleted." << endl;
}

//--------------------------------------------------------------
void Labels::update(float _scale, float _earthRad){
    
    scale = _scale;
    earthRad = _earthRad;
    
    switch (label_id){
        case 1:{ //HEO
            hoverRectWidth = (moonApogeeDist/scale) - (heoDist/scale);
        }break;
        case 2:{ //MEO
            hoverRectWidth = (heoDist/scale) - (meoDist/scale);
        }break;
        case 3:{ //LEO
            hoverRectWidth = (meoDist/scale) - (earthRad/scale);
        }break;
        case 4:{ //earthRad
            hoverRectWidth = earthRad;
        }break;
    }
}

//--------------------------------------------------------------
void Labels::draw(string _labelTxt, int labelType, int fontType, ofRectangle labelBox){
    
    ofPushStyle();{
        
        if(isHovering){
            switch (label_id){
                case 1:{ //HEO
                    labelTxt = "FURTHER THAN " + ofToString(heoDist) + " KM";
                }break;
                case 2:{ //MEO
                    labelTxt = ofToString(meoDist) + " KM TO " + ofToString(heoDist) + " KM";
                }break;
                case 3:{ //LEO
                    labelTxt = "EARTH'S SURFACE TO " + ofToString(meoDist) + " KM";
                }break;
                case 4:{ //earthRad
                    labelTxt = ofToString(earthRad) + " KM";
                }break;
            }
        } else {
            labelTxt = _labelTxt;
        }
        
        //hover state
        switch (labelType) {
                
            case -1: { // L/R arrows
                
                if(isHovering){
                    ofPushStyle();{
                        ofSetRectMode(OF_RECTMODE_CENTER);
                        ofSetColor(255, 0, 0, 127);
                        ofRect(labelBox.x, labelBox.y + labelBox.getHeight()/2 + 4, hoverRectWidth, 26);
                    }ofPopStyle();
                }
                
            } break;
            case 0: { //no arrows
                
            } break;
            case 1: { // U/D arrows
                
                if(isHovering){
                    isHoveringLine = true;
                }
                
            } break;
        }
        
        ofFill();
        ofSetRectMode(OF_RECTMODE_CORNER);
        ofSetColor(bgColor);

        
        //highlight
        ofRect(labelBox.getTopLeft().x - (labelBox.getWidth()/2) - 5,
               labelBox.getTopLeft().y + (labelBox.getHeight()/2) - 5,
               labelBox.getWidth() + 10,
               labelBox.getHeight() + 10);
                
        ofNoFill();
        ofSetColor(50);
        
        //border
        ofRect(labelBox.getTopLeft().x - (labelBox.getWidth()/2) - 5,
               labelBox.getTopLeft().y + (labelBox.getHeight()/2) - 5,
               labelBox.getWidth() + 10,
               labelBox.getHeight() + 10);
                
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofSetColor(50);
        
        //string
        if(fontType < 0){
            fontM.drawString(labelTxt,
                          labelBox.x - labelBox.getWidth()/2,
                          labelBox.y + labelBox.getHeight() + 5);
        } else if(fontType == 0){
            fontL.drawString(labelTxt,
                              labelBox.x - labelBox.getWidth()/2,
                              labelBox.y + labelBox.getHeight() + 9);
        } else if(fontType == 1){
            fontXL.drawString(labelTxt,
                              labelBox.x - labelBox.getWidth()/2 - 2,
                              labelBox.y + labelBox.getHeight() + 17);
        }
        
        ofFill();
        
        if(!isHovering){
            switch (labelType) {
                    
                case -1: {
                    
                    //left arrow
                    ofTriangle(labelBox.x - labelBox.getWidth()/2 - 17,
                               labelBox.y + labelBox.getHeight()/2 + 5,
                               labelBox.x - labelBox.getWidth()/2 - 10,
                               labelBox.y + labelBox.getHeight()/2,
                               labelBox.x - labelBox.getWidth()/2 - 10,
                               labelBox.y + labelBox.getHeight()/2 + 10);
                    
                    //right arrow
                    ofTriangle(labelBox.x + labelBox.getWidth()/2 + 17,
                               labelBox.y + labelBox.getHeight()/2 + 5,
                               labelBox.x + labelBox.getWidth()/2 + 10,
                               labelBox.y + labelBox.getHeight()/2,
                               labelBox.x + labelBox.getWidth()/2 + 10,
                               labelBox.y + labelBox.getHeight()/2 + 10);
                    
    //                if(isHovering){
    //                    ofRectMode(OF_RECTMODE_CENTER);
    //                    ofSetColor(255, 0, 0);
    //                    ofRect(labelBox.x, labelBox.y + labelBox.getHeight()/2 + 4, hoverRectWidth, 26);
    //                }
                    
                } break;
                case 0: {
                    //no arrows
                } break;
                case 1: {
                    
                    //up arrow
                    ofTriangle(labelBox.x,
                               labelBox.y - (labelBox.getHeight()/2) - 7,
                               labelBox.x + 5,
                               labelBox.y - (labelBox.getHeight()/2),
                               labelBox.x - 5,
                               labelBox.y - (labelBox.getHeight()/2));
                    
                    //down arrow
                    ofTriangle(labelBox.x,
                               labelBox.y + (labelBox.getHeight()) + 21,
                               labelBox.x + 5,
                               labelBox.y + (labelBox.getHeight()) + 14,
                               labelBox.x - 5,
                               labelBox.y + (labelBox.getHeight()) + 14);
                    
//                    if(isHovering){
//                        isHoveringLine = true;
//                    }
//                    
                } break;
            }
        }
        
    }ofPopStyle();
    
    _labelBox = labelBox;
    
}

//--------------------------------------------------------------
void Labels::mouseOver(int x, int y){
    
    if(x > (_labelBox.getTopLeft().x - (_labelBox.getWidth()/2) - 5) &&
       x < _labelBox.getTopLeft().x + (_labelBox.getWidth()/2 + 5) &&
       y > _labelBox.getTopLeft().y &&
       y < _labelBox.getTopLeft().y + _labelBox.getHeight() + 10){
        
//        cout << "hovering id = " << label_id << endl;
        
        //_labels[0] = moon apogee
        //_labels[1] = HEO
        //_labels[2] = MEO
        //_labels[3] = LEO
        //_labels[4] = earthRad
        //_labels[5] = arc hover (decade dates), no hover state
        
        if(label_id == 3){
//            hoverRectWidth = (meoDist/scale) - earthRad;
            cout << "meoDist/scale = " << meoDist/scale << endl;
            cout << "earthRad = " << earthRad << endl;
            cout << "hoverRectWidth = " << hoverRectWidth << endl;
        }
        
        isHovering = true;
    } else {
        isHovering = false;
    }
}

//--------------------------------------------------------------
void Labels::mouseReleased(){
    
}