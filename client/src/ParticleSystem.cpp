#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() {

}

ParticleSystem::ParticleSystem(glm::vec3 position, glm::vec4 color, glm::vec4 endColor, glm::vec3 veclocity, float lifeTime, int numParticles, float size) :
    position(position), color(color), endColor(endColor), velocity(veclocity), lifeTime(lifeTime), numParticles(numParticles){
        for(int i = 0; i < numParticles; i++) {
            particles.push_back(new Particle(position, veclocity, lifeTime, size, color, endColor));
        }
        index = numParticles - 1;
}

ParticleSystem::ParticleSystem(glm::vec3 position, glm::vec3 velocity, float lifeTime, int numParticles, float size, const char* fileName, const std::string& directory) :
    position(position), velocity(velocity), lifeTime(lifeTime), numParticles(numParticles) {
        for(int i = 0; i < numParticles; i++) {
            particles.push_back(new Particle(position, velocity, lifeTime, size, fileName, directory));
        }
        index = numParticles - 1;
}


void ParticleSystem::draw(const glm::mat4& viewProjMtx) {
    for(int i = 0; i < particles.size(); i++) {
        if(!particles[i]->isActive) {
            continue;
        }
        particles[i]->draw(viewProjMtx);
    }
}

void ParticleSystem::update(float dt) {
    for(int i = 0; i < particles.size(); i++) {
        if(!particles[i]->isActive) {
            continue;
        }
        particles[i]->update(dt);
    }
}

void ParticleSystem::spawnParticles(glm::vec3 position) {
    Particle* p = particles[index];
    p->reset(position, velocity);
    index = (index - 1 + numParticles) % numParticles;
}