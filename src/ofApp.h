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
#include "particles/ParticleEmitter.h"
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
		void updateTrackCamera();
		void nextGameCameraView();
		void nextTrackCameraView();

		// Game States
		enum GameState { STATE_TITLE, STATE_GAMESTART, STATE_GAMEOVER, STATE_INSTRUCTION, STATE_DIAGNOSTIC };
		GameState gameState = STATE_TITLE;
		bool bVictory = false;

		// Handle Gameover
		float deathStartTime = 0.0f;
		float fuelDeathStartTime = 0.0f;
		float bFuelDeathPending = false;

		// Menu Items
		enum MenuItem { MENU_START, MENU_INSTR, MENU_DIAG, MENU_QUIT };
		MenuItem currentMenuItem = MENU_START;

		// World
		ofImage skyBox;
		ofImage titleBackground;
		ofImage victoryBackground;
		ofImage defeatBackground;
		int nCowAbducted = 0;
		int nCowRequired = 5;
		
		// Cameras
		ofEasyCam debugCam;	// for debug/diagnostic
		ofCamera gameCam;	// for gameplay
		ofCamera trackCam;	// for tracking
		enum CamView { CAM_THIRD, CAM_FIRST, CAM_TOP, CAM_GROUND, CAM_DEATH };
		CamView camView = CAM_THIRD;
		enum TrackView { TRACK_MOTHERSHIP, TRACK_MOUNTAIN, TRACK_TREE, TRACK_LAKE, TRACK_SKY, TRACK_COW };
		TrackView trackView = TRACK_MOTHERSHIP;
		ofCamera* activeCam = &gameCam;	// pointer to camera that is currently being use
		ofVec3f camTrackPosition;

		// Shader
		ofShader shader;

		// Particle Emitters
		ParticleEmitter ufoMove;
		ParticleEmitter ufoExplosion;

		// UFO (Player)
		Ufo ufo;
		Beam beam;
		ofVec3f startUfoPos = ofVec3f(0, 550, 0);

		// Diagnostic Mode
		bool bUfoSelected = false;
		glm::vec3 mouseDownPos, mouseLastPos;
		bool bInDrag = false;
		glm::vec3 getMousePointOnPlane(glm::vec3 p, glm::vec3 n);

		// Terrrain Model
		ofxAssimpModelLoader terrain;
		ofxAssimpModelLoader terrainColor;
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
		ofTrueTypeFont fontInstruction;
		ofTrueTypeFont fontSmallText;

		int ORIGINAL_WIDTH = ofGetWindowWidth();
		int ORIGINAL_HEIGHT = ofGetWindowHeight();

		float lastScaleX = 1;
		float lastScaleY = 1;
		
		// Game Objects
		ChargingStation station1;
		ChargingStation station2;
		ChargingStation station3;

		Mothership mothership;
		SpeedRing speedRing1;

		// List of cows
		std::vector<std::unique_ptr<Cow>> cows;

		// List of cows positions
		std::vector<ofVec3f> cowsPositions 
		{ 
			ofVec3f(30, 50, 50), 
			ofVec3f(30, 50, 30),
			ofVec3f(30, 50, 40),
			ofVec3f(536.157, 70, -476.551),
			ofVec3f(550.157, 70, -476.551),
			ofVec3f(575.157, 70, -476.551),
			ofVec3f(-442.068, 145, 340),
			ofVec3f(-432.068, 145, 360),
			ofVec3f(-452.068, 145, 380),
			ofVec3f(-462.068, 145, 400)
		};
		Cow cow1;
		CowPlatform cowPlatform;

		// Lightings
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
		ofSoundPlayer alarmSound;
		bool bSparkSoundPlayed = false;
		ofSoundPlayer ufoSound;
		ofSoundPlayer ufoBeamSound;
		ofSoundPlayer menuHoverSound;
		ofSoundPlayer menuSelectedSound;

};

