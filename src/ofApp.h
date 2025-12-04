#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxAssimpModelLoader.h"
#include "player/Ufo.h"
#include "collision/Octree.h"
#include "glm/gtx/intersect.hpp"
#include "objects/Object.h"
#include "objects/ChargingStation.h"
#include "objects/Mothership.h"
#include "objects/SpeedRing.h"
#include "objects/Cow.h"
#include "objects/CowPlatform.h"
#include "player/beam/Beam.h"
#include <string>
#include <filesystem>

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
		ofVec3f getNormalAtContactPoint();
		void resetGame();

		// Helper functions for cameras:
		void updateGameCamera();
		void nextGameCameraView();

		// Game States
		enum GameState { STATE_TITLE, STATE_GAMESTART, STATE_GAMEOVER };
		GameState gameState = STATE_TITLE;

		// Handle Gameover
		float deathStartTime = 0.0f;

		// Menu Items
		enum MenuItem { MENU_START, MENU_INSTR, MENU_DIAG, MENU_QUIT };
		MenuItem currentMenuItem = MENU_START;

		// World
		ofImage skyBox;
		ofImage titleBackground;
		
		// Cameras
		ofEasyCam debugCam;	// for debug/diagnostic
		ofCamera gameCam;	// for gameplay
		enum CamView { CAM_THIRD, CAM_FIRST, CAM_TOP, CAM_GROUND, CAM_DEATH };
		CamView camView = CAM_THIRD;
		ofCamera* activeCam = &gameCam;	// pointer to camera that is currently being use
		ofVec3f camTrackPosition;

		// UFO (Player)
		Ufo ufo;
		Beam beam;
		ofVec3f startUfoPos = ofVec3f(0, 200, 0);

		// Terrrain Model
		ofxAssimpModelLoader terrain;
		bool bTerrainLoaded;

		// Keys Map to help with keybinds
		map<int, bool> keysMap;


		// Octree and Collision Detection
		bool bDrawOctree = false;
		Octree terrainOctree;				// The octree of the terrain
		vector<TreeNode> colNodeList;		// Store all collided (leaf) nodes
		vector<Box> colBoxList;				// Store all collided (leaf) nodes's boxes
		bool cowCaptured = false;

		// UI 
		ofTrueTypeFont fontUI;
		ofTrueTypeFont fontTitle;

		int ORIGINAL_WIDTH = ofGetWindowWidth();
		int ORIGINAL_HEIGHT = ofGetWindowHeight();

		float lastScaleX = 1;
		float lastScaleY = 1;
		
		// Game Object
		ChargingStation station1;
		Mothership mothership;
		SpeedRing speedRing1;
		Cow cow1;
		CowPlatform cowPlatform;

		// Lightnings
		ofLight ufoLight;			// Spotlight underneath the UFO like a beam
		bool bToggleUFOLight = false;
		ofLight sunLight;			// Directional light to represent sun light
		ofLight sunLight2;			// Backup light to light scene better
		ofLight sunLight3;			// Backup light to light scene better


		// Sounds
		ofSoundPlayer titleMusic;
		ofSoundPlayer bgMusic;
		ofSoundPlayer goMusic;
		ofSoundPlayer sparkSound;
		bool bSparkSoundPlayed = false;

};

