/**
 * Alison Schonauer and Hoang Nguyen
 * CS 134: Computer Game Design and Programming
 * Professor Kevin Smith
 * Fall 2025
 */
#pragma once
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "../collision/box.h"
#include "../collision/Octree.h"
#include "Object.h"
#include <string>

class Cow : public Object
{
public:
    Cow();
    void loadModel() override;
	void updateBoundingBox() override;
	void integrate() override;
	void handleLanding();
	void follow(glm::vec3 * Pos);
	void free();
	void destroy();

    enum STATE {FREE, ABDUCTED, LANDED};
	STATE state = FREE;

	glm::vec3 * followedPoint;

	ofVec3f force;
	ofVec3f velocity;
	ofVec3f acceleration;
	float forceStrength = 5;

	float rotForce = 0;
	float rotSpeed = 0;
	float rotAcceleration = 0;

	float damping;
	float mass;

	TreeNode terrainHitNode;
	ofVec3f terrainHitLocation;

	bool bAlive = true;;

};