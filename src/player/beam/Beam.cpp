/**
 * Alison Schonauer and Hoang Nguyen
 * CS 134: Computer Game Design and Programming
 * Professor Kevin Smith
 * Fall 2025
 */

#include "Beam.h"

Beam::Beam() {
    active = false;
    radius = 1;
    height = 0;
    maxHeight = 10;
    extendRate = 2;
    retractRate = 4;
};

// Toggle the beam on or off 
//
void Beam::toggle() {
    active = !active;
}

// Update the beam's height based on its active state
//
void Beam::update() {
    capturePoint.x = pos.x;
    capturePoint.z = pos.z;
    capturePoint.y = pos.y - 2;
    if (active) {
        if (height < maxHeight)
            height += extendRate;
        else 
            height = maxHeight;
    }
    else {
        if (height > 0)
            height -= retractRate;
        else
            height = 0;
    }
};

// Draw the beam as a semi-transparent cylinder
//
void Beam::draw() {
    if (height == 0) return;
    ofEnableAlphaBlending();
    ofSetColor(50, 255, 85, 80);
    ofSetCylinderResolution(64, 1);
    ofDrawCylinder(pos, radius, height);
    ofDisableAlphaBlending();
    ofSetColor(ofColor::white, 255);
};

// Check if there is a bouding box within the beam.
// This will be use for checking if the cow's bouding box
// is within the beam so we can start abducting the cow and apply
// physics and other thing to the cow.
//
bool Beam::checkInside(Box box) {
    if (!active) return false;
    float boxMinY = box.min().y();
    float boxMaxY = box.max().y();
    float cylMinY = pos.y - height/2;
    float cylMaxY = pos.y + height/2;

    // Used to calculate xz side of box point closest to the beam
    float boxX = pos.x;
    float boxZ = pos.z;

    if (boxX > box.max().x()) {
        boxX = box.max().x();
    }
    else if (boxX < box.min().x()) {
        boxX = box.min().x();
    }

    if (boxZ > box.max().z()) {
        boxZ = box.max().z();
    }
    else if (boxZ < box.min().z()) {
        boxZ = box.min().z();
    }

    // Used to see if the distance on the xz plane between the beam and box is within the radius of the beam
    float distance = glm::distance(glm::vec3(boxX, 0, boxZ), glm::vec3(pos.x, 0, pos.z));

    // Checks the radius vs distance as well as the y-range of the box overlapping the cylinder
    return ((cylMinY <= boxMaxY && cylMaxY >= boxMinY) && (distance <= radius));
};
