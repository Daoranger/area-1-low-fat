#include "Object.h"

Object::Object(string& path)
{
	position.set(0, 0, 0);
	rotation = 0.0;
	scale.set(1, 1, 1);

    loadModel(path);
}

void Object::draw()
{
    ofPushMatrix();
    ofMultMatrix(getTransform());
    model.drawFaces();
    ofPopMatrix();
}

void Object::loadModel(string& path)
{
    if (model.loadModel(path))
    {
        model.setScaleNormalization(false);
    }
}

void Object::updateBoundingBox()
{
    return;
}

void Object::handleCollision()
{
    return;
}

glm::mat4 Object::getTransform()
{
    glm::mat4 T = glm::translate(glm::mat4(1.0), glm::vec3(position));
    glm::mat4 R = glm::rotate(glm::mat4(1.0), glm::radians(rotation), glm::vec3(0, 1, 0));	// rotate around y-axis
    glm::mat4 S = glm::scale(glm::mat4(1.0), glm::vec3(scale));
    return T * R * S;
}
