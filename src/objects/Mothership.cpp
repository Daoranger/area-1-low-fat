#include "Mothership.h"

void Mothership::loadModel()
{
    if (model.loadModel("geo/mothership.obj"))
    {
        model.setScaleNormalization(false);
    }
}

void Mothership::handleCollision(Ufo& ufo)
{
    cout << "UFO collided with mothership\n";
}