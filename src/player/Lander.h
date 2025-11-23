#pragma once
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "../collision/box.h"
#include "../collision/Octree.h"

class Lander
{
public:
	Lander();
	void draw();
	void integrate();
	void loadModel();
	void updateBoundingBox();
	void handleLanding();
	void handleTakeOff();
	void calculateAltitude(Octree&);
	bool hasFuel();
	glm::mat4 getTransform();
	glm::vec3 getHeadingX();
	glm::vec3 getHeadingY();
	glm::vec3 getHeadingZ();

public:
	ofVec3f position;
	float rotation;
	ofVec3f scale;
	ofVec3f velocity;
	ofVec3f acceleration;
	ofVec3f force;
	float rotationSpeed;
	float rotationAcceleration;
	float rotationForce;
	float mass;
	float damping;

	ofxAssimpModelLoader ufoModel;

	// Variables: collision
	Box ufoBoundingBox;
	TreeNode terrainHitNode;
	ofVec3f terrainHitLocation;
	float altitude = 0.0;
	bool bLandingImpulseDone = false;

	// Variables: fuel
	float fuelTotalTime = 10;		// 2 minutes = 120 seconds
	float fuelLeftTime;
	float fuelStartTime;
	bool  bfuelActive = false;

};