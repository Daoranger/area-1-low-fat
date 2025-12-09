#include "SpeedRing.h"

SpeedRing::SpeedRing()
{
    position.set(100, 200, 0);
    rotation = 30;
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
    /*boundingBox = octree.root.box;
    Box newBox = Box(Vector3(boundingBox.min().x() + position.x, boundingBox.min().y() + position.y, boundingBox.min().z() + position.z),
                    Vector3(boundingBox.max().x() + position.x, boundingBox.max().y() + position.y, boundingBox.max().z() + position.z));*/
    Box boostBox = octree.root.box;
    if (ufo.boundingBox.overlap(boostBox)) {
        if (bVertical) {
            ufo.velocity.z *= 1.08;
        }
        else {
            ufo.velocity.y *= 1.08;
        }
    }
}

void SpeedRing::handleCollision(Ufo& ufo)
{
    cout << "Collided with speed ring" << endl;
    if (bVertical) {
        glm::vec3 dir = glm::vec3(0, 0, 1);
        dir = glm::rotate(dir, glm::radians(rotation), glm::vec3(0,1,0));
        dir = glm::abs(dir);
        ufo.velocity.z += (5 * dir.z);
        ufo.velocity.x += (5 * dir.x);
    }
    else {
        ufo.velocity.y *= 1.08;
    }
}
