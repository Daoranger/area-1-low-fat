#pragma once
#include "ofMain.h"
#include  "ofxAssimpModelLoader.h"

class Lander
{
public:
	Lander();
	void draw();
	void integrate();
	void loadModel();
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
};