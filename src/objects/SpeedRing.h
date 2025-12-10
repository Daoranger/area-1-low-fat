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

class SpeedRing : public Object
{
public:
    SpeedRing();
    void loadModel() override;
    void handleCollision(Ufo&) override;
    void handleSpeedBoost(Ufo&);

public:
    bool bVertical = false;

};