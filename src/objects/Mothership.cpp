#include "Mothership.h"

Mothership::Mothership()
{
    light.setup();
    light.setSpotlight();
    light.setSpotlightCutOff(10);
    light.setSpotConcentration(8);
    light.setDiffuseColor(ofFloatColor(2, 2, 2));
}

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

void Mothership::handleCollision(Ufo& ufo)
{
    cout << "UFO collided with mothership\n";
}

void Mothership::updateLight(Ufo& ufo)
{
    light.setPosition(position + glm::vec3(0, 80, 0));
    light.lookAt(position);
    if (glm::distance(glm::vec3(ufo.position), glm::vec3(position)) <= 40)
        light.enable();
    else
        light.disable();
}
