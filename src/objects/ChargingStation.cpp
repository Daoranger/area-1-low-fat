#include "ChargingStation.h"

ChargingStation::ChargingStation()
{
    position.set(100, 100, 0);
    rotation = 0.0;
    scale.set(1, 1, 1);
}

void ChargingStation::loadModel()
{
    if (model.loadModel("geo/charging-station-oct.obj"))
    {
        model.setScaleNormalization(false);
    }
    if (modelColor.loadModel("geo/charging-station.obj"))
    {
        modelColorsLoaded = true;
        modelColor.setScaleNormalization(false);
    }
}

void ChargingStation::handleCollision(Ufo& ufo)
{
    float deltaTime = 1.0 / ofGetFrameRate();
    ufo.fuelLeftTime = std::min(ufo.fuelLeftTime + 20 * deltaTime, ufo.fuelTotalTime);
}
