#include "Sector.h"
#include "Shader.h"
#include <cmath>

Sector::Sector() {

}

Sector::~Sector() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

Sector::Sector(float startAngle, float endAngle, float radius, float height, glm::vec2 center) 
    : startAngle(startAngle), endAngle(endAngle), radius(radius), height(height), center(center) {
    shader = Shader::GetShader(ShaderType::TEST);
    modelMtx = glm::mat4(1.0f);
    position = glm::vec3(0.0f);
    rotation = glm::vec3(0.0f);
    scale = glm::vec3(1.0f);
    color = glm::vec3(0.0f, 0.0f, 1.0f);
    int segments = 200;
    startAngle = glm::radians(startAngle);
    endAngle = glm::radians(endAngle);
    float angleStep = (endAngle - startAngle) / segments;
    positions.push_back(glm::vec3(center.x, height, center.y));
    for (int i = 0; i <= segments; i++) {
        float angle = startAngle + angleStep * i;
            positions.push_back(glm::vec3(center.x + radius * cos(angle), height, center.y + radius * sin(angle)));
    }
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), &positions[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Sector::draw(const glm::mat4& viewProjMtx) {
    std::cout<<"drawing sector"<<std::endl;
    glUseProgram(shader);
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, (float*)&viewProjMtx);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, (float*)&modelMtx);
    glUniform3fv(glGetUniformLocation(shader, "color"), 1, &color[0]);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, positions.size());
    glBindVertexArray(0);
    glUseProgram(0);
}

void Sector::update() {
    modelMtx = glm::translate(position) * glm::scale(glm::mat4(1.0f), scale);
    // modelMtx = glm::translate(position);
    // std::cout<<"position matrix" << glm::to_string(modelMtx) << std::endl;
    modelMtx = glm::rotate(modelMtx, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    modelMtx = glm::rotate(modelMtx, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    modelMtx = glm::rotate(modelMtx, glm::radians(rotation.z), glm::vec3(0, 0, 1));
}