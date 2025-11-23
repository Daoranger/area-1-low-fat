#pragma once
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "../collision/box.h"
#include "../collision/Octree.h"

class Object
{

public:
    Object(string&);
    virtual void draw();
    virtual void loadModel(string&);
    virtual void updateBoundingBox();
    virtual void handleCollision();
    glm::mat4 getTransform();

public:
    ofVec3f position;
    float rotation;
    ofVec3f scale;

    ofxAssimpModelLoader model;
    Box boudningBox;
    Octree octree;



};