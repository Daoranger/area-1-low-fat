/**
 * Alison Schonauer and Hoang Nguyen
 * CS 134: Computer Game Design and Programming
 * Professor Kevin Smith
 * Fall 2025
 */

#include "CowPlatform.h"

CowPlatform::CowPlatform()
{
    position.set(50, 200, 0);
    rotation = 0.0;
    scale.set(1, 1, 1);
}

//  Load the cow platform model
//
void CowPlatform::loadModel()
{
    if (model.loadModel("geo/cow-platform.obj"))
    {
        model.setScaleNormalization(false);
    }
}

// When UFO collides with CowPlatform, increase its fuel time by 20 units per second, up to the maximum fuel time
//
void CowPlatform::handleCollision(Ufo& ufo)
{
    float deltaTime = 1.0 / ofGetFrameRate();
    ufo.fuelLeftTime = std::min(ufo.fuelLeftTime + 20 * deltaTime, ufo.fuelTotalTime);
}
