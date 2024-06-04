#ifndef PARTICLE_H
#define PARTICLE_H

#include "core.h"
#include "Quad.h"
#include "Shader.h"

class Particle {
public:
    Particle();
    Particle(glm::vec3 position, glm::vec3 velocity, glm::vec4 startColor, glm::vec4 endColor, float lifeTime, float size);
    ~Particle();

    void update(float deltaTime);
    void draw(const glm::mat4& viewProjMtx);

    bool isActive() { return active; }
    void reset(glm::vec3 position);
    
private:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec4 startColor;
    glm::vec4 endColor;
    glm::vec4 color;
    float lifeTime;
    float size;
    Quad* quad;
    float scale;
    float rotation;
    bool active;
    float timePassed = 0.0f;
    GLuint shader;
};

#endif