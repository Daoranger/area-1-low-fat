#pragma once

#include "ofMain.h"
#include "Particle.h"


class ParticleSystem {
public:
    vector<Particle> particles;

	void add(Particle particle);
	void remove(int i);
	void integrate();
	void draw();
    vector<Particle> getParticles();
};