/**
 * Alison Schonauer and Hoang Nguyen
 * CS 134: Computer Game Design and Programming
 * Professor Kevin Smith
 * Fall 2025
 */

#include "Mothership.h"

// Set up the mothership's light properties in the constructor
//
Mothership::Mothership()
{
    light.setup();
    light.setSpotlight();
    light.setSpotlightCutOff(10);
    light.setSpotConcentration(8);
    light.setDiffuseColor(ofFloatColor(2, 2, 2));
}

// Load the mothership model and its color model
//
void Mothership::loadModel()
{
    if (model.loadModel("geo/mothership-oct.obj"))
    {
        model.setScaleNormalization(false);
    }
    if (modelColor.loadModel("geo/mothership.obj"))
    {
        modelColorsLoaded = true;
        modelColor.setScaleNormalization(false);
    }
}

// Handle mothership collision with UFO
//
void Mothership::handleCollision(Ufo& ufo)
{
    cout << "UFO collided with mothership\n";
}

// If the UFO is within 40 units of the mothership, enable the light; otherwise, disable it
//
void Mothership::updateLight(Ufo& ufo)
{
    light.setPosition(position + glm::vec3(0, 80, 0));
    light.lookAt(position);
    if (glm::distance(glm::vec3(ufo.position), glm::vec3(position)) <= 40)
        light.enable();
    else
        light.disable();
}
