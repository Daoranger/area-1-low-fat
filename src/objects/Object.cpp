/**
 * Alison Schonauer and Hoang Nguyen
 * CS 134: Computer Game Design and Programming
 * Professor Kevin Smith
 * Fall 2025
 */

#include "Object.h"

Object::Object()
{
	position.set(0, 200, 0);
	rotation = 0.0;
	scale.set(1, 1, 1);
}

// Default draw function, can be overridden by derived classes
//
void Object::draw()
{
    ofPushMatrix();
    ofMultMatrix(getTransform());
    if (modelColorsLoaded)
        modelColor.drawFaces();
    else
        model.drawFaces();
    //ofNoFill();
    //ofSetColor(ofColor::white);
    //octree.drawLeafNodes(octree.root);
    //ofFill();
    ofPopMatrix();
}

// Default integrate function, to be overridden by derived classes that required integration physics (e.g., Ufo and cow)
//
void Object::integrate()
{
    return;
}

// Default loadModel function, to be overridden by derived classes
//
void Object::loadModel()
{
    return;
}

// Default updateBoundingBox function, to be overridden by derived classes that need bouding box
//
void Object::updateBoundingBox()
{
    return;
}

// Default createOctree function, to be overridden by derived classes that need octree
//
void Object::createOctree()
{
    octree.create(model.getMesh(0), 10);
}

// Default updateOctree function, to be overridden by derived classes that need octree
//
void Object::updateOctree()
{
    //octree.box 
}

// Default handleCollision function, to be overridden by derived classes that need collision handling
void Object::handleCollision(Ufo& ufo)
{
    return;
}

// Get the transformation matrix of the object
//
glm::mat4 Object::getTransform()
{
    glm::mat4 T = glm::translate(glm::mat4(1.0), glm::vec3(position));
    glm::mat4 R = glm::rotate(glm::mat4(1.0), glm::radians(rotation), glm::vec3(0, 1, 0));	// rotate around y-axis
    glm::mat4 S = glm::scale(glm::mat4(1.0), glm::vec3(scale));
    return T * R * S;
}
