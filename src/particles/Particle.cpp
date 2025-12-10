/**
 * Alison Schonauer and Hoang Nguyen
 * CS 134: Computer Game Design and Programming
 * Professor Kevin Smith
 * Originally Created by Kevin Smith
 * Fall 2025
 */

#include "Particle.h"

Particle::Particle() {
    velocity.set(0,0,0);
    acceleration.set(0, 0, 0);
    position.set(0, 0, 0);
    rotSpeed = 0;
    rotAcceleration = 0;
};

// Checks if particle life is expired
bool Particle::lifeExpired() {
    return lifespan >= 0 && ofGetElapsedTimeMillis() - birthtime > lifespan;
};

// Sets life in ms given a time for life in seconds
void Particle::setLifespan(int life) {
    lifespan = life * 1000;
};
       
// Drawing the particle
void Particle::draw() {
    ofSetColor(color);
    ofFill();
    ofEnableAlphaBlending();
    ofSetSphereResolution(64);
    ofSetCylinderResolution(64, 1);
    ofPushMatrix();
    ofTranslate(position);
    ofRotateDeg(rotation, rotAxis.x, rotAxis.y, rotAxis.z);
    // Draws a different shape based on which is selected
    switch(shape) {
        case DISK:
            ofDrawCylinder(radius*scale, 2);
            break;
        case SPHERE:
            ofDrawSphere(radius*scale);
            break;
        case CUBE:
            ofDrawBox(radius*scale);
            break;
    }
    ofPopMatrix();
    ofDisableAlphaBlending();
};

// Sets a force on the particle
void Particle::setForce(float x, float y, float z) {
    acceleration.x = x / mass;
    acceleration.y = y / mass;
    acceleration.z = z / mass;
};

// Sets particle shape (Disk, Sphere, Cube)
void Particle::setShape(PARTICLE_SHAPE s) {
    shape = s;
};

// Integrates the particle's physics
void Particle::integrate() {
    position += velocity * (1/ofGetFrameRate());
    velocity += acceleration * (1/ofGetFrameRate());
    velocity = velocity * damping;

    rotation += rotSpeed * (1/ofGetFrameRate());
    rotSpeed += rotAcceleration * (1/ofGetFrameRate());
    rotSpeed = rotSpeed * damping;

    scale *= scaleRate;
};

