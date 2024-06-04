#include "ParticleSystem.hpp"


ParticleSystem::ParticleSystem() {

}

ParticleSystem::ParticleSystem(glm::vec3 startPosition, glm::vec3 endPosition, glm::vec3 velocity, glm::vec4 startColor, glm::vec4 endColor, float lifeTime, float size, int numParticles, int rate) 
    : rate(rate), numParticles(numParticles), velocity(velocity) { 
    for(int i = 0; i < numParticles; i++) {
        particles.push_back(new Particle(startPosition, velocity, startColor, endColor, lifeTime, size));
    }
    index = numParticles - 1;
}

ParticleSystem::~ParticleSystem() {
    for(int i = 0; i < particles.size(); i++) {
        delete particles[i];
    }
}
    
void ParticleSystem::update(float deltaTime) {
    for(int i = 0; i < particles.size(); i++) {
        if(!particles[i]->isActive()) {
            continue;
        }
        particles[i]->update(deltaTime);
    }
}

void ParticleSystem::draw(const glm::mat4& viewProjMtx) {
    for(int i = 0; i < particles.size(); i++) {
        if(!particles[i]->isActive()) {
            continue;
        }
        particles[i]->draw(viewProjMtx);
    }
}

void ParticleSystem::spawnParticles(glm::vec3 position) {
    Particle* p = particles[index];
    p->reset(position);
    index = (index - 1 + numParticles) % numParticles;
}
