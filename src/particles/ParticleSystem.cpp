
#include "ParticleSystem.h"

void ParticleSystem::add(Particle particle) {
    particles.push_back(particle);
};

void ParticleSystem::remove(int i) {
    particles.erase(particles.begin() + i);
};
	
void ParticleSystem::integrate() {
    for (int i = 0; i < int(particles.size()); i++) {
        particles[i].integrate();
        if (particles[i].lifeExpired()) {
            remove(i);
        }
    }
};

void ParticleSystem::draw() {
    for (int i = 0; i < int(particles.size()); i++) {
        particles[i].draw();
    }
};

vector<Particle> ParticleSystem::getParticles() {
    return particles;
};