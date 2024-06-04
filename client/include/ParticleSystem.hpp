#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "Particle.hpp"

class ParticleSystem {
public:
    ParticleSystem();
    ParticleSystem(glm::vec3 startPosition, glm::vec3 endPosition, glm::vec3 velocity, glm::vec4 startColor, glm::vec4 endColor, float lifeTime, float size, int numParticles, int rate);
    ~ParticleSystem();

    void update(float deltaTime);
    void draw(const glm::mat4& viewProjMtx);
    void spawnParticles(glm::vec3 position);
private:
    std::vector<Particle*> particles;
    int numParticles;
    int index;
    int rate;
    glm::vec3 velocity;
};


#endif