
#include "ParticleEmitter.h"

// Draws all particles stored in the system
void ParticleEmitter::draw() {
    particleSys->draw();
};

// Starts emitting based on the started timer
void ParticleEmitter::start() {
    active = true;
};

// Stops emitting based on the timer
void ParticleEmitter::stop() {
    active = false;
};

// Sets lifespan in seconds
void ParticleEmitter::setLifespan(float life) {
    lifespan = life;
};

// Sets Particle shape
void ParticleEmitter::setParticleShape(PARTICLE_SHAPE s) {
    shape = s;
};

// Sets Emitter shape
void ParticleEmitter::setEmitterShape(EMITTER_SHAPE s) {
    emitterShape = s;
};

// Updates the timer and emits when it hits 0
void ParticleEmitter::update() {
    if (active) {
        timer -= 1/ofGetFrameRate();
        if (timer <= 0) {
            emit();
            timer = rate;
        }
    }
};

// Emitts particles based on emitter shape and uses launchParticle()
void ParticleEmitter::emit() {
    switch (emitterShape) {
        // Launches Particle in current direction
        case DirectionalEmitter: {
            launchParticle();
            break;
        }
        // Launches particles in a circle (Unused)
        case RadialEmitter: {
            ofVec3f origDir = direction;
            float step = 360 / numParticles;
            for (int i = 0; i < numParticles; i ++) {
                launchParticle();
                direction = direction.rotate(step, direction.getPerpendicular(ofVec3f(0,0,0)));
            };
            direction = origDir;
            break;
        }
        // Launches Particles in a sphere
        case SphereEmitter: {
            // saves and loads original direction
            ofVec3f origDir = direction;
            // For the number of particles wanted launch a new particle and then randomize the direction on the sphere
            for (int i = 0; i < numParticles; i ++) {
                launchParticle();
                direction = glm::normalize(glm::vec3(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1)));
            };
            direction = origDir;
            break;
        }
        // Launches Particles in a cone
        case ConeEmitter: {
            // Saves and loads original direction
            ofVec3f origDir = direction;
            // For number of particles launch a new one, and then rotate a maximum of 25 degrees on a random axis
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

// Integrates particle system
void ParticleEmitter::integrateParticles() {
    particleSys->integrate();
};


// Launches a new particle based on current variables
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

// Launches a particle based on given speed, rotational speed, and acceleration
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

// Returns particles in the system
vector<Particle> ParticleEmitter::getParticles() {
    return particleSys->getParticles();
};
