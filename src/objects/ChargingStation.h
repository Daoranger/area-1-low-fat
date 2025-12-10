/**
 * Alison Schonauer and Hoang Nguyen
 * CS 134: Computer Game Design and Programming
 * Professor Kevin Smith
 * Fall 2025
 */

#pragma once
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "Object.h"
#include "../collision/box.h"
#include "../collision/Octree.h"
#include "../player/Ufo.h"
#include <string>

class ChargingStation : public Object
{
public:
    ChargingStation();
    void loadModel() override;
    void handleCollision(Ufo&) override;
    void updateLight(Ufo&);

    ofLight light;
};