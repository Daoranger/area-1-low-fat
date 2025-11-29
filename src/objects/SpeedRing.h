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

public:
    bool bVertical = false;

};