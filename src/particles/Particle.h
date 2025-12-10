/**
 * Alison Schonauer and Hoang Nguyen
 * CS 134: Computer Game Design and Programming
 * Professor Kevin Smith
 * Originally created by Kevin Smith
 * Fall 2025
 */

#pragma once

#include "ofMain.h"

// Particle shapes
enum PARTICLE_SHAPE {DISK, SPHERE, CUBE};

class Particle {
    public:
        Particle();

        PARTICLE_SHAPE shape = SPHERE;  // Shape of particles

        // Particle Attributes
        //
        glm::vec3 rotAxis;
        ofVec3f velocity;
        ofVec3f acceleration;
        ofVec3f position;
        ofColor color;
        float rotation;
        float rotSpeed;
        float rotAcceleration;
        float scale = 1;
        float scaleRate = 1;    // Used for particles to increase/decrease in size over time
        float lifespan = 5;
        float birthtime = ofGetElapsedTimeMillis();
        float radius = .5;
        float damping = .99;
        float mass = 1;


        bool lifeExpired();
        void setLifespan(int life);
        void draw();
        void setForce(float x, float y, float z);
        void setShape(PARTICLE_SHAPE s);
        void integrate();        
};
