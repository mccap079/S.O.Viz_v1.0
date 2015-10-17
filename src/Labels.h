//
//  Labels.h
//  satellites6
//
//  Created by Andrew McCausland on 8/27/15.
//
//

#pragma once

#include "ofMain.h"

class Labels {
    
protected: //accessible in this class and in classes that inherit from this class.
    
    static int l_id; //there is only a single instance of this variable that is shared among all instnces of this class
    int label_id;
    
public:
    Labels();
    void setup(float _earthRad, float _meoDist, float _heoDist, float _moonPerigeeDist, float _moonApogeeDist, ofTrueTypeFont _fontM, ofTrueTypeFont _fontL, ofTrueTypeFont _fontXL, ofColor _bgColor);
    void exit();
    void update(float _scale, float _earthRad);
    void draw(string _labelTxt, int labelType, int fontType, ofRectangle labelBox);
    void mouseOver(int x, int y);
    void mouseReleased();
    
    bool isHovering, isHoveringLine;
    float hoverRectWidth;
    
    ofPoint center;
    
    ofColor bgColor;
    
    ofTrueTypeFont fontM, fontL, fontXL;
    
    ofRectangle _labelBox;
    
    float scale, earthRad, meoDist, heoDist, moonPerigeeDist, moonApogeeDist;
    
    string labelTxt;
    
    //for labelType:
    // -1 = L/R arrows
    // 0 = no arrows
    // 1 = U/D arrows
    
    //for fontType:
    // -1 = small
    // 0 = lg
    // 1 = XL
};