
#include "Beam.h"

Beam::Beam() {
    active = false;
    radius = 1;
    height = 0;
    maxHeight = 10;
    extendRate = 2;
    retractRate = 4;
};

void Beam::toggle() {
    active = !active;
}

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

void Beam::draw() {
    if (height == 0) return;
    ofEnableAlphaBlending();
    ofSetColor(50, 255, 85, 80);
    ofDrawCylinder(pos, radius, height);
    ofDisableAlphaBlending();
    ofSetColor(ofColor::white, 255);
};

bool Beam::checkInside(Box box) {
    if (!active) return false;
    float boxMinY = box.min().y();
    float boxMaxY = box.max().y();
    float cylMinY = pos.y - height/2;
    float cylMaxY = pos.y + height/2;

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

    float distance = glm::distance(glm::vec3(boxX, 0, boxZ), glm::vec3(pos.x, 0, pos.z));

    return ((cylMinY <= boxMaxY && cylMaxY >= boxMinY) && (distance <= radius));
};
