#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxAssimpModelLoader.h"
#include "player/Lander.h"

class ofApp : public ofBaseApp
{

	public:
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		// Help functions:
		void initLightingAndMaterials();

		// Keys Map to help with keybinds
		map<int, bool> keysMap;
		
		// Camera
		ofEasyCam cam;

		// UFO (Player)
		Lander lander;

		ofxAssimpModelLoader mars;


};

