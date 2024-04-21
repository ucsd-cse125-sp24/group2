#include "../include/Transform.hpp"


// FUNCTION DEFINITIONS

Transform::Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) {
    this->pos = pos;
    this->rot = rot;
    this->scale = scale;
}

void Transform::receive(void* value) {
    glm::vec3* newPosition = reinterpret_cast<glm::vec3*>(value);
    setPos(*newPosition);
}