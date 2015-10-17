//
//  nameIndex.cpp
//  satellites3
//
//  Created by Andrew McCausland on 7/19/15.
//
//

#include "nameIndex.h"

nameIndex::nameIndex() {
    
}

//--------------------------------------------------------------
void nameIndex::setup(float _earthRadius){
    
    earthRadius = _earthRadius;
    cout << "earthRadius = " << earthRadius << endl;
    
    buttonHovered = false;
    
    showNamesButtons.resize(10);
    
    for(int i = 0; i < 10; i++){
        showNames.push_back(false);
        showNamesButtons[i].set(0, 0, 0, 0);
    }
}

//--------------------------------------------------------------
void nameIndex::update(ofPoint _position){
    
    //visible window params
    width = ofGetWidth();
    height = ofGetHeight();
    mouseX = ofGetMouseX();
    mouseY = ofGetMouseY();
    
    //whole canvas params
    canvasRight = _position.x + width*2;
    canvasLeft = _position.x - width*2;
    canvasTop = _position.y - height*4;
    canvasBottom = _position.y + height*4;
    canvasCenterX = _position.x - width/2;
    canvasCenterY = _position.y - height*1.5;
    
}

//--------------------------------------------------------------
void nameIndex::draw(int _i, int _moduleXPos){
    
    //hover state
    mouseOver(mouseX, mouseY, _i);
    if(showNames[_i]){
        ofSetColor(255);
    } else {
        ofSetColor(100);
    }
    
//    cout << "i = " << _i << "; topLeft = " << showNamesButtons[_i].getTopLeft() << endl;
    
    if(showNames[0] == true){
        
    }
    
    ofPushStyle();{
        ofFill();
        ofSetRectMode(OF_RECTMODE_CORNER);
        int buttonPosX = _moduleXPos;
        for(int i = 0; i < 10;i++){
            
            //            //hover state
            //            mouseMoved(mouseX, mouseY, i);
            //            if(showNames[i]){
            //                ofSetColor(255);
            //            } else {
            //                ofSetColor(100);
            //            }
            
            //            mouseReleased(mouseX, mouseY, button, i);
            //            showNamesButtons[i].set(ofRectangle(buttonPosX, canvasCenterY - earthRadius + 1, 9, 9) );
            //            ofRect(showNamesButtons[i].getTopLeft().x, showNamesButtons[i].getTopLeft().y, 9, 9);
            
            showNamesButtons[_i].set(ofRectangle(_moduleXPos, canvasCenterY - earthRadius + 1, 9, 9) );
            ofRect(showNamesButtons[_i].getTopLeft().x, showNamesButtons[_i].getTopLeft().y, 9, 9);
            buttonPosX += 10;
            
        }
    }ofPopStyle();
}

//--------------------------------------------------------------
void nameIndex::mouseOver(float _mouseX, float _mouseY, int _i){
    if(_mouseX > showNamesButtons[_i].getLeft() && _mouseX < showNamesButtons[_i].getRight() && _mouseY > showNamesButtons[_i].getTop() && _mouseY < showNamesButtons[_i].getBottom()){
        
        showNames[_i] = true;
    } else {
        showNames[_i] = false;
    }
}

//--------------------------------------------------------------
void nameIndex::mouseReleased(float _mouseX, float _mouseY, int _i){
    
}

//--------------------------------------------------------------
