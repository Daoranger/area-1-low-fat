#pragma once
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "Object.h"
#include "../collision/box.h"
#include "../collision/Octree.h"
#include "../player/Ufo.h"
#include <string>


class CowPlatform : public Object
{
public:
    CowPlatform();
    void loadModel() override;
    void handleCollision(Ufo&) override;
};