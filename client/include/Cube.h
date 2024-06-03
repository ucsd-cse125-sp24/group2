#pragma once

#include <vector>

#include "core.h"
#include <iostream>

class Cube {
private:
    GLuint VAO;
    GLuint VBO_positions, VBO_normals, EBO;

    glm::mat4 model;
    glm::vec3 color;

    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;

    // Cube Information
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;

public:
    Cube(glm::vec3 cubeMin = glm::vec3(-1, -1, -1), glm::vec3 cubeMax = glm::vec3(1, 1, 1));
    ~Cube();

    void draw(const glm::mat4& viewProjMtx, GLuint shader);
    void update(float dt, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);
    const glm::mat4& getModelMtx();
    void setPosition(glm::vec3 pos);
    void setRotation(glm::vec3 rot);
    void setScale(glm::vec3 scale);

    void spin(float deg);
};