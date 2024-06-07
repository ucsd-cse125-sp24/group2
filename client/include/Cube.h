#pragma once

#include <vector>

#include "core.h"

class Cube {
private:
    GLuint VAO;
    GLuint VBO_positions, VBO_normals, EBO;

    glm::mat4 modelMtx;
    glm::vec3 color;

    // Cube Information
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;

    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;

public:
    Cube(glm::vec3 cubeMin = glm::vec3(-1, -1, -1), glm::vec3 cubeMax = glm::vec3(1, 1, 1));
    ~Cube();

    void draw(const glm::mat4& viewProjMtx, GLuint shader);
    void update();
    void setPosition(glm::vec3 pos) { position = pos; }
    void setRotation(glm::vec3 rot) { rotation = rot; }
    void setScale(glm::vec3 scal) { scale = scal; }
    const glm::mat4& getModelMtx();

    void spin(float deg);
};