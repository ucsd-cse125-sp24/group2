#pragma once
#include "core.h"

class Sector {
public:
    Sector();
    ~Sector();
    // angle in degrees or you can comment out the glm::radians()
    Sector(float startAngle, float endAngle, float radius, float height, glm::vec2 center);
    void draw(const glm::mat4& viewProjMtx);
    void update();
    void setPosition(glm::vec3 position) { this->position = position; }
    void setRotation(glm::vec3 rotation) { this->rotation = rotation; }
    void setScale(glm::vec3 scale) { this->scale = scale; }
    const glm::vec3& getPosition() const { return position; }
    const glm::vec3& getRotation() const { return rotation; }
    const glm::vec3& getScale() const { return scale; }
private:
    std::vector<glm::vec3> positions;
    glm::mat4 modelMtx;
    GLuint VAO, VBO;
    GLuint shader;
    glm::vec3 color;
    float startAngle;
    float endAngle;
    float radius;
    float height;
    glm::vec2 center;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};

