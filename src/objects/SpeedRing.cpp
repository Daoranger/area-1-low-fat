#include "SpeedRing.h"

SpeedRing::SpeedRing()
{
    position.set(100, 100, 0);
    rotation = 0.0;
    scale.set(1, 1, 1);
}

void SpeedRing::loadModel()
{
    if (model.loadModel("geo/charging-station.obj"))
    {
        model.setScaleNormalization(false);
    }
}

void SpeedRing::handleCollision(Ufo& ufo)
{
    float deltaTime = 1.0 / ofGetFrameRate();
    ufo.fuelLeftTime = std::min(ufo.fuelLeftTime + 20 * deltaTime, ufo.fuelTotalTime);
}
