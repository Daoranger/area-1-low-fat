#pragma once
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "../collision/box.h"

class Beam
{
public:
    Beam();
    void toggle();
    void update();
    void draw();
    bool checkInside(Box box);

    glm::vec3 pos;
    glm::vec3 capturePoint;
    bool active;

    float radius;
    float height;
    float maxHeight;
    float extendRate;
    float retractRate;
};
