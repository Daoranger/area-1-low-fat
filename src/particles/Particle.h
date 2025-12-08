#pragma once

#include "ofMain.h"

enum PARTICLE_SHAPE {DISK, SPHERE, CUBE};

class Particle {
    public:
        Particle();

        PARTICLE_SHAPE shape = SPHERE;

        glm::vec3 rotAxis;
        ofVec3f velocity;
        ofVec3f acceleration;
        ofVec3f position;
        ofColor color;
        float rotation;
        float rotSpeed;
        float rotAcceleration;
        float scale = 1;
        float scaleRate = 1;
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
