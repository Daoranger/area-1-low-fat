
#include "Beam.h"

Beam::Beam() {
    radius = 1;
    height = 0;
    maxHeight = 10;
    extendRate = 2;
    retractRate = 4;
};

void Beam::activate() {
    active = true;
};

void Beam::deactivate() {
    active = false;
};

void Beam::update() {
    if (active) {
        if (height < maxHeight)
            height += 2;
        else 
            height = maxHeight;
    }
    else {
        if (height > 0)
            height -= 2;
        else
            height = 0;
    }
};

void Beam::draw() {
    if (height == 0) return;
    ofDrawCylinder(pos, radius, height);
    
};

bool Beam::checkInside(Box box) {
    float boxMinY = box.min().y();
    float boxMaxY = box.max().y();
    float cylMinY = pos.y;
    float cylMaxY = pos.y + height;

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
