/**
 * Alison Schonauer and Hoang Nguyen
 * CS 134: Computer Game Design and Programming
 * Professor Kevin Smith
 * Fall 2025
 */

#include "ChargingStation.h"

ChargingStation::ChargingStation()
{
    position.set(100, 100, 0);
    rotation = 0.0;
    scale.set(1, 1, 1);

    light.setup();
    light.setSpotlight();
    light.setSpotlightCutOff(10);

}

// Load the charging station model and its color model
//
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

// When UFO collides with ChargingStation, increase its fuel time by 20 units per second, up to the maximum fuel time
//
void ChargingStation::handleCollision(Ufo& ufo)
{
    float deltaTime = 1.0 / ofGetFrameRate();
    ufo.fuelLeftTime = std::min(ufo.fuelLeftTime + 20 * deltaTime, ufo.fuelTotalTime);
}

// If the UFO is within 15 units of the charging station, enable the light; otherwise, disable it
//
void ChargingStation::updateLight(Ufo& ufo)
{
    light.setPosition(position + glm::vec3(0, 10, 0));
    light.lookAt(position);
    if (glm::distance(glm::vec3(ufo.position), glm::vec3(position)) <= 15)
        light.enable();
    else
        light.disable();
}
