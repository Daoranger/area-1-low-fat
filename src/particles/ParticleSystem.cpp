
#include "ParticleSystem.h"

// Add particle to system
void ParticleSystem::add(Particle particle) {
    particles.push_back(particle);
};

// Remove particle from system
void ParticleSystem::remove(int i) {
    particles.erase(particles.begin() + i);
};
	
// Integrate particles in system
void ParticleSystem::integrate() {
    for (int i = 0; i < int(particles.size()); i++) {
        particles[i].integrate();
        if (particles[i].lifeExpired()) {
            remove(i);
        }
    }
};

// Draw particles in system
void ParticleSystem::draw() {
    for (int i = 0; i < int(particles.size()); i++) {
        particles[i].draw();
    }
};

// Return particles in system
vector<Particle> ParticleSystem::getParticles() {
    return particles;
};