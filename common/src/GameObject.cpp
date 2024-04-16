#include "../include/GameObject.hpp"
#include "../include/Component.hpp"
#include "../include/Transform.hpp"


GameObject::GameObject(glm::vec3 pos = glm::vec3(), glm::vec3 rot = glm::vec3(), glm::vec3 scale = glm::vec3()) {
    Transform transform = Transform(pos, rot, scale);
    addComponent(transform);
}