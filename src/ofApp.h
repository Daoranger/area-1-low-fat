#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxAssimpModelLoader.h"
#include "player/Lander.h"
#include "collision/Octree.h"
#include "glm/gtx/intersect.hpp"

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

		// Helper functions:
		void initLightingAndMaterials();

		// Helper functions for cameras:
		void updateGameCamera();
		void nextGameCameraView();
		
		// Cameras
		ofEasyCam debugCam;	// for debug/diagnostic
		ofCamera gameCam;	// for gameplay
		enum CamView { CAM_THIRD, CAM_FIRST, CAM_TOP };
		CamView camView = CAM_THIRD;
		ofCamera* activeCam = &gameCam;	// pointer to camera that is currently being use

		// UFO (Player)
		Lander lander;

		// Terrrain Model
		ofxAssimpModelLoader terrain;

		// Keys Map to help with keybinds
		map<int, bool> keysMap;


		// Octree and Collision Detection
		Octree terrainOctree;				// The octree of the terrain
		vector<TreeNode> colNodeList;		// Store all collided (leaf) nodes
		vector<Box> colBoxList;				// Store all collided (leaf) nodes's boxes

		// UI 
		ofTrueTypeFont fontUI;
		

};

