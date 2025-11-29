#include "SpeedRing.h"

SpeedRing::SpeedRing()
{
    position.set(100, 200, 0);
    rotation = 90.0;
    scale.set(1, 1, 1);
}

void SpeedRing::loadModel()
{
    if (bVertical)
    {
        if (model.loadModel("geo/speed-ring-vertical.obj"))
        {
            model.setScaleNormalization(false);
        }
    }
    else
    {
        if (model.loadModel("geo/speed-ring-horizontal.obj"))
        {
            model.setScaleNormalization(false);
        }
    }
}

void SpeedRing::handleCollision(Ufo& ufo)
{
    float deltaTime = 1.0 / ofGetFrameRate();
    ufo.fuelLeftTime = std::min(ufo.fuelLeftTime + 20 * deltaTime, ufo.fuelTotalTime);
}
