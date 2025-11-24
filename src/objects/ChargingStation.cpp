#include "ChargingStation.h"

void ChargingStation::loadModel()
{
    if (model.loadModel("geo/charging-station.obj"))
    {
        model.setScaleNormalization(false);
    }
}

void ChargingStation::handleCollision(Lander& ufo)
{
    float deltaTime = 1.0 / ofGetFrameRate();
    ufo.fuelLeftTime = std::min(ufo.fuelLeftTime + deltaTime, ufo.fuelTotalTime);
}
