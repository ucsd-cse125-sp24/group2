#pragma once
#include "core.h"

class Rectangle {
public:
    Rectangle();
    Rectangle(float width, float height, glm::vec3 pos);
    ~Rectangle();
    void draw(const glm::mat4& viewProjMtx, GLuint shader);
    glm::vec3 getPosition() { return modelMtx[3]; }
    glm::mat4 modelMtx;
private:
    std::vector<glm::vec3> positions;
    GLuint VAO;
    GLuint VBO_Pos;
    glm::vec3 color;
};