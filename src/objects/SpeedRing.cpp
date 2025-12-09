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

void SpeedRing::handleSpeedBoost(Ufo& ufo)
{
    boundingBox = octree.root.box;
    Box newBox = Box(Vector3(boundingBox.min().x() + position.x, boundingBox.min().y() + position.y, boundingBox.min().z() + position.z),
                    Vector3(boundingBox.max().x() + position.x, boundingBox.max().y() + position.y, boundingBox.max().z() + position.z));
    if (ufo.boundingBox.overlap(newBox)) {
        if (bVertical) {
            ufo.velocity.x *= 1.08;
        }
        else {
            ufo.velocity.y *= 1.08;
        }
    }
}

void SpeedRing::handleCollision(Ufo& ufo)
{
    cout << "Collided with speed ring" << endl;
}
