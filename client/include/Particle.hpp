#ifndef PARTICLE_H
#define PARTICLE_H

#include "core.h"
#include "Quad.h"
#include "Shader.h"

enum ParticleType {
    LINE,
    SPREAD
};

class Particle {
public:
    Particle();
    Particle(glm::vec3 position, glm::vec3 endPosition, glm::vec3 velocity, glm::vec4 startColor, glm::vec4 endColor, float lifeTime, float size, glm::vec3 direction, ParticleType type, bool isBillboard);
    Particle(glm::vec3 position, glm::vec3 endPosition, glm::vec3 velocity, const char* fileName, const std::string& directory, float lifeTime, float size, glm::vec3 direction, ParticleType type, bool isBillboard);
    ~Particle();

    void update(float deltaTime);
    void draw(const glm::mat4& viewMtx, const glm::mat4& projMtx);

    void resetForLine(glm::vec3 position, glm::vec3 velocity);
    void resetForSpread(glm::vec3 position, glm::vec3 velocity, int range);
    bool isActive() { return active; }
    void setPosition(glm::vec3 position) { this->position = position; }
    void setEndPosition(glm::vec3 position) { this->endPosition = position; }
    void setVelocity(glm::vec3 velocity) { this->velocity = velocity; }
    void setDirection(glm::vec3 direction) { this->direction = direction; }
    void setActive(bool active) { this->active = active; }
    void setIsBillboard(bool isBillboard) { this->isBillboard = isBillboard; }
    
private:
    glm::vec3 position;
    glm::vec3 endPosition;
    glm::vec3 velocity;
    glm::vec4 startColor;
    glm::vec4 endColor;
    glm::vec4 color;
    float rotation;
    float lifeTime;
    float dSize;
    float size;
    Quad* quad;
    float scale;
    bool active;
    float timePassed = 0.0f;
    GLuint shader;
    glm::vec3 direction;
    float distanceToEnd;
    float threshold = 0.5f;
    ParticleType type;
    bool isBillboard = false;
};

#endif