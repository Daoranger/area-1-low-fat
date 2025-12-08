#pragma once

#include "ofMain.h"
#include "ParticleSystem.h"

enum EMITTER_SHAPE {DirectionalEmitter, RadialEmitter, SphereEmitter};

class ParticleEmitter {
    public:
        ParticleSystem*particleSys = new ParticleSystem();

        ofColor color;
        float lifespan = 5;
        float damping = 0.99;
        float rotation = 0;
        float timer = 1;
        float rate = 1;
        float radius = 0.5;
        float numParticles = 10;
        float scale= 1;
        float scaleRate = 1;
        ofVec3f position;
        ofVec3f direction = ofVec3f(0, -1, 0);

        float speed;
        float rotSpeed;
        float acceleration;

        bool active = false;

        PARTICLE_SHAPE shape = SPHERE;
        EMITTER_SHAPE emitterShape = DirectionalEmitter;

        void draw();
        void start();
        void stop();
        void setLifespan(float life);
        void setParticleShape(PARTICLE_SHAPE s);
        void setEmitterShape(EMITTER_SHAPE s);
        void update();
        void emit();
        void integrateParticles();
        void launchParticle();
        void launchParticle(float speed, float rotSpeed, float acceleration);
        

        vector<Particle> getParticles();
};