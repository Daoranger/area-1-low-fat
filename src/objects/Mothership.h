#pragma once
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "Object.h"
#include "../collision/box.h"
#include "../collision/Octree.h"
#include "../player/Ufo.h"

class Mothership : public Object
{
public:
    void loadModel() override;
    void handleCollision(Ufo&) override;
};

