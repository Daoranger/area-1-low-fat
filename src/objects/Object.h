#pragma once
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "../collision/box.h"
#include "../collision/Octree.h"
#include "../player/Lander.h"
#include <string>

class Object
{

public:
    Object();
    virtual void draw();
    virtual void loadModel();
    virtual void updateBoundingBox();
    virtual void createOctree();
    virtual void updateOctree();
    virtual void handleCollision(Lander&);
    glm::mat4 getTransform();

public:
    ofVec3f position;
    float rotation;
    ofVec3f scale;

    ofxAssimpModelLoader model;
    Box boudningBox;
    Octree octree;



};