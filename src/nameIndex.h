//
//  nameIndex.h
//  satellites3
//
//  Created by Andrew McCausland on 7/19/15.
//
// This is the interactive menu that displays lists of all satellite names.

#pragma once

#include "ofMain.h"

class nameIndex {
public:
    nameIndex();
    void setup(float _earthRadius);
    void update(ofPoint _position);
    void draw(int _i, int _moduleXPos);
    void mouseOver(float _mouseX, float _mouseY, int _i);
    void mouseReleased(float _mouseX, float _mouseY, int _i);
    
    float width, height, mouseX, mouseY;
    float canvasRight, canvasLeft, canvasTop, canvasBottom, canvasCenterX, canvasCenterY;
    float earthRadius;
    
    bool buttonHovered;
    
    vector <ofRectangle> showNamesButtons;
    vector <bool> showNames;
    vector <string> satNames;
};