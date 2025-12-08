
#include "ParticleEmitter.h"


void ParticleEmitter::draw() {
    particleSys->draw();
};

void ParticleEmitter::start() {
    active = true;
};

void ParticleEmitter::stop() {
    active = false;
};

void ParticleEmitter::setLifespan(float life) {
    lifespan = life;
};

void ParticleEmitter::setParticleShape(PARTICLE_SHAPE s) {
    shape = s;
};

void ParticleEmitter::setEmitterShape(EMITTER_SHAPE s) {
    emitterShape = s;
};

void ParticleEmitter::update() {
    if (active) {
        timer -= 1/ofGetFrameRate();
        if (timer <= 0) {
            emit();
            timer = rate;
        }
    }
};

void ParticleEmitter::emit() {
    switch (emitterShape) {
        case DirectionalEmitter: {
            launchParticle();
            break;
        }
        case RadialEmitter: {
            float step = 360 / numParticles;
            for (int i = 0; i < numParticles; i ++) {
                launchParticle();
                rotation += step;
            };
            break;
        }
        case SphereEmitter: {
            ofVec3f origDir = direction;
            for (int i = 0; i < numParticles; i ++) {
                launchParticle();
                direction = glm::normalize(glm::vec3(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1)));
            };
            direction = origDir;
            break;
        }
        case ConeEmitter: {
            ofVec3f origDir = direction;
            for (int i = 0; i < numParticles; i ++) {
                launchParticle();
                direction = origDir;
                direction = direction.rotate(ofRandom(-25, 26), ofVec3f(ofRandom(0, 1), ofRandom(0, 1), ofRandom(0, 1)));
            };
            direction = origDir;
            break;
        }
    }
};

void ParticleEmitter::integrateParticles() {
    particleSys->integrate();
};

void ParticleEmitter::launchParticle() {
            
    Particle particle;
    particle.color = colors[ofRandom(0, colors.size())];
    particle.setShape(shape);
    particle.radius = radius;
    particle.position = position;
    particle.rotation = rotation;
    particle.rotSpeed = rotSpeed;
    particle.rotAcceleration = rotAccel;
    particle.rotAxis = glm::vec3(ofRandom(0, 1), ofRandom(0, 1), ofRandom(0, 1));
    particle.velocity = speed * direction;
    particle.acceleration = acceleration * direction;
    particle.scale = scale;
    particle.scaleRate = scaleRate;
    particle.damping = damping;
    particle.setLifespan(lifespan);
    
    particleSys->add(particle);
};

void ParticleEmitter::launchParticle(float speed, float rotSpeed, float acceleration) {
            
    Particle particle;
    particle.color = colors[ofRandom(0, colors.size())];
    particle.setShape(shape);
    particle.radius = radius;
    particle.position = position;
    particle.rotation = rotation;
    particle.rotSpeed = rotSpeed;
    particle.rotAcceleration = rotAccel;
    particle.rotAxis = glm::vec3(ofRandom(0, 1), ofRandom(0, 1), ofRandom(0, 1));
    particle.velocity = speed * direction;
    particle.acceleration = acceleration * direction;
    particle.damping = damping;
    particle.setLifespan(lifespan);
                
    particleSys->add(particle);
};

vector<Particle> ParticleEmitter::getParticles() {
    return particleSys->getParticles();
};
