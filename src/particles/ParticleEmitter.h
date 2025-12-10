/**
 * Alison Schonauer and Hoang Nguyen
 * CS 134: Computer Game Design and Programming
 * Professor Kevin Smith
 * Originally created by Kevin Smith
 * Edited by Alison Schonauer
 * Fall 2025
 */

#pragma once

#include "ofMain.h"
#include "ParticleSystem.h"

// Emitter shape types
enum EMITTER_SHAPE {DirectionalEmitter, RadialEmitter, SphereEmitter, ConeEmitter};

class ParticleEmitter {
    public:
        // Particle System from the emitter
        ParticleSystem*particleSys = new ParticleSystem();

        // Emitter and individual particle attributes
        //
        vector<ofColor> colors; // An array of possible particle colors (randomized)
        float lifespan = 5;
        float damping = 0.99;
        float rotation = 0;
        float timer = 0;
        float rate = 1;
        float radius = 0.5;
        float numParticles = 10;
        float scale= 1;
        float scaleRate = 1;    // Used for particles to increase/decrease in size over time
        ofVec3f position;
        ofVec3f direction = ofVec3f(0, -1, 0);

        float speed;
        float rotSpeed;
        float rotAccel;
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