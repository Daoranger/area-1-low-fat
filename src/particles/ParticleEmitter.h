
#pragma once

#include "ofMain.h"
#include "ParticleSystem.h"

class ParticleEmitter {
    public:
        ParticleSystem*particleSys = new ParticleSystem();

        ofColor color;
        float lifespan = 5;
        float damping = 0.99;
        float rotation;
        ofVec3f position;

        enum SHAPE {DISK, SPHERE, CUBE};

        SHAPE shape = SPHERE;

        void draw();
        void setLifespan(float life);
        void setShape(SHAPE s);
        void integrateParticles();
        void launchParticle(float speed, float ang_speed, float acceleration);
        vector<Particle> getParticles();
};