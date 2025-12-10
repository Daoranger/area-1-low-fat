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
#include <string>

// Forward declaration
class Ufo;

class Object
{

public:
    // Construtor and Destructor
    Object();
    virtual ~Object() = default;

    virtual void draw();
    virtual void integrate();
    virtual void loadModel();
    virtual void updateBoundingBox();
    virtual void createOctree();
    virtual void updateOctree();
    virtual void handleCollision(Ufo&);
    glm::mat4 getTransform();

public:
    ofVec3f position;
    float rotation;
    ofVec3f scale;

    ofxAssimpModelLoader model;
    ofxAssimpModelLoader modelColor;
    Box boundingBox;
    bool bHasBoundingBox;
    Octree octree;

    bool modelColorsLoaded = false;


};