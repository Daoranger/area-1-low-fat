#include "Mothership.h"

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
    cout << model.getMeshCount() << endl;
}

void Mothership::handleCollision(Ufo& ufo)
{
    cout << "UFO collided with mothership\n";
}