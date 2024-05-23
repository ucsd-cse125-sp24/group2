#pragma once
#include "core.h"
#include "Quad.h"
#include "Shader.h"
class Particle {
public:
    Particle();
    Particle(glm::vec3 position, glm::vec3 velocity, float lifeTime, float scale, glm::vec4 color, glm::vec4 endColor);
    const glm::vec3& getPosition() const { return position; }
    const glm::vec3& getColor() const { return color; }
    void reset(glm::vec3 position, glm::vec3 velocity);
    void update(float dt);
    void draw(const glm::mat4& viewProjMtx);
    bool isActive;
    float timePassed = 0.0f;
private:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec4 color;
    glm::vec4 startColor;
    glm::vec4 endColor;
    float lifeTime;
    float scale;
    float rotation;
    Quad* quad;
    float angle;
    float angularVelocity;
};