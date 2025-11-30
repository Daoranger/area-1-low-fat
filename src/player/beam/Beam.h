#pragma once
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "../collision/box.h"

class Beam
{
    Beam();
    void activate();
    void deactivate();
    void update();
    void draw();
    bool checkInside(Box box);

    glm::vec3 pos;
    glm::vec3 center;
    bool active = false;

    float radius;
    float height;
    float maxHeight;
    float extendRate;
    float retractRate;
};
