//
//  arcs_1.h
//  satellites6
//
//  Created by Andrew McCausland on 9/3/15.
//
//

#pragma once

#include "ofMain.h"
//#include "labels.h"

class Arcs_1 {
    public:
    
    Arcs_1();
    void setup();
    void update(ofPoint _center, ofPoint _mousePos, float _earthRadius);
    void draw(vector <float> _dateAngles, int i);
    void mouseOver(int x, int y);
    void mouseReleased();
    
//    Labels _labels;
    
    float earthRadius;
    
    ofPoint center, mousePos;
    
    ofPath path;
    
    vector <bool> isHovering;
    bool isAnyHovering;
    
    int thisLine;
    
    vector <ofPolyline> polylines;
    
};
