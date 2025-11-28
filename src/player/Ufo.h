#pragma once
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "../objects/Object.h"
#include "../collision/box.h"
#include "../collision/Octree.h"

class Ufo: public Object
{
public:
	Ufo();
	void draw() override;
	void integrate() override;
	void loadModel() override;
	void updateBoundingBox() override;
	void handleLanding();
	void handleTakeOff();
	void calculateAltitude(Octree&);
	bool hasFuel();
	glm::vec3 getHeadingX();
	glm::vec3 getHeadingY();
	glm::vec3 getHeadingZ();

public:

	ofVec3f velocity;
	ofVec3f acceleration;
	ofVec3f force;
	float rotationSpeed;
	float rotationAcceleration;
	float rotationForce;
	float mass;
	float damping;

	// Variables: collision
	TreeNode terrainHitNode;
	ofVec3f terrainHitLocation;
	float altitude = 0.0;
	bool bLandingImpulseDone = false;

	// Variables: fuel
	float fuelTotalTime = 120.0;		// 2 minutes = 120 seconds
	float fuelLeftTime;
	float fuelStartTime;
	bool  bfuelActive = false;

};