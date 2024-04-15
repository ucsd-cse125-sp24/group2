#include "../include/GameObject.hpp"

Transform::Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) {
    this->pos = pos;
    this->rot = rot;
    this->scale = scale;
}

GameObject::GameObject(glm::vec3 pos = glm::vec3(), glm::vec3 rot = glm::vec3(), glm::vec3 scale = glm::vec3()) {
    Transform transform = Transform(pos, rot, scale);
    components.push_back(transform);
}