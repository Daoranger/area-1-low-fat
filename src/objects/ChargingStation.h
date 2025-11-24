#pragma once
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "Object.h"
#include "../collision/box.h"
#include "../collision/Octree.h"
#include "../player/Lander.h"
#include <string>

class ChargingStation : public Object
{
public:
    void loadModel() override;
    void handleCollision(Lander&) override;
};