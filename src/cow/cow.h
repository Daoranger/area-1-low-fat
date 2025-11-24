#pragma once
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "../collision/box.h"
#include "../collision/Octree.h"

class Cow 
{
public:
    Cow();
    void integrate();
    void draw();
    void loadModel();
	void follow(glm::vec3 * Pos);
	void free();
    glm::mat4 getTransform();

    enum STATE {FREE, ABDUCTED};
	STATE state = FREE;

	glm::vec3 * followedPoint;

    ofVec3f position;
	float rotation;
	ofVec3f scale;
	ofVec3f velocity;
	ofVec3f acceleration;
	ofVec3f force;
	float forceStrength = 1;
	float rotationSpeed;
	float rotationAcceleration;
	float rotationForce;
	float mass;
	float damping;

	ofxAssimpModelLoader cowModel;
	Box cowBoundingBox;
	TreeNode terrainHitNode;
	ofVec3f terrainHitLocation;

};