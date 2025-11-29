#include "CowPlatform.h"

CowPlatform::CowPlatform()
{
    position.set(50, 200, 0);
    rotation = 0.0;
    scale.set(1, 1, 1);
}

void CowPlatform::loadModel()
{
    if (model.loadModel("geo/cow-platform.obj"))
    {
        model.setScaleNormalization(false);
    }
}

void CowPlatform::handleCollision(Ufo& ufo)
{
    float deltaTime = 1.0 / ofGetFrameRate();
    ufo.fuelLeftTime = std::min(ufo.fuelLeftTime + 20 * deltaTime, ufo.fuelTotalTime);
}
