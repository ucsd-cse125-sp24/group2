#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "Particle.hpp"

class ParticleSystem {
public:
    ParticleSystem();
    ParticleSystem(glm::vec3 velocity, float lifeTime, float size, int numParticles, float rate, const char* fileName, const std::string& directory, ParticleType type, bool isBillboard);
    ParticleSystem(glm::vec3 velocity, float lifeTime, float size, int numParticles, float rate, glm::vec4 startColor, glm::vec4 endColor, ParticleType type, bool isBillboard);
    ParticleSystem(glm::vec3 startPosition, glm::vec3 endPosition, glm::vec3 velocity, glm::vec4 startColor, glm::vec4 endColor, float lifeTime, float size, int numParticles, float rate, ParticleType type, bool isBillboard);
    ~ParticleSystem();

    void update(float deltaTime);
    void draw(const glm::mat4& viewMtx, const glm::mat4& projMtx);
    void spawnParticles(glm::vec3 position, ParticleType type);
    void start(float deltaTime, ParticleType type, float duration);
    void setData(glm::vec3 startPosition, glm::vec3 targetPosition, glm::vec3 velocity);
    const glm::vec3& getVelocity() const { return velocity; }
    void reset();
private:
    std::vector<Particle*> particles;
    int numParticles;
    int index;
    float rate;
    glm::vec3 velocity;
    glm::vec3 startPosition;
    float timeSinceLastSpawn = 0.0f;
    glm::vec3 direction;
    int count = 0;
    glm::vec3 targetPosition;
    ParticleType type;
    float elapsedTime = 0.0f;
    float isFinished = false;
};



#endif