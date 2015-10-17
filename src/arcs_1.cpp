//
//  arcs_1.cpp
//  satellites6
//
//  Created by Andrew McCausland on 9/3/15.
//
//

#include "arcs_1.h"

//--------------------------------------------------------------
Arcs_1::Arcs_1(){
    
}

//--------------------------------------------------------------
void Arcs_1::setup(){
    
    earthRadius = 0;
    center.x = center.y = 0;
    thisLine = 0;
    isAnyHovering = false;
    
    isHovering.resize(7);
    polylines.resize(7);
    for (int i = 0; i < polylines.size(); i++){
        polylines[i].setClosed(true);
        isHovering[i] = false;
    }
}

//--------------------------------------------------------------
void Arcs_1::update(ofPoint _center, ofPoint _mousePos, float _earthRadius){
    
    earthRadius = _earthRadius;
    center = _center;
    mousePos = _mousePos;
    
//    cout << "thisLine = " << thisLine << endl; //always 6 (last iteration of i...)

    
}

//--------------------------------------------------------------
void Arcs_1::draw(vector <float> _dateAngles, int i){
    
//            ofPath path;
//            path.setCircleResolution(120);
//            path.setFilled(true);
//            path.setFillColor(ofColor(50));
////            path.setStrokeColor(ofColor(0,127,255));
////            path.setStrokeWidth(1);
//            path.moveTo(0,0);
//            path.arc(0, 0, earthRadius - 15, earthRadius - 15, _dateAngles[i-1], _dateAngles[i] - 1);
//            path.arcNegative(0 , 0, earthRadius - 35, earthRadius - 35, _dateAngles[i] - 1, _dateAngles[i-1]);
//            path.close();
//            path.draw();
    
//    for(int i = 1; i < 7; i++){
    
//        ofPolyline curve;
//        curve.setClosed(true);
//        curve.arc(0, 0, earthRadius - 15, earthRadius - 15, _dateAngles[i-1], _dateAngles[i] - 1, 120);
//        curve.arcNegative(0 , 0, earthRadius - 35, earthRadius - 35, _dateAngles[i] - 1, _dateAngles[i-1], 120);
//        curve.draw();
//        curve.getSmoothed(10,1);
//    }

    polylines[i].clear();
    polylines[i].setClosed(true);
    polylines[i].arc(0, 0, earthRadius - 15, earthRadius - 15, _dateAngles[i-1], _dateAngles[i] - 1, 120);
    polylines[i].arcNegative(0 , 0, earthRadius - 35, earthRadius - 35, _dateAngles[i] - 1, _dateAngles[i-1], 120);
    polylines[i].draw();
    polylines[i].getSmoothed(10,1);
    
    if(polylines[i].inside((mousePos.x - center.x), (mousePos.y - center.y)) == true){
        isHovering[i] = true;
//        cout << "we're hovering " << i << endl;
    } else {
        isHovering[i] = false;
    }
    
    if(isHovering[0] || isHovering[1] || isHovering[2] || isHovering[3] || isHovering[4] || isHovering[5] || isHovering[6]){
        isAnyHovering = true;
    } else {
        isAnyHovering = false;
    }

}

//--------------------------------------------------------------
void Arcs_1::mouseOver(int x, int y){
    
}

//--------------------------------------------------------------
void Arcs_1::mouseReleased(){
    
}

