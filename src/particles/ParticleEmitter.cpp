#pragma once

#include "ParticleEmitter.h"


void ParticleEmitter::draw() {
    particleSys->draw();
};

void ParticleEmitter::setLifespan(float life) {
    lifespan = life;
};

void ParticleEmitter::setShape(SHAPE s) {
    shape = s;
};

void ParticleEmitter::integrateParticles() {
    particleSys->integrate();
};

void ParticleEmitter::launchParticle(float speed, float ang_speed, float acceleration) {
            
    Particle particle;
    particle.color = color;
    particle.position = position;
    particle.rotation = rotation;
    particle.ang_velocity = ang_speed;
    particle.velocity = speed * ofVec3f(0, -1, 0).rotate(rotation, ofVec3f(0,0,1));
    particle.acceleration = acceleration * ofVec3f(0, -1, 0).rotate(rotation, ofVec3f(0,0,1));
    particle.damping = damping;
    particle.setLifespan(lifespan);
                
    particleSys->add(particle);
};

vector<Particle> ParticleEmitter::getParticles() {
    return particleSys->getParticles();
};