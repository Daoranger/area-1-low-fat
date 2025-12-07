#pragma once

#include "ofMain.h"


class Particle {
    public:
        Particle();

        ofVec3f velocity;
        ofVec3f acceleration;
        ofVec3f position;
        ofColor color;
        float rotation;
        float rotSpeed;
        float rotAcceleration;
        float lifespan = 5;
        float birthtime = ofGetElapsedTimeMillis();
        float radius = .5;
        float damping = .99;
        float mass = 1;


        bool lifeExpired();
        void setLifespan(int life);
        void draw();
        void setForce(float x, float y, float z);
        void integrate();        
};
