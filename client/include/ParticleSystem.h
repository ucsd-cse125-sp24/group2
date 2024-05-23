#pragma once

#include "Particle.h"
#include <vector>

class ParticleSystem {
public:
    ParticleSystem();
    ParticleSystem(glm::vec3 position, glm::vec4 color, glm::vec4 endColor, glm::vec3 veclocity, float lifeTime, int numParticles, float size);
    ParticleSystem(glm::vec3 position, glm::vec3 velocity, float lifeTime, int numParticles, float size, const char* fileName, const std::string& directory);
    void update(float dt);
    void draw(const glm::mat4& viewProjMtx);
    void spawnParticles(glm::vec3 position);
private:
    glm::vec3 position;
    glm::vec4 color;
    glm::vec4 endColor;
    glm::vec3 velocity;
    float lifeTime;
    bool spawn;
    std::vector<Particle*> particles;
    int numParticles;
    int index;
};