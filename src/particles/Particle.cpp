
#pragma once

#include "Particle.h"

Particle::Particle() {
    velocity.set(0,0,0);
    acceleration.set(0, 0, 0);
    position.set(0, 0, 0);
    rotSpeed = 0;
    rotAcceleration = 0;
};


bool Particle::lifeExpired() {
    return lifespan >= 0 && ofGetElapsedTimeMillis() - birthtime > lifespan;
};

void Particle::setLifespan(int life) {
    lifespan = life * 1000;
};

       

void Particle::draw() {
    ofSetColor(color);
    
};

void Particle::setForce(float x, float y, float z) {
    acceleration.x = x / mass;
    acceleration.y = y / mass;
    acceleration.z = z / mass;
};

void Particle::integrate() {
    position += velocity * (1/ofGetFrameRate());
    velocity += acceleration * (1/ofGetFrameRate());
    velocity = velocity * damping;

    rotation += rotSpeed * (1/ofGetFrameRate());
    rotSpeed += rotAcceleration * (1/ofGetFrameRate());
    rotSpeed = rotSpeed * damping;
};

