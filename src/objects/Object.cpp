#include "Object.h"

Object::Object()
{
	position.set(0, 200, 0);
	rotation = 0.0;
	scale.set(1, 1, 1);
}

void Object::draw()
{
    ofPushMatrix();
    ofMultMatrix(getTransform());
    if (modelColorsLoaded)
        modelColor.drawFaces();
    else
        model.drawFaces();
    ofNoFill();
    ofSetColor(ofColor::white);
    octree.drawLeafNodes(octree.root);
    ofFill();
    ofPopMatrix();
}

void Object::integrate()
{
    return;
}

void Object::loadModel()
{
    return;
}

void Object::updateBoundingBox()
{
    return;
}

void Object::createOctree()
{
    octree.create(model.getMesh(0), 10);
}

void Object::updateOctree()
{
    //octree.box 
}

void Object::handleCollision(Ufo& ufo)
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
