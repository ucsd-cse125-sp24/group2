#include "Rectangle.h"

Rectangle::Rectangle() {

}

Rectangle::Rectangle(float width, float height, glm::vec3 pos) {
    color = glm::vec3(1.0f, 1.0f, 1.0f);
    modelMtx = glm::mat4(1.0f) * glm::translate(glm::mat4(1.0f), pos);
    positions = {
        glm::vec3(pos.x + width/2.0f, pos.y + height/2.0f, pos.z), // Top right
        glm::vec3(pos.x + width/2.0f, pos.y - height/2.0f, pos.z), // Bottom right
        glm::vec3(pos.x - width/2.0f, pos.y - height/2.0f, pos.z), // Bottom left
        glm::vec3(pos.x - width/2.0f, pos.y + height/2.0f, pos.z) // Top left
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO_Pos);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Pos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
}

Rectangle::~Rectangle() {
    glDeleteBuffers(1, &VBO_Pos);
    glDeleteVertexArrays(1, &VAO);
}

void Rectangle::draw(const glm::mat4& viewProjMtx, GLuint shader) {
    glUseProgram(shader);
    glUniform1i(glGetUniformLocation(shader, "hasTexture"), 0);
    glUniform3fv(glGetUniformLocation(shader, "color"), 1, (float*)&color);
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, (float*)&viewProjMtx);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, (float*)&modelMtx);

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
}