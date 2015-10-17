
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

#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofSetupOpenGL(1024,768,OF_WINDOW); //1024,768
//    ofSetupOpenGL(1728,2592,OF_WINDOW); // <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
